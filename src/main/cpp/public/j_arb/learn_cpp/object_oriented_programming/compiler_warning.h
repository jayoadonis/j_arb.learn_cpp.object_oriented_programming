#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_COMPILER_WARNING_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_COMPILER_WARNING_H

#if defined(_MSC_VER)
# define OBJECT_ORIENTED_PROGRAMMING_COMPILER_ALLOW_SHADOW_BEGIN \
        __pragma(warning(push)) \
        __pragma(warning(disable : 4458))
# define OBJECT_ORIENTED_PROGRAMMING_COMPILER_ALLOW_SHADOW_END \
        __pragma(warning(pop))
#elif defined(__clang__) || defined(__GNUC__) /*REM: TODO for prior c++11*/
# define OBJECT_ORIENTED_PROGRAMMING_COMPILER_ALLOW_SHADOW_BEGIN \
        _Pragma("GCC diagnostic push") \
        _Pragma("GCC diagnostic ignored \"-Wshadow\"")
# define OBJECT_ORIENTED_PROGRAMMING_COMPILER_ALLOW_SHADOW_END \
        _Pragma("GCC diagnostic pop")
#else
# define OBJECT_ORIENTED_PROGRAMMING_COMPILER_ALLOW_SHADOW_BEGIN
# define OBJECT_ORIENTED_PROGRAMMING_COMPILER_ALLOW_SHADOW_END
#endif

#endif