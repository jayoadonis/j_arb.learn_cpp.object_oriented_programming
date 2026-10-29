#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_DEMO_I_TYPE_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_DEMO_I_TYPE_H

#include <ostream>
#include <string_view>

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation::demo_i {

  enum class Type : unsigned int;

  char const* get_type_as_cstr(
    Type const & type
  );
  
  std::ostream& operator<<(
    std::ostream&, Type const &
  );

  enum class Type : unsigned int {

    UNKNOWN = 0,
    ITEM    = 1u << 1u,
    ACTOR   = 1u << 2u,
    _END
  };

  char const* get_type_as_cstr(
    Type const & type
  ) {
    switch(type) {
      case Type::ITEM:    return "ITEM";
      case Type::ACTOR:   return "ACTOR";
      case Type::UNKNOWN:
      [[fallthrough]];
      default:            return "UNKNOWN"; 
    }
  }

  std::ostream& operator<<(
    std::ostream& os, Type const & type
  ) {
    os << get_type_as_cstr(type);
    return os;
  }
}

#endif