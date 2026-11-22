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
    # Catch2 ${project.TARGET_BASENAME_UP})
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
    # Catch2WithMain ${project.TARGET_BASENAME_UP})
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
  COMMENT "[${project.TARGET_BASENAME}_resources_test](build) Update Resource Paths.")

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
  RUNTIME_OUTPUT_DIRECTORY "$<CONFIG>/bin"
  ARCHIVE_OUTPUT_DIRECTORY "$<CONFIG>/lib"
  LIBRARY_OUTPUT_DIRECTORY "$<CONFIG>/lib"
  BUILD_RPATH
  "${project.RPATH};${project.RPATH}/../lib"
  INSTALL_RPATH
  "${project.RPATH};${project.RPATH}/../lib"
  BUILD_WITH_INSTALL_RPATH OFF)
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

set(project.COMMON_CATCH_DISCOVER_ARGS

  # EXTRA_ARGS "--success"
  DISCOVERY_MODE PRE_TEST
  PROPERTIES
  DL_PATHS "${project.CXX_COMPILER_DIR}")

# REM: Discovers all visible tests
catch_discover_tests(
  ${project.TARGET_BASENAME}_app_test
  ADD_TAGS_AS_LABELS
  ${project.COMMON_CATCH_DISCOVER_ARGS})

# REM: Explicitly discovers hidden unit tests
catch_discover_tests(
  ${project.TARGET_BASENAME}_app_test
  TEST_SPEC "[.unit]"
  TEST_SUFFIX " [hidden_unit]"
  ${project.COMMON_CATCH_DISCOVER_ARGS}
  PROPERTIES
  LABELS ".unit")

# REM: Explicitly discovers hidden integration tests
catch_discover_tests(
  ${project.TARGET_BASENAME}_app_test
  TEST_SPEC "[.integration]"
  TEST_SUFFIX " [hidden_integration]"
  ${project.COMMON_CATCH_DISCOVER_ARGS}
  PROPERTIES
  LABELS ".integration")

if(TARGET ${project.TARGET_BASENAME}_app_test)
  enable_target_sanitizers(
    ${project.TARGET_BASENAME}_app_test ${project.TARGET_BASENAME_UP})
endif()

if(WIN32)
  set(_target_app_test
    "${project.TARGET_BASENAME}_app_test")
  set(_app_test_runtime_dll_filepaths
    "$<TARGET_RUNTIME_DLLS:${_target_app_test}>")
  set(_app_test_runtime_dll_filenames
    "$<PATH:GET_FILENAME,${_app_test_runtime_dll_filepaths}>")
  set(_app_test_clean_runtime_dlls
    "$<LIST:TRANSFORM,${_app_test_runtime_dll_filenames},PREPEND,$<TARGET_FILE_DIR:${_target_app_test}>/>")

  add_custom_command(
    TARGET ${_target_app_test} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:${_target_app_test}>"
    "${_app_test_runtime_dll_filepaths}"
    "$<TARGET_FILE_DIR:${_target_app_test}>"
    COMMAND_EXPAND_LISTS
    VERBATIM
    COMMENT "[${_target_app_test}](build) Extracting and deploying Runtime DLL dependencies."
  )

  if(CMAKE_GENERATOR MATCHES "Ninja|Makefiles")
    # REM: These generators honor ADDITIONAL_CLEAN_FILES and will delete
    # REM: the listed files when the global `clean` target runs.
    if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.20")
      get_target_property(_tgt_src_dir ${_target_app_test} SOURCE_DIR)
      set_property(DIRECTORY "${_tgt_src_dir}" APPEND PROPERTY
        ADDITIONAL_CLEAN_FILES
        "$<$<BOOL:${_app_test_runtime_dll_filepaths}>:${_app_test_clean_runtime_dlls}>")
      unset(_tgt_src_dir)
    else()
      message(WARNING
        "[${_target_app_test}](runtime-dlls): CMake ${CMAKE_VERSION} < 3.20; "
        "skipping clean registration for runtime DLLs (copy still works).")
    endif()

  else()
    # REM: Visual Studio / Xcode do not expose a scriptable `clean`
    # REM: target. Provide an explicit per-target clean target.
    set(_clean_tgt "_runtime_dll_clean_${_target_app_test}")

    if(NOT TARGET ${_clean_tgt})
      add_custom_target(${_clean_tgt}
        COMMAND ${CMAKE_COMMAND} -E rm -f
        "$<$<BOOL:${_app_test_runtime_dll_filepaths}>:${_app_test_clean_runtime_dlls}>"
        COMMAND_EXPAND_LISTS
        COMMENT "[${_target_app_test}](clean) removing deployed runtime DLLs"
        VERBATIM)

      message(STATUS
        "[${_target_app_test}](runtime-dlls): generator '${CMAKE_GENERATOR}' "
        "does not support automatic clean registration; run "
        "`cmake --build . --target ${_clean_tgt}` to remove the deployed DLLs.")
    endif()

    unset(_clean_tgt)
  endif()

  unset(_app_test_clean_runtime_dlls)
  unset(_app_test_runtime_dll_filenames)
  unset(_app_test_runtime_dll_filepaths)
  unset(_target_app_test)
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

    unset(_is_imported)
  endif()
endif()