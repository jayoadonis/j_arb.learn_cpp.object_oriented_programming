#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_RULE_OF_THREE_PLAYER_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_RULE_OF_THREE_PLAYER_H

#include "./Model.h"

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation::rule_of_three {

  class Player : public Model {
    // public: virtual std::string to_string() const override
    // {
    //   std::ostringstream oss;
    //   //REM: TODO demangle name for GNU/CLANG
    //   oss << object_oriented_programming::
    //     utility::get_type_name_constexpr<decltype(*this)>()
    //     << "@"
    //     << "[]";

    //   return oss.str();
    // }

    // public: friend std::ostream& operator<<(
    //   std::ostream& os, Player const& model
    //   ) {
    //   os << model.to_string();
    //   return os;
    // }
  };
}

#endif