#[[
  Sanitizers.cmake - Modern List-Based Multi-Config Compatible Module
  
  Supports: -D<PREFIX>_SAN_MODE=none OR -D<PREFIX>_SAN_MODE=address;undefined OR -D<PREFIX>_SAN_MODE=address OR -D<PREFIX>_SAN_MODE=undefined OR -D<PREFIX>_SAN_MODE=memory OR -D<PREFIX>_SAN_MODE=thread
]]

include_guard(GLOBAL)
include(CheckCXXCompilerFlag)

function(enable_target_sanitizers TARGET_NAME PREFIX)
  if(NOT TARGET ${TARGET_NAME})
    message(FATAL_ERROR
      "enable_target_sanitizers: '${TARGET_NAME}' is not a valid CMake target.")
  endif()

  string(STRIP "${PREFIX}" PREFIX_UP)
  string(TOUPPER "${PREFIX_UP}" PREFIX_UP)

  if(NOT PREFIX OR PREFIX_UP STREQUAL "")
    message(FATAL_ERROR
      "enable_target_sanitizers: '${PREFIX}' is not a valid PREFIX such as; (\${PREFIX}_SAN_MODE)")
  endif()

  # REM: LEAF GUARD: Skips imported targets since precompiled external binaries cannot accept local compiler flag manipulations.
  get_target_property(IS_IMPORTED ${TARGET_NAME} IMPORTED)

  if(IS_IMPORTED)
    message(DEBUG
      "Sanitizers: Skipping imported target '${TARGET_NAME}' (handled by package profile).")
    return()
  endif()

  # REM: LEAF GUARD: Skips interface targets since they lack compiled binary objects and would throw errors if passed to private flags.
  get_target_property(TARGET_TYPE ${TARGET_NAME} TYPE)

  if(TARGET_TYPE STREQUAL "INTERFACE_LIBRARY")
    message(DEBUG
      "Sanitizers: Skipping interface library target '${TARGET_NAME}' (not a binary leaf).")
    return()
  endif()

  set(${PREFIX_UP}_SAN_MODE "none"
    CACHE STRING "List of sanitizers to enable (none, address, undefined, memory, thread)")
  option(${PREFIX_UP}_SAN_FORCE_STATIC
    "Force static linking of sanitizer runtimes" OFF)

  # REM: Convert the entire list to lowercase for robust string matching
  string(TOLOWER "${${PREFIX_UP}_SAN_MODE}" SAN_MODE_LOWER)

  # REM: Clean exit if the mode is explicitly set to none, empty, or false
  if(NOT SAN_MODE_LOWER OR SAN_MODE_LOWER STREQUAL "none" OR SAN_MODE_LOWER STREQUAL "off" OR SAN_MODE_LOWER STREQUAL "false")
    return()
  endif()

  # REM: Track choices to validate mutual exclusivity
  set(HAS_ASAN FALSE)
  set(HAS_UBSAN FALSE)
  set(HAS_TSAN FALSE)
  set(HAS_MSAN FALSE)

  foreach(MODE ${SAN_MODE_LOWER})
    if(MODE STREQUAL "address")
      set(HAS_ASAN TRUE)
    elseif(MODE STREQUAL "undefined")
      set(HAS_UBSAN TRUE)
    elseif(MODE STREQUAL "thread")
      set(HAS_TSAN TRUE)
    elseif(MODE STREQUAL "memory")
      set(HAS_MSAN TRUE)
    elseif(MODE STREQUAL "none")
      message(FATAL_ERROR
        "Sanitizers: 'none' cannot be combined with active sanitizers in list: ${${PREFIX_UP}_SAN_MODE}")
    else()
      message(FATAL_ERROR
        "Sanitizers: Unknown mode '${MODE}'. Choose from: none, address, undefined, memory, thread")
    endif()
  endforeach()

  # REM: Enforce mutual exclusivity
  if((HAS_TSAN AND HAS_ASAN) OR(HAS_MSAN AND HAS_ASAN) OR(HAS_MSAN AND HAS_TSAN))
    message(FATAL_ERROR
      "Sanitizers: address, thread, and memory sanitizers are mutually exclusive.")
  endif()

  set(COMPILER_FLAGS "")
  set(LINKER_FLAGS "")
  set(OPTIMIZED_CONFIGS
    "Release" "RelWithDebInfo" "MinSizeRel")

  # REM: GCC / Clang / AppleClang Flag Resolver
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    list(APPEND
      COMPILER_FLAGS "-fno-omit-frame-pointer" "-g")

    foreach(CONFIG ${OPTIMIZED_CONFIGS})
      list(APPEND
        COMPILER_FLAGS "$<$<CONFIG:${CONFIG}>:-O2>")
    endforeach()

    set(SAN_LIST "")

    if(HAS_ASAN)
      list(APPEND SAN_LIST "address")
    endif()

    if(HAS_TSAN)
      list(APPEND SAN_LIST "thread")
    endif()

    if(HAS_UBSAN)
      list(APPEND SAN_LIST "undefined")
    endif()

    if(HAS_MSAN)
      if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(FATAL_ERROR
          "Sanitizers: MSan is not supported by GCC. Use Clang.")
      endif()

      list(APPEND SAN_LIST "memory")
      list(APPEND COMPILER_FLAGS
        "-fsanitize-memory-track-origins=2")
    endif()

    if(SAN_LIST)
      list(JOIN SAN_LIST "," SAN_STR)
      set(TEST_FLAG "-fsanitize=${SAN_STR}")

      set(CMAKE_REQUIRED_FLAGS
        "${TEST_FLAG} -fno-omit-frame-pointer")
      check_cxx_compiler_flag("${TEST_FLAG}"
        COMPILER_SUPPORTS_SANITIZER)
      unset(CMAKE_REQUIRED_FLAGS)

      if(NOT COMPILER_SUPPORTS_SANITIZER)
        message(FATAL_ERROR
          "Sanitizers: The compiler ${CMAKE_CXX_COMPILER_ID} does not support '-fsanitize=${SAN_STR}'.")
      endif()

      list(APPEND COMPILER_FLAGS "${TEST_FLAG}")
      list(APPEND LINKER_FLAGS "${TEST_FLAG}")
    endif()

    if(HAS_UBSAN)
      list(APPEND COMPILER_FLAGS
        "-fno-sanitize-recover=all")
    endif()

    if(${PREFIX_UP}_SAN_FORCE_STATIC AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      if(HAS_ASAN)
        list(APPEND LINKER_FLAGS "-static-libasan")
      endif()

      if(HAS_UBSAN)
        list(APPEND LINKER_FLAGS "-static-libubsan")
      endif()

      if(HAS_TSAN)
        list(APPEND LINKER_FLAGS "-static-libtsan")
      endif()
    endif()

  # REM: MSVC (Windows Ecosystem)
  elseif(MSVC)
    if(HAS_ASAN)
      set(CMAKE_REQUIRED_FLAGS "/fsanitize=address")
      check_cxx_compiler_flag(
        "/fsanitize=address" MSVC_SUPPORTS_ASAN)
      unset(CMAKE_REQUIRED_FLAGS)

      if(NOT MSVC_SUPPORTS_ASAN)
        message(FATAL_ERROR
          "Sanitizers: This version of MSVC does not support '/fsanitize=address'.")
      endif()

      list(APPEND COMPILER_FLAGS
        "/fsanitize=address" "/Zi")

      # REM: No caching, re-compiled everytime,
      # REM: for address sanitization to work properly.
      list(APPEND LINKER_FLAGS "/INCREMENTAL:NO")

      foreach(CONFIG ${OPTIMIZED_CONFIGS})
        list(APPEND COMPILER_FLAGS
          "$<$<CONFIG:${CONFIG}>:/O2>")
      endforeach()

      set_target_properties(${TARGET_NAME}
        PROPERTIES
        MSVC_RUNTIME_LIBRARY "$<IF:$<CONFIG:Debug,Development>,MultiThreadedDebugDLL,MultiThreadedDLL>")

      # REM: Locate and deploy the MSVC ASan runtime DLL next to executable/shared leaf targets
      # REM: Locate and deploy the MSVC ASan runtime DLL next to executable/shared leaf targets
      if(TARGET_TYPE STREQUAL "EXECUTABLE" OR TARGET_TYPE STREQUAL "SHARED_LIBRARY")
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
          set(_asan_arch "x86_64")
          set(_msvc_target_arch "x64")
        else()
          set(_asan_arch "i386")
          set(_msvc_target_arch "x86")
        endif()

        if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "AMD64|x86_64")
          set(_msvc_host_arch "Hostx64")
        else()
          set(_msvc_host_arch "Hostx86")
        endif()

        set(_asan_dll_filename "clang_rt.asan_dynamic-${_asan_arch}.dll")
        get_filename_component(_compiler_bin_dir "${CMAKE_CXX_COMPILER}" DIRECTORY)

        find_file(MSVC_ASAN_DLL
          NAMES "${_asan_dll_filename}"
          PATHS
          "${_compiler_bin_dir}"
          "${_compiler_bin_dir}/../../../../bin/${_msvc_host_arch}/${_msvc_target_arch}"
          NO_DEFAULT_PATH
          NO_CACHE)

        if(MSVC_ASAN_DLL)
          add_custom_command(
            TARGET ${TARGET_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${MSVC_ASAN_DLL}"
            "$<TARGET_FILE_DIR:${TARGET_NAME}>"
            VERBATIM
            COMMENT "[Sanitizers] Deploying ${MSVC_ASAN_DLL} to $<TARGET_FILE_DIR:${TARGET_NAME}>")

          # get_target_property(_target_bin_dir ${TARGET_NAME} BINARY_DIR)

          # REM: Bind into 'clean'; this will remove asan dll file
          set_property(
            DIRECTORY "${CMAKE_BINARY_DIR}"
            APPEND PROPERTY

            # REM: Only works with ninja generator?
            ADDITIONAL_CLEAN_FILES
            "$<PATH:NORMAL_PATH,$<TARGET_FILE_DIR:${TARGET_NAME}>/${_asan_dll_filename}>")
        else()
          message(WARNING
            "Sanitizers: Could not locate '${_asan_dll_filename}' for automatic deployment.")
        endif()
      endif()
    endif()

    if(HAS_UBSAN OR HAS_TSAN OR HAS_MSAN)
      message(WARNING
        "Sanitizers: MSVC natively only supports 'address' mode. Other options ignored.")
    endif()
  endif()

  if(COMPILER_FLAGS)
    target_compile_options(${TARGET_NAME}
      PRIVATE ${COMPILER_FLAGS})
  endif()

  if(LINKER_FLAGS)
    target_link_options(${TARGET_NAME}
      PRIVATE ${LINKER_FLAGS})
  endif()
endfunction()
