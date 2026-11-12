#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_SANITIZER_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_SANITIZER_H

#if OBJECT_ORIENTED_PROGRAMMING_SAN_MODE

  /*
   * Compiler feature-detection fallback
   */
# ifndef __has_feature
#   define __has_feature(x) 0
# endif

  /*
   * Hook linkage
   *
   * These hooks *must* be strong, externally-visible symbols so they override the
   * weak defaults in the sanitizer runtime.  In C++ `inline` provides that (it
   * gives a COMDAT-weak symbol that still overrides the shared-library default).
   * C11/C99 `inline` has the wrong semantics for this, so C users must include
   * this header from exactly one .c file.
   */
# if defined(__cplusplus)
#   define __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK inline
# else
#   define __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK    /* non-static: include from one .c file only */
# endif

  /*
   * Halt-vs-recover policy (mirrors <PREFIX>_SAN_RECOVER from CMake)
   */
# if defined(OBJECT_ORIENTED_PROGRAMMING_SAN_RECOVER) && \
    OBJECT_ORIENTED_PROGRAMMING_SAN_RECOVER
#   define __OBJECT_ORIENTED_PROGRAMMING_SAN_HALT "halt_on_error=0:abort_on_error=0:"
# else
#   define __OBJECT_ORIENTED_PROGRAMMING_SAN_HALT "halt_on_error=1:abort_on_error=0:"
# endif

  /*
   * Sanitizer presence
   */
# if defined(__SANITIZE_ADDRESS__) || __has_feature(address_sanitizer)
#   define __OBJECT_ORIENTED_PROGRAMMING_SAN_HAS_ASAN 1
# else
#   define __OBJECT_ORIENTED_PROGRAMMING_SAN_HAS_ASAN 0
# endif

# if defined(__SANITIZE_THREAD__) || __has_feature(thread_sanitizer)
#   define __OBJECT_ORIENTED_PROGRAMMING_SAN_HAS_TSAN 1
# else
#   define __OBJECT_ORIENTED_PROGRAMMING_SAN_HAS_TSAN 0
# endif

# if __has_feature(memory_sanitizer)
#   define __OBJECT_ORIENTED_PROGRAMMING_SAN_HAS_MSAN 1
# else
#   define __OBJECT_ORIENTED_PROGRAMMING_SAN_HAS_MSAN 0
# endif

# ifdef __cplusplus
extern "C" {
# endif

/*
 * MSVC (cl.exe)  -  ASan only, restricted option set
 */
# if defined(_MSC_VER) && !defined(__clang__)
#  if __OBJECT_ORIENTED_PROGRAMMING_SAN_HAS_ASAN
  __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK const char* __asan_default_options(void) {
    return "halt_on_error=1:"
      "abort_on_error=0:"
      "symbolize=1:"
      "print_stacktrace=1:"
      "alloc_dealloc_mismatch=1:"
      "new_delete_type_mismatch=1:"
      "strict_string_checks=1:"
      "strict_memcmp=1:"
      "quarantine_size_mb=256:"
      "malloc_context_size=30";
  }
#  endif

/**
 * clang-cl (Clang driving the MSVC ABI)
 * ASan + UBSan.  No LSan, TSan or MSan on Windows.
 */
# elif defined(_MSC_VER) && defined(__clang__)
#  if __OBJECT_ORIENTED_PROGRAMMING_SAN_HAS_ASAN
  __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK const char* __asan_default_options(void) {
    return __OBJECT_ORIENTED_PROGRAMMING_SAN_HALT
      "symbolize=1:"
      "print_stacktrace=1:"
      "detect_leaks=0:"
      "detect_stack_use_after_return=1:"
      "detect_invalid_pointer_pairs=2:"
      "detect_container_overflow=1:"
      "alloc_dealloc_mismatch=1:"
      "new_delete_type_mismatch=1:"
      "strict_string_checks=1:"
      "strict_memcmp=1:"
      "quarantine_size_mb=256:"
      "malloc_context_size=30";
  }
#  endif
  __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK const char* __ubsan_default_options(void) {
    return __OBJECT_ORIENTED_PROGRAMMING_SAN_HALT
      "symbolize=1:"
      "print_stacktrace=1:"
      "report_error_type=1";
  }

/*
 * MinGW / MSYS2 (UCRT64, MINGW64, CLANG64) with Clang
 * ASan (with recover) + UBSan.  No LSan/TSan/MSan on Windows.
 */
# elif defined(__MINGW32__) && defined(__clang__)
#  if __OBJECT_ORIENTED_PROGRAMMING_SAN_HAS_ASAN
  __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK const char* __asan_default_options(void) {
    return __OBJECT_ORIENTED_PROGRAMMING_SAN_HALT
      "symbolize=1:"
      "print_stacktrace=1:"
      "detect_leaks=0:"
      "detect_stack_use_after_return=1:"
      "detect_invalid_pointer_pairs=2:"
      "detect_container_overflow=1:"
      "alloc_dealloc_mismatch=1:"
      "new_delete_type_mismatch=1:"
      "strict_string_checks=1:"
      "strict_memcmp=1:"
      "quarantine_size_mb=256:"
      "malloc_context_size=30";
  }
#  endif
  __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK const char* __ubsan_default_options(void) {
    return __OBJECT_ORIENTED_PROGRAMMING_SAN_HALT
      "symbolize=1:"
      "print_stacktrace=1:"
      "report_error_type=1";
  }

/*
 * MinGW / MSYS2 with GCC
 * UBSan reliable; libasan on MinGW is partial (guarded if enabled).
 */
# elif defined(__MINGW32__) && defined(__GNUC__) && !defined(__clang__)
#  if __OBJECT_ORIENTED_PROGRAMMING_SAN_HAS_ASAN
  __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK const char* __asan_default_options(void) {
    return __OBJECT_ORIENTED_PROGRAMMING_SAN_HALT
      "symbolize=1:"
      "print_stacktrace=1:"
      "detect_leaks=0:"
      "strict_string_checks=1:"
      "strict_memcmp=1";
  }
#  endif
  __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK const char* __ubsan_default_options(void) {
    return __OBJECT_ORIENTED_PROGRAMMING_SAN_HALT
      "symbolize=1:"
      "print_stacktrace=1:"
      "report_error_type=1";
  }

/*
 * Clang on Linux / macOS / *BSD
 * ASan + LSan (not on Apple) + UBSan + TSan + MSan (not on Apple)
 */
# elif defined(__clang__) && !defined(_WIN32)
#  if __OBJECT_ORIENTED_PROGRAMMING_SAN_HAS_ASAN
  __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK const char* __asan_default_options(void) {
    return __OBJECT_ORIENTED_PROGRAMMING_SAN_HALT
      "symbolize=1:"
      "print_stacktrace=1:"
#    if defined(__APPLE__)
      "detect_leaks=0:"
#    else
      "detect_leaks=1:"
#    endif
      "detect_stack_use_after_return=1:"
      "detect_invalid_pointer_pairs=2:"
      "detect_container_overflow=1:"
      "detect_odr_violation=2:"
      "alloc_dealloc_mismatch=1:"
      "new_delete_type_mismatch=1:"
      "strict_string_checks=1:"
      "strict_memcmp=1:"
      "check_initialization_order=1:"
      "strict_init_order=1:"
      "quarantine_size_mb=256:"
      "malloc_context_size=30";
  }
#  endif
  __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK const char* __ubsan_default_options(void) {
    return __OBJECT_ORIENTED_PROGRAMMING_SAN_HALT
      "symbolize=1:"
      "print_stacktrace=1:"
      "report_error_type=1";
  }
#  if __OBJECT_ORIENTED_PROGRAMMING_SAN_HAS_TSAN
  __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK const char* __tsan_default_options(void) {
    return __OBJECT_ORIENTED_PROGRAMMING_SAN_HALT
      "symbolize=1:"
      "print_stacktrace=1:"
      "detect_thread_leaks=1:"
      "detect_deadlocks=1:"
      "second_deadlock_stack=1:"
      "report_thread_leaks=1:"
      "report_destroy_locked=1:"
      "report_mutex_bugs=1:"
      "report_signal_unsafe=1:"
      "history_size=3";
  }
#  endif
#  if __OBJECT_ORIENTED_PROGRAMMING_SAN_HAS_MSAN
  __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK const char* __msan_default_options(void) {
    return __OBJECT_ORIENTED_PROGRAMMING_SAN_HALT
      "keep_going=1:"
      "symbolize=1:"
      "print_stacktrace=1:"
      "poison_in_dtor=1:"
      "print_stats=1:"
      "report_umrs=1:"
      "wrap_signals=1:"
      "check_printf=1";
  }
#  endif

/*
 * GCC on Linux / macOS / *BSD
 * ASan + LSan (not on Apple) + UBSan + TSan.  No MSan.
 */
# elif defined(__GNUC__) && !defined(__clang__) && !defined(_WIN32)
#  if __OBJECT_ORIENTED_PROGRAMMING_SAN_HAS_ASAN
  __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK const char* __asan_default_options(void) {
    return __OBJECT_ORIENTED_PROGRAMMING_SAN_HALT
      "symbolize=1:"
      "print_stacktrace=1:"
#    if defined(__APPLE__)
      "detect_leaks=0:"
#    else
      "detect_leaks=1:"
#    endif
      "detect_stack_use_after_return=1:"
      "detect_invalid_pointer_pairs=2:"
      "detect_container_overflow=1:"
      "detect_odr_violation=2:"
      "alloc_dealloc_mismatch=1:"
      "new_delete_type_mismatch=1:"
      "strict_string_checks=1:"
      "strict_memcmp=1:"
      "check_initialization_order=1:"
      "strict_init_order=1:"
      "quarantine_size_mb=256:"
      "malloc_context_size=30";
  }
#  endif
  __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK const char* __ubsan_default_options(void) {
    return __OBJECT_ORIENTED_PROGRAMMING_SAN_HALT
      "symbolize=1:"
      "print_stacktrace=1:"
      "report_error_type=1";
  }
#  if __OBJECT_ORIENTED_PROGRAMMING_SAN_HAS_TSAN
  __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK const char* __tsan_default_options(void) {
    return __OBJECT_ORIENTED_PROGRAMMING_SAN_HALT
      "symbolize=1:"
      "print_stacktrace=1:"
      "detect_thread_leaks=1:"
      "detect_deadlocks=1:"
      "second_deadlock_stack=1:"
      "report_thread_leaks=1:"
      "report_destroy_locked=1:"
      "report_mutex_bugs=1:"
      "report_signal_unsafe=1:"
      "history_size=3";
  }
#  endif
# endif /* compiler dispatch */

# ifdef __cplusplus
}
# endif

# undef __OBJECT_ORIENTED_PROGRAMMING_SAN_HAS_ASAN
# undef __OBJECT_ORIENTED_PROGRAMMING_SAN_HAS_TSAN
# undef __OBJECT_ORIENTED_PROGRAMMING_SAN_HAS_MSAN
# undef __OBJECT_ORIENTED_PROGRAMMING_SAN_HALT
# undef __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK

#endif /* OBJECT_ORIENTED_PROGRAMMING_SAN_MODE */
#endif /* J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_SANITIZER_H */