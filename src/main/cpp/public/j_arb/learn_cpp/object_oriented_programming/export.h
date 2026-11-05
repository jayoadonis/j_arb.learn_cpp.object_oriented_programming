#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_EXPORT_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_EXPORT_H

//REM: (Shared-Helper) 
#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
      //REM: MSVC-style; cl|clang-cl; PE (Shared)
#     define OBJECT_ORIENTED_PROGRAMMING_EXPORT       __declspec(dllexport)
#     define OBJECT_ORIENTED_PROGRAMMING_IMPORT       __declspec(dllimport)
#     define OBJECT_ORIENTED_PROGRAMMING_INTERNAL
#elif defined(__GNUC__) && (__GNUC__ >= 4) 
      //REM: GNU-style; gcc|clang; ELF/Mach-O (Shared)
#     define OBJECT_ORIENTED_PROGRAMMING_EXPORT       __attribute__((visibility("default")))
#     define OBJECT_ORIENTED_PROGRAMMING_IMPORT       __attribute__((visibility("default")))
#     define OBJECT_ORIENTED_PROGRAMMING_INTERNAL     __attribute__((visibility("hidden")))
#else
      //REM: (Unknown toolchain) Fallthrough
#     define OBJECT_ORIENTED_PROGRAMMING_EXPORT 
#     define OBJECT_ORIENTED_PROGRAMMING_IMPORT 
#     define OBJECT_ORIENTED_PROGRAMMING_INTERNAL
#endif

//REM (Hybrid-Header-Only)
#if   defined(OBJECT_ORIENTED_PROGRAMMING_BUILD_HHO) 
#     define OBJECT_ORIENTED_PROGRAMMING_API             
#     define OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL    
#     define OBJECT_ORIENTED_PROGRAMMING_API_INLINE   inline 

//REM: (Shared) Build
#elif defined(OBJECT_ORIENTED_PROGRAMMING_BUILD_SHARED)
#     define OBJECT_ORIENTED_PROGRAMMING_API          OBJECT_ORIENTED_PROGRAMMING_EXPORT
#     define OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL OBJECT_ORIENTED_PROGRAMMING_INTERNAL
#     define OBJECT_ORIENTED_PROGRAMMING_API_INLINE   

//REM: (Shared) Consume
#elif defined(OBJECT_ORIENTED_PROGRAMMING_CONSUME_SHARED)
#     define OBJECT_ORIENTED_PROGRAMMING_API          OBJECT_ORIENTED_PROGRAMMING_IMPORT
#     define OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL OBJECT_ORIENTED_PROGRAMMING_INTERNAL
#     define OBJECT_ORIENTED_PROGRAMMING_API_INLINE   

//REM: (Static) Fallthrough 
#else 
#     define OBJECT_ORIENTED_PROGRAMMING_API
#     define OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL 
#     define OBJECT_ORIENTED_PROGRAMMING_API_INLINE 

#endif

//REM: (Testing override) Make internals visible
#if defined(OBJECT_ORIENTED_PROGRAMMING_BUILD_TESTING)
#     undef OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
#     define OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL OBJECT_ORIENTED_PROGRAMMING_API
#endif

//REM: (Deprecation-macro) std c++ attribute
#if defined(__has_cpp_attribute) 
#     if __has_cpp_attribute(deprecated)
#           define OBJECT_ORIENTED_PROGRAMMING_API_DEPRECATED          [[deprecated]]
#           define OBJECT_ORIENTED_PROGRAMMING_API_DEPRECATED_MSG(msg) [[deprecated(msg)]]
#     endif
#endif

//REM: (Deprecation-macro) MSVC-style attribute
#ifndef OBJECT_ORIENTED_PROGRAMMING_API_DEPRECATED
#     if defined(_MSC_VER)
#           define OBJECT_ORIENTED_PROGRAMMING_API_DEPRECATED           __declspec(deprecated)
#           define OBJECT_ORIENTED_PROGRAMMING_API_DEPRECATED_MSG(msg)  __declspec(deprecated(msg))
#     endif
#endif

//REM: (Deprecation-macro) GNU-style attribute (GCC, Clang, etc.)
#ifndef OBJECT_ORIENTED_PROGRAMMING_API_DEPRECATED
#     if defined(__has_attribute)
#           if __has_attribute(deprecated)
#                 define OBJECT_ORIENTED_PROGRAMMING_API_DEPRECATED           __attribute__((deprecated))
#                 define OBJECT_ORIENTED_PROGRAMMING_API_DEPRECATED_MSG(msg)  __attribute__((deprecated(msg)))
#           endif
#     elif defined(__GNUC__) && (__GNUC__ >= 4)
            //REM: Fallback for very old GCC versions that lack __has_attribute
            //REM: but do support __attribute__((deprecated)) (since GCC 4.5)
#           define OBJECT_ORIENTED_PROGRAMMING_API_DEPRECATED           __attribute__((deprecated))
#           define OBJECT_ORIENTED_PROGRAMMING_API_DEPRECATED_MSG(msg)  __attribute__((deprecated(msg)))
#     endif
#endif

//REM: (Deprecation-macro) Fallthrough: do nothing 
#ifndef OBJECT_ORIENTED_PROGRAMMING_API_DEPRECATED
#     define OBJECT_ORIENTED_PROGRAMMING_API_DEPRECATED
#     define OBJECT_ORIENTED_PROGRAMMING_API_DEPRECATED_MSG(msg)
#endif

#endif //REM: END; J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_EXPORT_H
