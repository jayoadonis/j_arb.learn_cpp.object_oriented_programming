#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_UTILITY_E_TYPE_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_UTILITY_E_TYPE_H

#include <istream>
#include <ostream>
#include <type_traits>
#include <string>
#include "j_arb/learn_cpp/object_oriented_programming/export.h"

namespace j_arb::learn_cpp::object_oriented_programming::utility
{

  enum class EType : unsigned int
  {
    NONE = 0u,
    UNKNOWN = 1u << 0u,
    ITEM = 1u << 1u,
    ACTOR = 1u << 2u,

    ALL = (UNKNOWN | ITEM | ACTOR)
  };

  using EType_t = std::underlying_type_t<EType>;

  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    char const * get_e_type_as_cstr(EType e_type);

  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    std::string get_e_type_as_string(EType e_type);

  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    std::string get_e_type_as_string_dump(EType e_type);
  
  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    constexpr EType get_e_type_from_string(std::string_view sv);

  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    std::ostream& operator<<(
      std::ostream& os, EType e_type);
  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    std::istream& operator>>(
      std::istream& is, EType& e_type);

  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    EType operator|(EType lhs, EType rhs);
  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    EType operator&(EType lhs, EType rhs);
  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    EType operator^(EType lhs, EType rhs);
  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    EType operator~(EType e_type);

  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    EType& operator|=(EType& lhs, EType rhs);
  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    EType& operator&=(EType& lhs, EType rhs);
  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    EType& operator^=(EType& lhs, EType rhs);

  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    EType operator<<(EType lhs, EType shift);
  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    EType operator>>(EType lhs, EType shift);
  
  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    EType operator<<(EType lhs, unsigned int shift);
  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    EType operator>>(EType lhs, unsigned int shift);
}

#if defined(OBJECT_ORIENTED_PROGRAMMING_BUILD_HHO)
# include "j_arb/learn_cpp/object_oriented_programming/utility/EType.cpp"
#endif

#endif