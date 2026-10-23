file(GLOB_RECURSE
  project.SRC_MAIN_CXX_PRIVATE_IMPL_SET
  CONFIGURE_DEPENDS
  "${project.SRC_MAIN_CXX_DIR}/private/*.cc"
  "${project.SRC_MAIN_CXX_DIR}/private/*.cxx"
  "${project.SRC_MAIN_CXX_DIR}/private/*.cpp")

file(GLOB_RECURSE
  project.SRC_MAIN_CXX_PRIVATE_HEADER_SET
  CONFIGURE_DEPENDS
  "${project.SRC_MAIN_CXX_DIR}/private/*.h"
  "${project.SRC_MAIN_CXX_DIR}/private/*.hxx"
  "${project.SRC_MAIN_CXX_DIR}/private/*.hpp"
  "${project.SRC_MAIN_CXX_DIR}/private/*.tpp"
  "${project.SRC_MAIN_CXX_DIR}/private/*.inl")

file(GLOB_RECURSE
  project.SRC_MAIN_CXX_PUBLIC_HEADER_SET
  CONFIGURE_DEPENDS
  "${project.SRC_MAIN_CXX_DIR}/public/*.h"
  "${project.SRC_MAIN_CXX_DIR}/public/*.hxx"
  "${project.SRC_MAIN_CXX_DIR}/public/*.hpp"
  "${project.SRC_MAIN_CXX_DIR}/public/*.tpp"
  "${project.SRC_MAIN_CXX_DIR}/public/*.inl")

set(
  project.SRC_MAIN_CXX_PRIVATE_ENTRYPOINT_SET
  "${project.SRC_MAIN_CXX_PRIVATE_IMPL_SET}")
list(
  FILTER project.SRC_MAIN_CXX_PRIVATE_ENTRYPOINT_SET
  INCLUDE REGEX "${project.REGEX_ENTRYPOINT_FILENAME}")
list(
  FILTER project.SRC_MAIN_CXX_PRIVATE_IMPL_SET
  EXCLUDE REGEX "${project.REGEX_ENTRYPOINT_FILENAME}")

# REM: [x]; resources UTILITY
add_custom_target(
  ${project.TARGET_BASENAME}_resources
  COMMAND "${CMAKE_COMMAND}"
  "-DSRC=${project.SRC_MAIN_RESOURCES_DIR}"
  "-DDST=${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/resources"
  "-DMODE=LINK"
  "-DFILES_MATCHING=REGEX_EXCLUDE:\\..*keep$"
  "-P" "${PROJECT_SOURCE_DIR}/cmake/LinkCopyIncremental.i.process.cmake"
  VERBATIM
  COMMENT "[main](build-time) ${project.TARGET_BASENAME}_resources; Update Resource Paths")

if(${project.TARGET_BASENAME_UP}_PACKAGE)
  install(
    DIRECTORY "${project.SRC_MAIN_RESOURCES_DIR}/"
    DESTINATION "resources"
    COMPONENT resources
    OPTIONAL
    FILES_MATCHING
    PATTERN "*"
    PATTERN ".env.example"
    PATTERN ".env*" EXCLUDE
    PATTERN ".keep" EXCLUDE
    PATTERN ".gitkeep" EXCLUDE)
endif()

# REM: [x]; common INTERFACE
add_library(
  ${project.TARGET_BASENAME}_common INTERFACE)
add_library(
  ${project.GROUP}::${project.TARGET_BASENAME}_common
  ALIAS ${project.TARGET_BASENAME}_common)
target_compile_features(
  ${project.TARGET_BASENAME}_common
  INTERFACE "${project.CXX_STD}")
target_compile_options(
  ${project.TARGET_BASENAME}_common
  INTERFACE
  $<$<CXX_COMPILER_ID:MSVC>:
  /W4;/permissive-;/utf-8;
  $<$<BOOL:${${project.TARGET_BASENAME_UP}_BUILD_WAE}>:/WX>>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
  -Wall;-Wpedantic;-Wshadow;-Wnon-virtual-dtor;
  $<$<BOOL:${${project.TARGET_BASENAME_UP}_BUILD_WAE}>:-Werror>>)

if(${project.TARGET_BASENAME_UP}_BUILD_TESTING)
  target_compile_definitions(
    ${project.TARGET_BASENAME}_common
    INTERFACE
    ${project.TARGET_BASENAME_UP}_BUILD_TESTING)
endif()

# REM: [x]; core_private INTERFACE
add_library(
  ${project.TARGET_BASENAME}_core_private INTERFACE)
add_library(
  ${project.GROUP}::${project.TARGET_BASENAME}_core_private
  ALIAS ${project.TARGET_BASENAME}_core_private)

if(NOT ${project.TARGET_BASENAME_UP}_BUILD_HHO)
  target_sources(
    ${project.TARGET_BASENAME}_core_private
    INTERFACE
    ${project.SRC_MAIN_CXX_PRIVATE_IMPL_SET})
endif()

target_sources(
  ${project.TARGET_BASENAME}_core_private
  INTERFACE
  FILE_SET HEADERS
  BASE_DIRS "${project.SRC_MAIN_CXX_DIR}/private"
  FILES ${project.SRC_MAIN_CXX_PRIVATE_HEADER_SET})

# REM: [x]; core_public INTERFACE
add_library(
  ${project.TARGET_BASENAME}_core_public INTERFACE)
add_library(
  ${project.GROUP}::${project.TARGET_BASENAME}_core_public
  ALIAS ${project.TARGET_BASENAME}_core_public)
target_sources(
  ${project.TARGET_BASENAME}_core_public
  INTERFACE
  FILE_SET HEADERS
  BASE_DIRS "${project.SRC_MAIN_CXX_DIR}/public"
  FILES ${project.SRC_MAIN_CXX_PUBLIC_HEADER_SET})

# REM: [x]; core ((HHO) INTERFACE) | SHARED_LIBRARY | STATIC_LIBRARY, install
if(${project.TARGET_BASENAME_UP}_BUILD_HHO)
  if(${project.TARGET_BASENAME_UP}_BUILD_SHARED)
    message(WARNING
      "Both HHO and SHARED are enable, however, HHO takes precedence.")
  endif()

  add_library(
    ${project.TARGET_BASENAME}_core INTERFACE)
  target_compile_definitions(
    ${project.TARGET_BASENAME}_core
    INTERFACE
    ${project.TARGET_BASENAME_UP}_BUILD_HHO)
  target_link_libraries(
    ${project.TARGET_BASENAME}_core
    INTERFACE
    $<BUILD_INTERFACE:${project.TARGET_BASENAME}_common>
    ${project.TARGET_BASENAME}_core_private
    ${project.TARGET_BASENAME}_core_public)

  if(${project.TARGET_BASENAME_UP}_PACKAGE)
    install(
      TARGETS ${project.TARGET_BASENAME}_core_public
      EXPORT ${project.TARGET_BASENAME}_targets
      FILE_SET HEADERS DESTINATION "include"
      COMPONENT core)
    install(
      TARGETS ${project.TARGET_BASENAME}_core_private
      EXPORT ${project.TARGET_BASENAME}_targets
      FILE_SET HEADERS DESTINATION "include"
      COMPONENT core)
    install(
      DIRECTORY "${project.SRC_MAIN_CXX_DIR}/private/"
      DESTINATION "include"
      COMPONENT core
      OPTIONAL
      FILES_MATCHING
      PATTERN "*.cc"
      PATTERN "*.cxx"
      PATTERN "*.cpp"
      REGEX "${project.REGEX_ENTRYPOINT_FILENAME}" EXCLUDE)
    install(
      TARGETS ${project.TARGET_BASENAME}_core
      RUNTIME_DEPENDENCY_SET
      _target_core_precompiled_runtime_deps
      EXPORT ${project.TARGET_BASENAME}_targets
      FILE_SET HEADERS DESTINATION "include"
      COMPONENT core)
    install(
      RUNTIME_DEPENDENCY_SET
      _target_core_precompiled_runtime_deps
      PRE_EXCLUDE_REGEXES
      ${project.REGEX_SYSTEM_PRECOMPILED_RUNTIME_DEPENDENCIES}
      RUNTIME DESTINATION "bin"
      COMPONENT core
      OPTIONAL
      LIBRARY DESTINATION "lib"
      COMPONENT core
      OPTIONAL)
  endif()

else()
  if(${project.TARGET_BASENAME_UP}_BUILD_SHARED)
    add_library(
      ${project.TARGET_BASENAME}_core SHARED)
    set_target_properties(
      ${project.TARGET_BASENAME}_core
      PROPERTIES
      OUTPUT_NAME
      "$<LOWER_CASE:${project.TARGET_BASENAME}>"
      SOVERSION "${PROJECT_VERSION_MAJOR}"
      DEBUG_POSTFIX "d"
      INSTALL_RPATH
      "${project.RPATH};${project.RPATH}/../lib"
      BUILD_WITH_INSTALL_RPATH OFF)

    if(WIN32)
      # REM: Main: Build-time, core-shared; (PE) Windows SHARED libs deps
      # REM: Non-system, non-legacy precompiled libraries
      add_custom_command(
        TARGET ${project.TARGET_BASENAME}_core
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:${project.TARGET_BASENAME}_core>"
        "$<TARGET_RUNTIME_DLLS:${project.TARGET_BASENAME}_core>"
        "$<TARGET_FILE_DIR:${project.TARGET_BASENAME}_core>"
        COMMAND_EXPAND_LISTS
        VERBATIM
        COMMENT "[main](build-time) ${project.TARGET_BASENAME}_core: Extracting and deploying Runtime DLL dependencies.")
    endif()
  else()
    add_library(
      ${project.TARGET_BASENAME}_core STATIC)
    set_target_properties(
      ${project.TARGET_BASENAME}_core
      PROPERTIES
      OUTPUT_NAME
      "$<LOWER_CASE:${project.TARGET_BASENAME}-${PROJECT_VERSION}-$<CONFIG>-${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_PROCESSOR}>")
  endif()

  set_target_properties(
    ${project.TARGET_BASENAME}_core
    PROPERTIES
    CXX_EXTENSIONS OFF
    LINKER_LANGUAGE "CXX"
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET "hidden"
    VISIBILITY_INLINES_HIDDEN ON

    RUNTIME_OUTPUT_DIRECTORY "$<CONFIG>/bin"
    LIBRARY_OUTPUT_DIRECTORY "$<CONFIG>/lib"
    ARCHIVE_OUTPUT_DIRECTORY "$<CONFIG>/lib")

  if(${project.TARGET_BASENAME_UP}_BUILD_SHARED)
    target_compile_definitions(
      ${project.TARGET_BASENAME}_core
      PRIVATE
      ${project.TARGET_BASENAME_UP}_BUILD_SHARED
      INTERFACE
      ${project.TARGET_BASENAME_UP}_CONSUME_SHARED)
  else()
    target_compile_definitions(
      ${project.TARGET_BASENAME}_core
      PUBLIC
      ${project.TARGET_BASENAME_UP}_BUILD_STATIC)
  endif()

  target_link_libraries(
    ${project.TARGET_BASENAME}_core
    PRIVATE
    $<BUILD_INTERFACE:${project.TARGET_BASENAME}_common>
    $<BUILD_INTERFACE:${project.TARGET_BASENAME}_core_private>
    PUBLIC
    ${project.TARGET_BASENAME}_core_public)

  if(${project.TARGET_BASENAME_UP}_PACKAGE)
    install(
      TARGETS ${project.TARGET_BASENAME}_core_public
      EXPORT ${project.TARGET_BASENAME}_targets
      FILE_SET HEADERS DESTINATION "include"
      COMPONENT core)
    install(
      TARGETS ${project.TARGET_BASENAME}_core
      RUNTIME_DEPENDENCY_SET
      _target_core_precompiled_runtime_deps
      EXPORT ${project.TARGET_BASENAME}_targets
      FILE_SET HEADERS DESTINATION "include"
      COMPONENT core
      RUNTIME DESTINATION "bin"
      COMPONENT core
      LIBRARY DESTINATION "lib"
      COMPONENT core
      ARCHIVE DESTINATION "lib"
      COMPONENT core)
    install(
      RUNTIME_DEPENDENCY_SET _target_core_precompiled_runtime_deps
      PRE_EXCLUDE_REGEXES
      ${project.REGEX_SYSTEM_PRECOMPILED_RUNTIME_DEPENDENCIES}
      RUNTIME DESTINATION "bin"
      COMPONENT core_runtime_deps
      OPTIONAL
      LIBRARY DESTINATION "lib"
      COMPONENT core_runtime_deps
      OPTIONAL)
  endif()
endif()

target_compile_definitions(
  ${project.TARGET_BASENAME}_core
  PRIVATE
  _CRT_SECURE_NO_WARNINGS)

if(TARGET ${project.TARGET_BASENAME}_resources)
  add_dependencies(
    ${project.TARGET_BASENAME}_core
    ${project.TARGET_BASENAME}_resources)
endif()

if(${project.TARGET_BASENAME_UP}_PACKAGE)
  install(
    EXPORT ${project.TARGET_BASENAME}_targets
    NAMESPACE ${project.GROUP}::
    FILE ${project.TARGET_BASENAME}_targets.cmake
    DESTINATION "lib/cmake/${project.TARGET_BASENAME}"
    COMPONENT core)
endif()

add_library(
  ${project.GROUP}::${project.TARGET_BASENAME}_core
  ALIAS ${project.TARGET_BASENAME}_core)

# REM: [x]; app, install
if(${project.TARGET_BASENAME_UP}_BUILD_APP)
  add_executable(${project.TARGET_BASENAME}_app)

  set_target_properties(
    ${project.TARGET_BASENAME}_app
    PROPERTIES
    OUTPUT_NAME
    "$<LOWER_CASE:${project.TARGET_BASENAME}-${PROJECT_VERSION}-$<CONFIG>-${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_PROCESSOR}>"
    CXX_EXTENSIONS OFF
    LINKER_LANGUAGE "CXX"
    POSITION_INDEPENDENT_CODE ON
    RUNTIME_OUTPUT_DIRECTORY "$<CONFIG>/bin")

  target_sources(
    ${project.TARGET_BASENAME}_app
    PRIVATE
    ${project.SRC_MAIN_CXX_PRIVATE_ENTRYPOINT_SET})

  target_link_libraries(
    ${project.TARGET_BASENAME}_app
    PRIVATE
    ${project.TARGET_BASENAME}_common
    ${project.TARGET_BASENAME}_core)

  if(WIN32)
    # REM: Main: Build-time, app-executable; (PE) Windows SHARED libs deps
    # REM: Non-system, non-legacy precompiled libraries
    add_custom_command(
      TARGET ${project.TARGET_BASENAME}_app
      POST_BUILD

      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "$<TARGET_FILE:${project.TARGET_BASENAME}_app>"
      "$<TARGET_RUNTIME_DLLS:${project.TARGET_BASENAME}_app>"
      "$<TARGET_FILE_DIR:${project.TARGET_BASENAME}_app>"

      COMMAND_EXPAND_LISTS
      VERBATIM
      COMMENT "[main](build-time) ${project.TARGET_BASENAME}_app: Extracting and deploying Runtime DLL dependencies.")
  endif()

  if(${project.TARGET_BASENAME_UP}_PACKAGE)
    install(
      TARGETS ${project.TARGET_BASENAME}_app
      RUNTIME_DEPENDENCY_SET
      _target_app_precompiled_runtime_deps
      RUNTIME DESTINATION "bin"
      COMPONENT app)

    install(
      RUNTIME_DEPENDENCY_SET _target_app_precompiled_runtime_deps
      PRE_EXCLUDE_REGEXES
      ${project.REGEX_SYSTEM_PRECOMPILED_RUNTIME_DEPENDENCIES}

      RUNTIME DESTINATION "bin"
      COMPONENT app_runtime_deps
      OPTIONAL

      LIBRARY DESTINATION "lib"
      COMPONENT app_runtime_deps
      OPTIONAL)

    if(TARGET ${project.TARGET_BASENAME}_core)
      get_target_property(
        _is_imported ${project.TARGET_BASENAME}_core IMPORTED)

      if(NOT _is_imported)
        install(
          TARGETS ${project.TARGET_BASENAME}_core

          RUNTIME
          DESTINATION "bin"
          COMPONENT app_runtime_deps
          OPTIONAL

          LIBRARY
          DESTINATION "lib"
          COMPONENT app_runtime_deps
          OPTIONAL)
      endif()
    endif()
  endif()
endif()