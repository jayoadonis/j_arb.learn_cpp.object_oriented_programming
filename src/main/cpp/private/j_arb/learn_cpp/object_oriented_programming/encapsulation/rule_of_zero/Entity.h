#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_RULE_OF_ZERO_ENTITY_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_RULE_OF_ZERO_ENTITY_H

#include <string>
#include "j_arb/learn_cpp/object_oriented_programming/export.h"

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation::rule_of_zero {


  class Entity {

    public: OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
      explicit Entity() = default;

    //REM: [x] I (dtor)
    public: OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
      virtual ~Entity() = default;

    //REM: [x] II (copy ctor)
    protected: OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
      explicit Entity(Entity const& other) = default;

    //REM: [x] III (copy assignment member operator)
    protected: OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
      Entity& operator=(Entity const& other) = default;

    //REM: [x] IV (move ctor)
    protected: OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
      explicit Entity(Entity&& other) noexcept = default;

    //REM: [x] V (move assignment member operator)
    protected: OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
      Entity& operator=(Entity&& other) noexcept = default;

    public: OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
      virtual void set_id(std::string const & id);

    public: OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
      virtual char const* get_id() const;

    private: std::string m_id;
  };
}

#if defined(OBJECT_ORIENTED_PROGRAMMING_BUILD_HHO)
# include "./Entity.cpp"
#endif

#endif