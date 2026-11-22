
include_guard(GLOBAL)
include(CheckCXXCompilerFlag)

#[[
  <TARGET_NAME> -- project_name
  <PREFIX>      -- PROJECT_NAME
]]
function(enable_target_sanitizers TARGET_NAME PREFIX)
  if(NOT TARGET ${TARGET_NAME})
    message(FATAL_ERROR
      "[${TARGET_NAME}](enable_target_sanitizers): '${TARGET_NAME}' is not a valid CMake target.")
  endif()

  string(STRIP "${PREFIX}" PREFIX_UP)
  string(TOUPPER "${PREFIX_UP}" PREFIX_UP)

  if("${PREFIX_UP}" STREQUAL "")
    message(FATAL_ERROR
      "[${TARGET_NAME}](enable_target_sanitizers): '${PREFIX}' is not a valid PREFIX such as (\${PREFIX}_SAN_MODE)")
  endif()

  get_target_property(_imported ${TARGET_NAME} IMPORTED)

  if(_imported)
    message(DEBUG "[${TARGET_NAME}](enable_target_sanitizers): skipping imported target '${TARGET_NAME}'")
    return()
  endif()

  get_target_property(_type ${TARGET_NAME} TYPE)

  if(_type STREQUAL "INTERFACE_LIBRARY")
    message(DEBUG "[${TARGET_NAME}](enable_target_sanitizers): skipping interface library '${TARGET_NAME}'")
    return()
  endif()

  set(${PREFIX_UP}_SAN_MODE "none" CACHE STRING
    "List of sanitizers to enable: none, address, undefined, memory, thread")
  option(${PREFIX_UP}_SAN_FORCE_STATIC
    "Force static sanitizer runtime linkage (GCC)" OFF)
  option(${PREFIX_UP}_SAN_RECOVER
    "Continue after sanitizer errors (runtime-recoverable)" OFF)
  set(${PREFIX_UP}_SAN_MAX_ERRORS
    "0" CACHE STRING "Maximum number of errors/leaks reported per run")

  string(TOLOWER "${${PREFIX_UP}_SAN_MODE}" _mode)

  # REM: foreach() only splits on ';', so normalize commas to semicolons here.
  string(REPLACE "," ";" _mode "${_mode}")
  string(REPLACE " " "" _mode "${_mode}")

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

  if(${PREFIX_UP}_SAN_MAX_ERRORS STREQUAL "")
    set(_max_errors "0")
  elseif(NOT ${PREFIX_UP}_SAN_MAX_ERRORS MATCHES "^[0-9]+$")
    message(FATAL_ERROR "${PREFIX_UP}_SAN_MAX_ERRORS must be a non-negative integer, got: '${${PREFIX_UP}_SAN_MAX_ERRORS}'")
  endif()

  target_compile_definitions(${TARGET_NAME} PRIVATE
    ${PREFIX_UP}_SAN_MODE_ENABLED=${_enabled}
    ${PREFIX_UP}_SAN_RECOVER=${_recover_def}
    ${PREFIX_UP}_SAN_MAX_ERRORS=${_max_errors})

  if(NOT _enabled)
    message(DEBUG "[${TARGET_NAME}](enable_target_sanitizers): disabled on target '${TARGET_NAME}'")
    return()
  endif()

  message(WARNING
    "[${TARGET_NAME}](enable_target_sanitizers): Sanitizers flawlessly works on Debug & RelWithDebInfo.")

  set(HAS_ASAN FALSE)
  set(HAS_UBSAN FALSE)
  set(HAS_TSAN FALSE)
  set(HAS_MSAN FALSE)

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
        "[${TARGET_NAME}](enable_target_sanitizers): 'none' cannot be combined with active sanitizers "
        "in list: '${${PREFIX_UP}_SAN_MODE}'")
    else()
      message(FATAL_ERROR
        "[${TARGET_NAME}](enable_target_sanitizers): unknown mode '${MODE}'. Choose from: "
        "none, address, undefined, memory, thread")
    endif()
  endforeach()

  if((HAS_TSAN AND HAS_ASAN) OR(HAS_MSAN AND HAS_ASAN) OR(HAS_MSAN AND HAS_TSAN))
    message(FATAL_ERROR
      "[${TARGET_NAME}](enable_target_sanitizers): address, thread and memory sanitizers are mutually exclusive.")
  endif()

  set(_compile "")
  set(_link "")

  # REM: GCC / Clang / AppleClang / clang-cl.
  # REM: NOTE: clang-cl is routed here because it fully supports ASan, UBSan and
  # REM: TSan using the standard Clang flag spellings below. It does NOT
  # REM: support MSan on Windows (same as upstream Clang on Windows).
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
        message(FATAL_ERROR "[${TARGET_NAME}](enable_target_sanitizers): MSan is not supported by GCC; use Clang.")
      endif()

      list(APPEND _san_list "memory")
      list(APPEND _compile "-fsanitize-memory-track-origins=2")
    endif()

    if(_san_list)
      list(JOIN _san_list "," _san_str)
      set(_test_flag "-fsanitize=${_san_str}")

      string(MAKE_C_IDENTIFIER "${_test_flag}" _flag_id)
      set(_check_var "COMPILER_SUPPORTS_${PREFIX_UP}_SAN_${_flag_id}")

      set(CMAKE_REQUIRED_FLAGS "${_test_flag} -fno-omit-frame-pointer")
      set(CMAKE_REQUIRED_LINK_OPTIONS "${_test_flag}")
      check_cxx_compiler_flag("${_test_flag}" ${_check_var})
      unset(CMAKE_REQUIRED_FLAGS)
      unset(CMAKE_REQUIRED_LINK_OPTIONS)

      if(NOT ${_check_var})
        message(FATAL_ERROR
          "[${TARGET_NAME}](enable_target_sanitizers): ${CMAKE_CXX_COMPILER_ID} does not support '${_test_flag}'.")
      endif()

      list(APPEND _compile "${_test_flag}")
      list(APPEND _link "${_test_flag}")
    endif()

    # REM: Recover / halt behavior
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

    # REM: Optional static runtime linkage (GCC only by design)
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

  # REM: MSVC (cl.exe) -- ASan only.
  # REM: Only the cl.exe-specific pieces live here. INCREMENTAL/LNK2038/DLL
  # REM: deployment are handled in the shared MSVC-target block below so that
  # REM: clang-cl receives the same runtime treatment.
  elseif(MSVC)
    if(HAS_ASAN)
      set(_check_var "MSVC_SUPPORTS_${PREFIX_UP}_SAN_ASAN")
      set(CMAKE_REQUIRED_FLAGS "/fsanitize=address")
      check_cxx_compiler_flag("/fsanitize=address" ${_check_var})
      unset(CMAKE_REQUIRED_FLAGS)

      if(NOT ${_check_var})
        message(FATAL_ERROR
          "[${TARGET_NAME}](enable_target_sanitizers): this MSVC does not support '/fsanitize=address'.")
      endif()

      list(APPEND _compile "/fsanitize=address" "/Zi")

      if(${PREFIX_UP}_SAN_RECOVER)
        set(_check_var "MSVC_SUPPORTS_${PREFIX_UP}_SAN_RECOVER")
        set(CMAKE_REQUIRED_FLAGS "/fsanitize=address")
        check_cxx_compiler_flag("/fsanitize-recover=address" ${_check_var})
        unset(CMAKE_REQUIRED_FLAGS)

        if(${_check_var})
          list(APPEND _compile "/fsanitize-recover=address")
        else()
          message(WARNING
            "[${TARGET_NAME}](enable_target_sanitizers): this MSVC does not support '/fsanitize-recover=address'.")
        endif()
      endif()
    endif()

    if(HAS_UBSAN OR HAS_TSAN OR HAS_MSAN)
      message(WARNING
        "[${TARGET_NAME}](enable_target_sanitizers): MSVC (cl.exe) natively supports only ASan ('address'). "
        "Ignoring UBSan/TSan/MSan. Use clang-cl for UBSan/TSan.")
    endif()

  else()
    message(WARNING
      "[${TARGET_NAME}](enable_target_sanitizers): compiler '${CMAKE_CXX_COMPILER_ID}' is not supported.")
    return()
  endif()

  # REM: SHARED MSVC-TARGET POST-PROCESSING
  # REM: Runs for BOTH cl.exe AND clang-cl. CMake sets MSVC=TRUE for clang-cl
  # REM: (CMAKE_CXX_SIMULATE_ID=MSVC), so this single block covers both compilers.
  # REM: Both produce PE binaries that need identical runtime treatment:
  # REM: (a) /INCREMENTAL:NO      -- incremental linking is incompatible with ASan
  # REM: (b) LNK2038 mitigation   -- annotate_string / annotate_vector mismatch
  # REM: against non-ASan static libraries
  # REM: (c) The ASan runtime DLL -- clang_rt.asan_dynamic-<arch>.dll -- copied
  # REM: next to the executable/shared library AND
  # REM: registered for `clean`.
  if(MSVC AND HAS_ASAN)
    list(APPEND _link "/INCREMENTAL:NO")

    # REM: (b) LNK2038 mitigation.
    # REM: If /fno-sanitize-address-vcasan-lib is supported we use it; otherwise
    # REM: we fall back to disabling the string/vector annotation macros so that
    # REM: linking against non-ASan static libraries does not trip LNK2038.
    set(_check_var "MSVC_SUPPORTS_${PREFIX_UP}_SAN_NOVCASAN")
    set(CMAKE_REQUIRED_FLAGS "/fsanitize=address /fno-sanitize-address-vcasan-lib")
    check_cxx_compiler_flag("/fno-sanitize-address-vcasan-lib" ${_check_var})
    unset(CMAKE_REQUIRED_FLAGS)

    if(${_check_var})
      list(APPEND _compile "/fno-sanitize-address-vcasan-lib")
    endif()

    target_compile_definitions(${TARGET_NAME} PRIVATE
      _DISABLE_STRING_ANNOTATION
      _DISABLE_VECTOR_ANNOTATION)

    # REM: (c) ASan runtime DLL deployment (cl.exe AND clang-cl).
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

      # REM: Search paths, in order of preference:
      # REM: 1. clang-cl: beside clang-cl.exe  (<LLVM>/bin)
      # REM: 2. MSVC:     <VS>/VC/Tools/MSVC/<ver>/bin/Hostx64/x64
      # REM: 3. clang-cl: clang resource dir   (<LLVM>/lib/clang/<ver>/lib/windows)
      set(_asan_search_paths
        "${_compiler_bin}"
        "${_compiler_bin}/../../../../bin/${_msvc_host_arch}/${_msvc_target_arch}")

      if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        execute_process(
          COMMAND "${CMAKE_CXX_COMPILER}" -print-resource-dir
          OUTPUT_VARIABLE _clang_resource_dir
          OUTPUT_STRIP_TRAILING_WHITESPACE
          ERROR_QUIET)

        if(_clang_resource_dir AND EXISTS "${_clang_resource_dir}/lib/windows")
          list(APPEND _asan_search_paths "${_clang_resource_dir}/lib/windows")
        endif()
      endif()

      find_file(_asan_dll_path
        NAMES "${_asan_dll}"
        PATHS ${_asan_search_paths}
        NO_DEFAULT_PATH
        NO_CACHE)

      if(_asan_dll_path)
        # REM: (c.1) COPY the DLL next to the client binary on every build.
        add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "${_asan_dll_path}"
          "$<TARGET_FILE_DIR:${TARGET_NAME}>"
          VERBATIM
          COMMENT "[${TARGET_NAME}](build) deploying sanitizer [${_asan_dll}]")

        # REM: (c.2) BIND the deployed DLL to `clean`.
        # REM:
        # REM: Generator support for ADDITIONAL_CLEAN_FILES:
        # REM:   - Ninja, Ninja Multi-Config, Unix Makefiles, MSYS Makefiles ...
        # REM:       -> honored  (genex needs CMake >= 3.20)
        # REM:   - Visual Studio, Xcode, others
        # REM:       -> IGNORED  -> fall back to hooking the built-in
        # REM:                     `clean` target via add_dependencies().
        if(CMAKE_GENERATOR MATCHES "Ninja|Makefiles")
          #REM: Ninja / Makefile path
          if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.20")
            get_target_property(_tgt_src_dir ${TARGET_NAME} SOURCE_DIR)
            set_property(DIRECTORY "${_tgt_src_dir}" APPEND PROPERTY
              ADDITIONAL_CLEAN_FILES
              "$<TARGET_FILE_DIR:${TARGET_NAME}>/${_asan_dll}")
          else()
            message(DEBUG
              "[${TARGET_NAME}](enable_target_sanitizers): CMake ${CMAKE_VERSION} < 3.20; skipping clean "
              "registration for '${_asan_dll}' (copy still works).")
          endif()
        else()
          # REM: Visual Studio / Xcode / others
          # REM: `clean` is a built-in CMake target. Adding a custom target as
          # REM: its dependency makes it run when the user cleans.
          set(_clean_tgt "_sanitizer_clean_${TARGET_NAME}")

          if(NOT TARGET ${_clean_tgt})
            add_custom_target(${_clean_tgt}
              COMMAND ${CMAKE_COMMAND} -E rm -f
              "$<TARGET_FILE_DIR:${TARGET_NAME}>/${_asan_dll}"
              COMMENT "[${TARGET_NAME}](clean) removing sanitizer [${_asan_dll}]"
              VERBATIM)

            # REM: `clean` is generated per-directory; in VS it is exposed as a
            # REM: solution-level target. add_dependencies() is best-effort: if
            # REM: the generator does not expose `clean`, we simply skip it and
            # REM: the user can invoke `cmake --build . --target ${_clean_tgt}`.
            if(TARGET clean)
              add_dependencies(clean ${_clean_tgt})
            else()
              message(WARNING
                "[${TARGET_NAME}](enable_target_sanitizers): generator "
                "'${CMAKE_GENERATOR}' does not expose a `clean` target; invoke "
                "`cmake --build . --target ${_clean_tgt}` to remove "
                "'${_asan_dll}' manually.")
            endif()
          endif()
        endif()
      else()
        message(WARNING
          "[${TARGET_NAME}](enable_target_sanitizers): could not locate '${_asan_dll}' for deployment. "
          "Searched: ${_asan_search_paths}")
      endif()
    endif()
  endif()

  if(_compile)
    target_compile_options(${TARGET_NAME} PRIVATE ${_compile})
  endif()

  if(_link)
    target_link_options(${TARGET_NAME} PRIVATE ${_link})
  endif()

  message(STATUS
    "[${TARGET_NAME}](enable_target_sanitizers): [${${PREFIX_UP}_SAN_MODE}] enabled on '${TARGET_NAME}' "
    "(RECOVER=${${PREFIX_UP}_SAN_RECOVER})")
endfunction()