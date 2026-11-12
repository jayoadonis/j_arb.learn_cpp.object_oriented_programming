include(FetchContent)

find_package(Catch2 3.16.0 QUIET)

if(Catch2_FOUND AND NOT ${project.TARGET_BASENAME_UP}_FETCHCONTENT)
  message(STATUS "::: Catch2 3.16.0 found locally")
else()
  if(${project.TARGET_BASENAME_UP}_FETCHCONTENT)
    message(STATUS "::: Catch2 3.16.0, Fetching from GitHub...")
  else()
    message(STATUS "::: Catch2 3.16.0 not found locally. Fetching from GitHub...")
  endif()

  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.16.0

    # REM: Only downloads the single requested commit (no history)
    GIT_SHALLOW ON

    # REM: Prints progress so you see download feedback
    GIT_PROGRESS ON

    # REM: Marks headers as SYSTEM to bypass compiler warning checks
    SYSTEM ON)

  set(BUILD_TESTING OFF CACHE INTERNAL "")
  set(CATCH_BUILD_EXAMPLES OFF CACHE INTERNAL "")
  set(CATCH_BUILD_EXTRA_TESTS OFF CACHE INTERNAL "")

  # REM: REQUIRED: Makes 'catch_discover_tests' available
  set(CATCH_INSTALL_HELPERS ON CACHE INTERNAL "")

  FetchContent_MakeAvailable(Catch2)

  list(INSERT CMAKE_MODULE_PATH 0
    "${catch2_SOURCE_DIR}/extras")

  # if(TARGET Catch2)
  # target_link_libraries(Catch2
  # PRIVATE ${project.TARGET_BASENAME}_common)
  # endif()

  # if(TARGET Catch2WithMain)
  # target_link_libraries(Catch2WithMain
  # PRIVATE ${project.TARGET_BASENAME}_common)
  # endif()
  if(TARGET Catch2)
    target_compile_features(
      Catch2
      PRIVATE "${project.CXX_STD}")

    if(MSVC)
      target_compile_options(Catch2 PRIVATE /W0)
    else()
      target_compile_options(Catch2 PRIVATE -w)
    endif()

    # enable_target_sanitizers(
    #   Catch2 ${project.TARGET_BASENAME_UP})
  endif()

  if(TARGET Catch2WithMain)
    target_compile_features(
      Catch2WithMain
      PRIVATE "${project.CXX_STD}")

    if(MSVC)
      target_compile_options(Catch2WithMain PRIVATE /W0)
    else()
      target_compile_options(Catch2WithMain PRIVATE -w)
    endif()

    # enable_target_sanitizers(
    #   Catch2WithMain ${project.TARGET_BASENAME_UP})
  endif()
endif()

include(Catch)

file(GLOB_RECURSE
  project.SRC_TEST_CXX_PRIVATE_IMPL_SET
  CONFIGURE_DEPENDS
  "${project.SRC_TEST_CXX_DIR}/private/*.cc"
  "${project.SRC_TEST_CXX_DIR}/private/*.cxx"
  "${project.SRC_TEST_CXX_DIR}/private/*.cpp")

file(GLOB_RECURSE
  project.SRC_TEST_CXX_PRIVATE_HEADER_SET
  CONFIGURE_DEPENDS
  "${project.SRC_TEST_CXX_DIR}/private/*.h"
  "${project.SRC_TEST_CXX_DIR}/private/*.hxx"
  "${project.SRC_TEST_CXX_DIR}/private/*.hpp"
  "${project.SRC_TEST_CXX_DIR}/private/*.tpp"
  "${project.SRC_TEST_CXX_DIR}/private/*.inl")

set(project.SRC_TEST_CXX_PRIVATE_ENTRYPOINT_SET
  "${project.SRC_TEST_CXX_PRIVATE_IMPL_SET}")
list(FILTER project.SRC_TEST_CXX_PRIVATE_ENTRYPOINT_SET
  INCLUDE REGEX "${project.REGEX_ENTRYPOINT_FILENAME}")
list(FILTER project.SRC_TEST_CXX_PRIVATE_IMPL_SET
  EXCLUDE REGEX "${project.REGEX_ENTRYPOINT_FILENAME}")

add_custom_target(
  ${project.TARGET_BASENAME}_resources_test
  COMMAND "${CMAKE_COMMAND}"
  "-DSRC=${project.SRC_TEST_RESOURCES_DIR}"
  "-DDST=${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/resources"
  "-DMODE=LINK"
  "-P" "${PROJECT_SOURCE_DIR}/cmake/LinkCopyIncremental.process.cmake"
  VERBATIM
  COMMENT "[test](build-time) ${project.TARGET_BASENAME}_resources_test; Update Resource Paths.")

if(${project.TARGET_BASENAME_UP}_PACKAGE)
  install(
    DIRECTORY "${project.SRC_TEST_RESOURCES_DIR}/"
    DESTINATION "resources"
    COMPONENT resources_test
    OPTIONAL
    FILES_MATCHING
    PATTERN "*"
    PATTERN ".env.production" EXCLUDE
    PATTERN ".env.development" EXCLUDE
    PATTERN ".keep" EXCLUDE
    PATTERN ".gitkeep" EXCLUDE
    REGEX "${project.REGEX_ENTRYPOINT_FILENAME}" EXCLUDE)
endif()

add_library(
  ${project.TARGET_BASENAME}_core_private_test INTERFACE)
add_library(
  ${project.GROUP}::${project.TARGET_BASENAME}_core_private_test
  ALIAS ${project.TARGET_BASENAME}_core_private_test)
target_sources(
  ${project.TARGET_BASENAME}_core_private_test
  INTERFACE
  ${project.SRC_TEST_CXX_PRIVATE_IMPL_SET}
  INTERFACE
  FILE_SET HEADERS
  BASE_DIRS "${project.SRC_TEST_CXX_DIR}/private"
  FILES ${project.SRC_TEST_CXX_PRIVATE_HEADER_SET})
target_include_directories(
  ${project.TARGET_BASENAME}_core_private_test
  INTERFACE
  "$<BUILD_INTERFACE:${project.SRC_MAIN_CXX_DIR}/private>")

if(TARGET ${project.TARGET_BASENAME}_resources_test)
  add_dependencies(
    ${project.TARGET_BASENAME}_core_private_test
    ${project.TARGET_BASENAME}_resources_test)
endif()

add_executable(
  ${project.TARGET_BASENAME}_app_test)
set_target_properties(
  ${project.TARGET_BASENAME}_app_test
  PROPERTIES
  OUTPUT_NAME
  "$<LOWER_CASE:${project.TARGET_BASENAME}-${PROJECT_VERSION}-test-$<CONFIG>-${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_PROCESSOR}>"
  CXX_EXTENSIONS OFF
  LINKER_LANGUAGE "CXX"
  POSITION_INDEPENDENT_CODE ON
  RUNTIME_OUTPUT_DIRECTORY "$<CONFIG>/bin")
target_sources(
  ${project.TARGET_BASENAME}_app_test
  PRIVATE
  ${project.SRC_TEST_CXX_PRIVATE_ENTRYPOINT_SET})
target_link_libraries(
  ${project.TARGET_BASENAME}_app_test
  PRIVATE
  ${project.TARGET_BASENAME}_common
  ${project.TARGET_BASENAME}_core_private_test
  ${project.TARGET_BASENAME}_core
  Catch2::Catch2)

catch_discover_tests(
  ${project.TARGET_BASENAME}_app_test
  DISCOVERY_MODE PRE_TEST
  ADD_TAGS_AS_LABELS
  PROPERTIES
  LABELS *
  ENVIRONMENT "${project.TEST_ENV_VARS}"
  DL_PATHS "${project.CXX_COMPILER_DIR}" #[["$ENV{PATH}"]])

if(TARGET ${project.TARGET_BASENAME}_app_test)
  enable_target_sanitizers(
    ${project.TARGET_BASENAME}_app_test ${project.TARGET_BASENAME_UP})
endif()

if(WIN32)
  # REM: Test target post-build deployment.
  # REM: Automatically extracts and copies all non-system precompiled
  # REM: Windows shared library dependencies (PE DLLs) into the test execution folder. (non-system imported precompiled libraries and internal build-from-source precompiled libraries)
  add_custom_command(
    TARGET ${project.TARGET_BASENAME}_app_test
    POST_BUILD

    #[[NOTE: We intentionally pass '$<TARGET_FILE:...>' as the first source argument.
    This ensures 'copy_if_different' has at least one valid file to copy if '$<TARGET_RUNTIME_DLLS>' is empty, preventing a fatal CMake command syntax error when no dynamic dependencies exist.]]
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:${project.TARGET_BASENAME}_app_test>"
    "$<TARGET_RUNTIME_DLLS:${project.TARGET_BASENAME}_app_test>"
    "$<TARGET_FILE_DIR:${project.TARGET_BASENAME}_app_test>"

    COMMAND_EXPAND_LISTS
    VERBATIM
    COMMENT "[test](build-time) ${project.TARGET_BASENAME}_app_test: Extracting and deploying Runtime DLL dependencies.")

  #[[NOTE: Safely bind it into 'clean'; This will remove the
    copied non-system system shared libraries that resides adjacent to
    the binary]]
  # get_target_property(_app_test_bin_dir
  # ${project.TARGET_BASENAME}_app_test BINARY_DIR)
  set_property(
    DIRECTORY "${CMAKE_BINARY_DIR}"
    APPEND PROPERTY

    # REM: Only works with ninja generator
    ADDITIONAL_CLEAN_FILES
    "$<LIST:TRANSFORM,$<PATH:GET_FILENAME,$<TARGET_RUNTIME_DLLS:${project.TARGET_BASENAME}_app_test>>,PREPEND,$<TARGET_FILE_DIR:${project.TARGET_BASENAME}_app_test>/>")
  unset(_app_test_bin_dir)
endif()

if(${project.TARGET_BASENAME_UP}_PACKAGE)
  install(
    TARGETS ${project.TARGET_BASENAME}_app_test
    RUNTIME_DEPENDENCY_SET
    _target_app_test_precompiled_runtime_deps
    RUNTIME DESTINATION "bin"
    COMPONENT app_test)

  install(
    RUNTIME_DEPENDENCY_SET
    _target_app_test_precompiled_runtime_deps
    PRE_EXCLUDE_REGEXES
    ${project.REGEX_SYSTEM_PRECOMPILED_RUNTIME_DEPENDENCIES}
    RUNTIME DESTINATION "bin"
    COMPONENT app_test_runtime_deps
    OPTIONAL
    LIBRARY DESTINATION "lib"
    COMPONENT app_test_runtime_deps
    OPTIONAL)

  if(TARGET ${project.TARGET_BASENAME}_core)
    get_target_property(
      _is_imported ${project.TARGET_BASENAME}_core IMPORTED)

    if(NOT _is_imported)
      install(
        TARGETS ${project.TARGET_BASENAME}_core
        RUNTIME DESTINATION "bin"
        COMPONENT app_test_runtime_deps
        OPTIONAL
        LIBRARY DESTINATION "lib"
        COMPONENT app_test_runtime_deps
        OPTIONAL)
    endif()
  endif()
endif()