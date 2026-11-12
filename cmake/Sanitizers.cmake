# Sanitizers.cmake
#
# Per-target Sanitizer wiring for GCC, Clang, AppleClang, MSVC and clang-cl.
#
# Cache variables (all PREFIX-scoped):
#   -D<PREFIX>_SAN_MODE         = none | address | undefined | memory | thread
#                                 (address/undefined may be combined)
#   -D<PREFIX>_SAN_FORCE_STATIC = ON|OFF   force -static-libasan/-static-libubsan
#   -D<PREFIX>_SAN_RECOVER      = ON|OFF   continue after a report
#
# Mutually exclusive: address, thread, memory.
# ------------------------------------------------------------------------------

include_guard(GLOBAL)
include(CheckCXXCompilerFlag)

function(enable_target_sanitizers TARGET_NAME PREFIX)
  if(NOT TARGET ${TARGET_NAME})
    message(FATAL_ERROR
      "enable_target_sanitizers: '${TARGET_NAME}' is not a valid CMake target.")
  endif()

  string(STRIP "${PREFIX}" PREFIX_UP)
  string(TOUPPER "${PREFIX_UP}" PREFIX_UP)
  if(PREFIX_UP STREQUAL "")
    message(FATAL_ERROR
      "enable_target_sanitizers: '${PREFIX}' is not a valid PREFIX such as (\${PREFIX}_SAN_MODE)")
  endif()

  # -- Skip non-linking targets -----------------------------------------------
  get_target_property(_imported ${TARGET_NAME} IMPORTED)
  if(_imported)
    message(DEBUG "Sanitizers: skipping imported target '${TARGET_NAME}'")
    return()
  endif()

  get_target_property(_type ${TARGET_NAME} TYPE)
  if(_type STREQUAL "INTERFACE_LIBRARY")
    message(DEBUG "Sanitizers: skipping interface library '${TARGET_NAME}'")
    return()
  endif()

  # -- Options ---------------------------------------------------------------
  set(${PREFIX_UP}_SAN_MODE "none" CACHE STRING
      "List of sanitizers to enable: none, address, undefined, memory, thread")
  option(${PREFIX_UP}_SAN_FORCE_STATIC
         "Force static sanitizer runtime linkage (GCC)" OFF)
  option(${PREFIX_UP}_SAN_RECOVER
         "Continue after sanitizer errors (runtime-recoverable)" OFF)

  string(TOLOWER "${${PREFIX_UP}_SAN_MODE}" _mode)

  # -- Resolve enabled state WITHOUT inverting the macro ---------------------
  set(_enabled 1)
  if(_mode STREQUAL "" OR _mode STREQUAL "none" OR
     _mode STREQUAL "off" OR _mode STREQUAL "false")
    set(_enabled 0)
  endif()

  if(${PREFIX_UP}_SAN_RECOVER)
    set(_recover_def 1)
  else()
    set(_recover_def 0)
  endif()

  target_compile_definitions(${TARGET_NAME} PRIVATE
    OBJECT_ORIENTED_PROGRAMMING_SAN_MODE=${_enabled}
    OBJECT_ORIENTED_PROGRAMMING_SAN_RECOVER=${_recover_def})

  if(NOT _enabled)
    message(DEBUG "Sanitizers: disabled on target '${TARGET_NAME}'")
    return()
  endif()

  # -- Parse requested sanitizer list ----------------------------------------
  set(HAS_ASAN  FALSE)
  set(HAS_UBSAN FALSE)
  set(HAS_TSAN  FALSE)
  set(HAS_MSAN  FALSE)

  foreach(MODE ${_mode})
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
        "Sanitizers: 'none' cannot be combined with active sanitizers "
        "in list: '${${PREFIX_UP}_SAN_MODE}'")
    else()
      message(FATAL_ERROR
        "Sanitizers: unknown mode '${MODE}'. Choose from: "
        "none, address, undefined, memory, thread")
    endif()
  endforeach()

  if((HAS_TSAN AND HAS_ASAN) OR (HAS_MSAN AND HAS_ASAN) OR (HAS_MSAN AND HAS_TSAN))
    message(FATAL_ERROR
      "Sanitizers: address, thread and memory sanitizers are mutually exclusive.")
  endif()

  set(_compile "")
  set(_link    "")

  # ---------------------------------------------------------------------------
  # GCC / Clang / AppleClang (incl. clang-cl)
  # ---------------------------------------------------------------------------
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    list(APPEND _compile "-fno-omit-frame-pointer" "-g")

    set(_san_list "")
    if(HAS_ASAN)
      list(APPEND _san_list "address")
    endif()
    if(HAS_TSAN)
      list(APPEND _san_list "thread")
    endif()
    if(HAS_UBSAN)
      list(APPEND _san_list "undefined")
    endif()
    if(HAS_MSAN)
      if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(FATAL_ERROR "Sanitizers: MSan is not supported by GCC; use Clang.")
      endif()
      list(APPEND _san_list "memory")
      list(APPEND _compile "-fsanitize-memory-track-origins=2")
    endif()

    if(_san_list)
      list(JOIN _san_list "," _san_str)
      set(_test_flag "-fsanitize=${_san_str}")

      string(MAKE_C_IDENTIFIER "${_test_flag}" _flag_id)
      set(_check_var "COMPILER_SUPPORTS_${PREFIX_UP}_SAN_${_flag_id}")

      set(CMAKE_REQUIRED_FLAGS       "${_test_flag} -fno-omit-frame-pointer")
      set(CMAKE_REQUIRED_LINK_OPTIONS "${_test_flag}")
      check_cxx_compiler_flag("${_test_flag}" ${_check_var})
      unset(CMAKE_REQUIRED_FLAGS)
      unset(CMAKE_REQUIRED_LINK_OPTIONS)

      if(NOT ${_check_var})
        message(FATAL_ERROR
          "Sanitizers: ${CMAKE_CXX_COMPILER_ID} does not support '${_test_flag}'.")
      endif()

      list(APPEND _compile "${_test_flag}")
      list(APPEND _link    "${_test_flag}")
    endif()

    # Recover / halt behavior
    if(${PREFIX_UP}_SAN_RECOVER)
      if(HAS_ASAN)
        list(APPEND _compile "-fsanitize-recover=address")
      endif()
      if(HAS_UBSAN)
        list(APPEND _compile "-fsanitize-recover=undefined")
      endif()
    else()
      if(HAS_UBSAN)
        list(APPEND _compile "-fno-sanitize-recover=all")
      endif()
    endif()

    # Optional static runtime linkage (GCC only by design)
    if(${PREFIX_UP}_SAN_FORCE_STATIC AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      if(HAS_ASAN)
        list(APPEND _link "-static-libasan")
      endif()
      if(HAS_UBSAN)
        list(APPEND _link "-static-libubsan")
      endif()
      if(HAS_TSAN)
        list(APPEND _link "-static-libtsan")
      endif()
    endif()

  # ---------------------------------------------------------------------------
  # MSVC (cl.exe) -- ASan only
  # ---------------------------------------------------------------------------
  elseif(MSVC)
    if(HAS_ASAN)
      set(_check_var "MSVC_SUPPORTS_${PREFIX_UP}_SAN_ASAN")
      set(CMAKE_REQUIRED_FLAGS "/fsanitize=address")
      check_cxx_compiler_flag("/fsanitize=address" ${_check_var})
      unset(CMAKE_REQUIRED_FLAGS)

      if(NOT ${_check_var})
        message(FATAL_ERROR
          "Sanitizers: this MSVC does not support '/fsanitize=address'.")
      endif()

      list(APPEND _compile "/fsanitize=address" "/Zi")
      list(APPEND _link    "/INCREMENTAL:NO")

      # Prevent LNK2038 (annotate_string / annotate_vector) against non-ASan libs.
      set(_check_var "MSVC_SUPPORTS_${PREFIX_UP}_SAN_NOVCASAN")
      set(CMAKE_REQUIRED_FLAGS "/fsanitize=address /fno-sanitize-address-vcasan-lib")
      check_cxx_compiler_flag("/fno-sanitize-address-vcasan-lib" ${_check_var})
      unset(CMAKE_REQUIRED_FLAGS)

      if(${_check_var})
        list(APPEND _compile "/fno-sanitize-address-vcasan-lib")
      # else()
        target_compile_definitions(${TARGET_NAME} PRIVATE
          _DISABLE_STRING_ANNOTATION
          _DISABLE_VECTOR_ANNOTATION)
      endif()

      if(${PREFIX_UP}_SAN_RECOVER)
        set(_check_var "MSVC_SUPPORTS_${PREFIX_UP}_SAN_RECOVER")
        check_cxx_compiler_flag("/fsanitize-recover=address" ${_check_var})
        if(${_check_var})
          list(APPEND _compile "/fsanitize-recover=address")
        else()
          message(WARNING
            "Sanitizers: this MSVC does not support '/fsanitize-recover=address'.")
        endif()
      endif()

      # Deploy the ASan runtime DLL next to executables / shared libs.
      if(_type STREQUAL "EXECUTABLE" OR _type STREQUAL "SHARED_LIBRARY")
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

        set(_asan_dll "clang_rt.asan_dynamic-${_asan_arch}.dll")
        get_filename_component(_compiler_bin "${CMAKE_CXX_COMPILER}" DIRECTORY)

        find_file(_asan_dll_path
          NAMES "${_asan_dll}"
          PATHS
            "${_compiler_bin}"
            "${_compiler_bin}/../../../../bin/${_msvc_host_arch}/${_msvc_target_arch}"
          NO_DEFAULT_PATH
          NO_CACHE)

        if(_asan_dll_path)
          add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
              "${_asan_dll_path}"
              "$<TARGET_FILE_DIR:${TARGET_NAME}>"
            VERBATIM
            COMMENT "[Sanitizers] deploying ${_asan_dll}")
          set_property(DIRECTORY "${CMAKE_BINARY_DIR}" APPEND PROPERTY
            ADDITIONAL_CLEAN_FILES
              "$<PATH:NORMAL_PATH,$<TARGET_FILE_DIR:${TARGET_NAME}>/${_asan_dll}>")
        else()
          message(WARNING
            "Sanitizers: could not locate '${_asan_dll}' for deployment.")
        endif()
      endif()
    endif()

    if(HAS_UBSAN OR HAS_TSAN OR HAS_MSAN)
      message(WARNING
        "Sanitizers: MSVC natively supports only ASan ('address'). "
        "Ignoring UBSan/TSan/MSan.")
    endif()

  # ---------------------------------------------------------------------------
  # Unsupported compiler
  # ---------------------------------------------------------------------------
  else()
    message(WARNING
      "Sanitizers: compiler '${CMAKE_CXX_COMPILER_ID}' is not supported.")
    return()
  endif()

  if(_compile)
    target_compile_options(${TARGET_NAME} PRIVATE ${_compile})
  endif()
  if(_link)
    target_link_options(${TARGET_NAME} PRIVATE ${_link})
  endif()

  message(STATUS
    "Sanitizers: [${${PREFIX_UP}_SAN_MODE}] enabled on '${TARGET_NAME}' "
    "(RECOVER=${${PREFIX_UP}_SAN_RECOVER})")
endfunction()