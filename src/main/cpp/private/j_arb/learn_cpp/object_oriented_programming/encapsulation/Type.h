#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_TYPE_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_TYPE_H

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation {

  enum class Type : unsigned int {

    UNKNOWN = 0b0000'0000u,
    POLY = 1 << 1,
    ITEM = 1 << 2,
    ACTOR = 0x0008u,

    _END,
  };

  inline constexpr char const* get_type_str(
    Type const & type
  ) {
    
    switch(type) {
      case Type::ACTOR:   return "ACTOR";
      case Type::ITEM:    return "ITEM";
      case Type::POLY:    return "POLY";
      case Type::UNKNOWN: [[fallthrough]];
      default:            return  "UNKNOWN";
    }
  }
}

#endif