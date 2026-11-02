
#include "j_arb/learn_cpp/object_oriented_programming/utility/demangle.h"

#include <string>
#include <utility>
#include <memory>

#if defined(__GNUC__) || defined(__clang__)
#include <cxxabi.h>
#endif

#include "j_arb/learn_cpp/object_oriented_programming/export.h"

namespace j_arb::learn_cpp::object_oriented_programming::utility {

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE 
    std::string demangle(char const* mangled_name)
  {
  #if defined(__GNUC__) || defined(__clang__)
    constexpr int OK = 0u;
    int status = OK;
    char* demangled = abi::__cxa_demangle(
      mangled_name, NULL, NULL, &status
    );
    if (status == OK && demangled != NULL)
    {
      std::string result(demangled);
      std::free(demangled);
      return std::move(result);
    }
    return mangled_name;
  #else
    return mangled_name;
  #endif
  }

}