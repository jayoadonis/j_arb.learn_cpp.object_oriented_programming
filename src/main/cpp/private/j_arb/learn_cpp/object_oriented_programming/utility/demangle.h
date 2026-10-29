#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_UTILITY_DEMANGLE_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_UTILITY_DEMANGLE_H

#include <string_view>
#include <string>

#include "j_arb/learn_cpp/object_oriented_programming/export.h"

namespace j_arb::learn_cpp::object_oriented_programming::utility {

  OBJECT_ORIENTED_PROGRAMMING_API 
  std::string demangle(char const* mangled_name);

  template<typename T>
  std::string get_type_name();

  template<typename T>
  constexpr std::string_view get_type_name_constexpr();
}

#if defined(OBJECT_ORIENTED_PROGRAMMING_BUILD_HHO)
# include "j_arb/learn_cpp/object_oriented_programming/utility/demangle.cpp"
#endif

#include "./demangle.tpp"

#endif