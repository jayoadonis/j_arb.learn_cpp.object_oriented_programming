#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_UTILITY_DEMANGLE_TPP
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_UTILITY_DEMANGLE_TPP

#include <string_view>

#include "j_arb/learn_cpp/object_oriented_programming/utility/demangle.h"

namespace j_arb::learn_cpp::object_oriented_programming::utility {

  template<typename T>
  std::string get_type_name()
  {
    return demangle(typeid(T).name());
  }

  template<typename T>
  constexpr std::string_view get_type_name_constexpr() 
  {
  #if defined(__GNUC__) || defined(__clang__)
    constexpr std::string_view sig = __PRETTY_FUNCTION__;
    constexpr std::string_view marker = "[T = ";
    constexpr std::string_view alt_marker = "[with T = ";
    constexpr std::string_view suffix = "]";

    auto start = sig.find(marker);
    if constexpr (start != std::string_view::npos) {
      start += marker.length();
    }
    else {
      start = sig.find(alt_marker);
      if constexpr (start == std::string_view::npos) return sig;
      start += alt_marker.length();
    }
    const auto end = sig.find(suffix, start);
    return sig.substr(start, end - start);

  #elif defined(_MSC_VER)
    constexpr std::string_view sig = __FUNCSIG__;
    constexpr std::string_view marker = "get_type_name_constexpr<";
    constexpr std::string_view suffix = ">(void)";

    if constexpr (const auto marker_pos = sig.find(marker);
    marker_pos == std::string_view::npos
      ) {
      return sig;
    }
    else {
      const auto start = marker_pos + marker.length();
      const auto end = sig.find(suffix, start);
      return sig.substr(start, end - start);
    }
  #else
    return "Unsupported Compiler";
  #endif
  }
}

#endif