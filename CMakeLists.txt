cmake_minimum_required(VERSION 3.25...3.30)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/LoadThatEnv.cmake")

load_that_env("${CMAKE_CURRENT_SOURCE_DIR}/.env.root")

project("${env.project.NAME}"
  VERSION "${env.project.VERSION}"
  DESCRIPTION ""
  HOMEPAGE_URL ""
  LANGUAGES "CXX")

if(NOT CMAKE_BUILD_TYPE
  AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Debug"
    CACHE STRING "Build Type" FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(project.GROUP "${env.project.GROUP}")

set(project.TARGET_BASENAME "${PROJECT_NAME}")
string(STRIP "${project.TARGET_BASENAME}"
  project.TARGET_BASENAME)
string(TOLOWER "${project.TARGET_BASENAME}"
  project.TARGET_BASENAME)
string(REGEX REPLACE "[ -]+" "_"
  project.TARGET_BASENAME "${project.TARGET_BASENAME}")

string(TOUPPER "${project.TARGET_BASENAME}" 
  project.TARGET_BASENAME_UP)

set(project.CXX_STD "cxx_std_17")
set(project.REGEX_ENTRYPOINT_FILENAME 
  ".*/main\\.(cc|cxx|cpp)$")

if(WIN32)
  set(project.REGEX_SYSTEM_PRECOMPILED_RUNTIME_DEPENDENCIES
    "^ext-ms-.*\\.dll"
    "^[Kk][Ee][Rr][Nn][Ee][Ll]32\\.dll"
    "^[Aa][Pp][Ii]-[Mm][Ss]-.*\\.dll"
    "^[Mm][Ss][Vv][Cc][Pp]140.*\\.dll"
    "^[Vv][Cc][Oo][Mm][Pp]140.*\\.dll"
    "^[Vv][Cc][Rr][Uu][Nn][Tt][Ii][Mm][Ee]140.*\\.dll"
    "^[Uu][Cc][Rr][Tt][Bb][Aa][Ss][Ee][Dd].*\\.dll")
else()
  if(APPLE)
    set(project.RPATH "@loader_path")
  else()
    set(project.RPATH "\\$ORIGIN")
  endif()

  set(project.REGEX_SYSTEM_PRECOMPILED_RUNTIME_DEPENDENCIES
    "^libgcc_s"
    "^libc\\.so"
    "^libstdc\\+\\+"
    "^libm\\.so")
endif()

set(project.SRC_MAIN_CXX_DIR
  "${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp")
set(project.SRC_TEST_CXX_DIR
  "${CMAKE_CURRENT_SOURCE_DIR}/src/test/cpp")

set(project.SRC_MAIN_RESOURCES_DIR
  "${CMAKE_CURRENT_SOURCE_DIR}/src/main/resources")
set(project.SRC_TEST_RESOURCES_DIR
  "${CMAKE_CURRENT_SOURCE_DIR}/src/main/resources")

option(${project.TARGET_BASENAME_UP}_BUILD_APP
  "Build App" ON)

option(${project.TARGET_BASENAME_UP}_BUILD_HHO
  "Build Hybrid Header Only" OFF)

option(${project.TARGET_BASENAME_UP}_BUILD_SHARED
  "Build Shared Library" OFF)

option(${project.TARGET_BASENAME_UP}_BUILD_WAE
  "Build Warning As Error" OFF)

option(${project.TARGET_BASENAME_UP}_BUILD_TESTING
  "Build Testing" ON)

option(${project.TARGET_BASENAME_UP}_PACKAGE
  "Package" ON)

option(${project.TARGET_BASENAME_UP}_FETCH_CONTENT
  "Fetch Content for external libraries" OFF)

add_subdirectory("src/main/cpp")

if(${project.TARGET_BASENAME_UP}_BUILD_TESTING)
  enable_testing()
  include(CTest)
  add_subdirectory("src/test/cpp")
endif()

if(${project.TARGET_BASENAME_UP}_PACKAGE)
  
  set(CPACK_VERBATIM_VARIABLES ON)

  set(CPACK_PACKAGE_VENDOR "${project.GROUP}")
  set(CPACK_PACKAGE_NAME "${project.TARGET_BASENAME}")
  set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
  set(CPACK_PACKAGE_HOMEPAGE_URL "${PROJECT_HOMEPAGE_URL}")

  # set(CPACK_RESOURCE_FILE_LICENSE
  #   "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")

  #REM: Final output name in user space
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")

  set(CPACK_GENERATOR "ZIP;TGZ")
  #REM: Enable filtering "Separation" for ARCHIVE; `-DCPACK_COMPONENTS_ALL="..."`
  set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
  #REM: Override the "Seperation" into "Monolith", however,
  #REM: the filtering `-DCPACK_COMPONENTS_ALL="..."` still active,
  #REM: assuming `CPACK_COMPONENT_INSTALL ON`
  set(CPACK_COMPONENTS_GROUPING "ALL_COMPONENTS_IN_ONE")

  if(WIN32)
    list(APPEND CPACK_GENERATOR "NSIS;WIX")
    set(CPACK_NSIS_COMPONENT_INSTALL ON)
    set(CPACK_WIX_COMPONENT_INSTALL ON)
  elseif(UNIX AND NOT APPLE)
    list(APPEND CPACK_GENERATOR "DEB;RPM")
    set(CPACK_DEB_COMPONENT_INSTALL ON)
    set(CPACK_RPM_COMPONENT_INSTALL ON)
  elseif(APPLE)
    list(APPEND CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_COMPONENT_INSTALL ON)
  else()
    message(WARNING "CPack, unknown OS")
  endif()
  
  include(CPack)

endif()