#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_ENTITY_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_ENTITY_H

#include "j_arb/learn_cpp/object_oriented_programming/export.h"

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation {

  class Entity {

    public:
    OBJECT_ORIENTED_PROGRAMMING_API
      explicit Entity();

    public:
    OBJECT_ORIENTED_PROGRAMMING_API
      virtual ~Entity();

    public:
    OBJECT_ORIENTED_PROGRAMMING_API
      virtual Entity* clone() const;

    protected:
    OBJECT_ORIENTED_PROGRAMMING_API
      explicit Entity(Entity const& entity);

    protected:
    OBJECT_ORIENTED_PROGRAMMING_API
      Entity& operator=(Entity const& entity);

    protected:
    OBJECT_ORIENTED_PROGRAMMING_API
      explicit Entity(Entity&& entity) noexcept;

    protected:
    OBJECT_ORIENTED_PROGRAMMING_API
      Entity& operator=(Entity&& entity) noexcept;

    public:
    OBJECT_ORIENTED_PROGRAMMING_API
      virtual void set_id(char const* id);

    public:
    OBJECT_ORIENTED_PROGRAMMING_API
      virtual char const* get_id() const;

    private: char* m_id_ptr;
  };
}

#if defined(OBJECT_ORIENTED_PROGRAMMING_BUILD_HHO)
# include "./Entity.cpp"
#endif

#endif