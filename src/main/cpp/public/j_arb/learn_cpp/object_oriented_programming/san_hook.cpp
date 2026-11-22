#if defined(OBJECT_ORIENTED_PROGRAMMING_SAN_MODE_ENABLED) && \
    OBJECT_ORIENTED_PROGRAMMING_SAN_MODE_ENABLED != 0

#   ifndef OBJECT_ORIENTED_PROGRAMMING_SAN_RECOVER
    //REM: Fallback to disable recover
#       define OBJECT_ORIENTED_PROGRAMMING_SAN_RECOVER 0
#   endif

#   ifndef OBJECT_ORIENTED_PROGRAMMING_SAN_MAX_ERRORS
    //REM: Fallback to Unlimited
#       define OBJECT_ORIENTED_PROGRAMMING_SAN_MAX_ERRORS 0
#   endif

#   define __OBJECT_ORIENTED_PROGRAMMING_STR_IMPL(x) # x
#   define __OBJECT_ORIENTED_PROGRAMMING_STR(x)  \
        __OBJECT_ORIENTED_PROGRAMMING_STR_IMPL(x)

#   if OBJECT_ORIENTED_PROGRAMMING_SAN_RECOVER
#       define __OBJECT_ORIENTED_PROGRAMMING_HALT_OPT "halt_on_error=0"
#       define __OBJECT_ORIENTED_PROGRAMMING_MAX_ERR_OPT    \
            ":max_errors=" __OBJECT_ORIENTED_PROGRAMMING_STR(OBJECT_ORIENTED_PROGRAMMING_SAN_MAX_ERRORS)
#   else
#       define __OBJECT_ORIENTED_PROGRAMMING_HALT_OPT "halt_on_error=1"
#       define __OBJECT_ORIENTED_PROGRAMMING_MAX_ERR_OPT ""
#   endif

// REM: Pure msvc
#   if defined(_MSC_VER) && !defined(__clang__)
#       define __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK_EXPORT    \
            __declspec(dllexport)
#       define __OBJECT_ORIENTED_PROGRAMMING_NO_SANITIZE    \
            __declspec(no_sanitize_address)
// REM: clang-cl (msvc-like)
#   elif defined(_MSC_VER) && defined(__clang__)
#       define __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK_EXPORT    \
            __declspec(dllexport)
#       define __OBJECT_ORIENTED_PROGRAMMING_NO_SANITIZE \
            __attribute__((no_sanitize("address", "undefined")))
// REM: Standard clang
#   elif defined(__clang__)
#       define __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK_EXPORT    \
            __attribute__((used, visibility("default")))
#       define __OBJECT_ORIENTED_PROGRAMMING_NO_SANITIZE    \
            __attribute__(( \
                no_sanitize("address", "undefined", "thread", "memory", "leak") \
            ))
// REM: Standard GCC/G++
#   elif defined(__GNUC__)
#       define __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK_EXPORT    \
            __attribute__((used, visibility("default")))
#       define __OBJECT_ORIENTED_PROGRAMMING_NO_SANITIZE    \
            __attribute__(( \
                no_sanitize_address, no_sanitize_undefined, no_sanitize_thread  \
            ))
#   else
#       define __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK_EXPORT
#       define __OBJECT_ORIENTED_PROGRAMMING_NO_SANITIZE
#   endif

extern "C" {

    __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK_EXPORT __OBJECT_ORIENTED_PROGRAMMING_NO_SANITIZE const char*
        __asan_default_options()
    {
        return "detect_stack_use_after_return=1:" __OBJECT_ORIENTED_PROGRAMMING_HALT_OPT
            __OBJECT_ORIENTED_PROGRAMMING_MAX_ERR_OPT ":exitcode=1";
    }

    __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK_EXPORT __OBJECT_ORIENTED_PROGRAMMING_NO_SANITIZE const char*
        __lsan_default_options()
    {
        return "print_suppressions=0:max_leaks=" __OBJECT_ORIENTED_PROGRAMMING_STR(
            OBJECT_ORIENTED_PROGRAMMING_SAN_MAX_ERRORS
        ) ":exitcode=1";
    }

    __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK_EXPORT __OBJECT_ORIENTED_PROGRAMMING_NO_SANITIZE const char*
        __ubsan_default_options()
    {
        return __OBJECT_ORIENTED_PROGRAMMING_HALT_OPT ":print_stacktrace=1:exitcode=1";
    }

    __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK_EXPORT __OBJECT_ORIENTED_PROGRAMMING_NO_SANITIZE const char*
        __tsan_default_options()
    {
        return __OBJECT_ORIENTED_PROGRAMMING_HALT_OPT ":second_deadlock_stack=1:exitcode=1";
    }

    __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK_EXPORT __OBJECT_ORIENTED_PROGRAMMING_NO_SANITIZE const char*
        __msan_default_options()
    {
        return __OBJECT_ORIENTED_PROGRAMMING_HALT_OPT __OBJECT_ORIENTED_PROGRAMMING_MAX_ERR_OPT ":exitcode=1";
    }

} // REM: extern "C"

#   undef __OBJECT_ORIENTED_PROGRAMMING_STR_IMPL
#   undef __OBJECT_ORIENTED_PROGRAMMING_STR
#   undef __OBJECT_ORIENTED_PROGRAMMING_HALT_OPT
#   undef __OBJECT_ORIENTED_PROGRAMMING_MAX_ERR_OPT
#   undef __OBJECT_ORIENTED_PROGRAMMING_SAN_HOOK_EXPORT
#   undef __OBJECT_ORIENTED_PROGRAMMING_NO_SANITIZE

#endif //REM: OBJECT_ORIENTED_PROGRAMMING_SAN_MODE_ENABLED
