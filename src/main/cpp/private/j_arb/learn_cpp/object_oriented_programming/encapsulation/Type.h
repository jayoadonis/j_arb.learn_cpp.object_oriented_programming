#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_TYPE_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_TYPE_H

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation {

  enum class Type : unsigned int {

    UNKNOWN = 0b0000'0000u,
    POLY = 0x0002u,
    ITEM = 0b0000'0100u,
    ACTOR = 0x0008u,
  };
}

#endif