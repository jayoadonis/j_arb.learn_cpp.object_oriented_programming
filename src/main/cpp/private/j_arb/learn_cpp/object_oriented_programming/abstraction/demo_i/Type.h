#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ABSTRACTION_DEMO_I_TYPE_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ABSTRACTION_DEMO_I_TYPE_H

#include <iostream>
#include <ostream>
#include <string>
#include "j_arb/learn_cpp/object_oriented_programming/export.h"
#include <type_traits>

namespace j_arb::learn_cpp::object_oriented_programming::abstraction::demo_i
{

  enum class
    OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    EType : unsigned int
  {
    NONE = 0u,
    UNKNOWN = 1u << 0u,
    ITEM = 1u << 1u,
    ACTOR = 1u << 2u,

    ALL = (UNKNOWN | ITEM | ACTOR)
  };

  using EType_t = std::underlying_type_t<EType>;

  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    std::string get_type_cstr(EType const& type);

  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    std::ostream& operator<<(std::ostream& os, EType const& type);

  inline OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    EType operator<<(EType lhs, EType rhs)
  {
    return static_cast<EType>(
      static_cast<EType_t>(lhs)
      << static_cast<EType_t>(rhs)
      );
  }

  inline OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    EType operator>>(EType lhs, EType rhs)
  {
    return static_cast<EType>(
      static_cast<EType_t>(lhs)
      >> static_cast<EType_t>(rhs)
      );
  }

  inline OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    EType operator~(EType e_type)
  {
    if (e_type >= EType::ALL) return EType::ALL;

    return static_cast<EType>(
      ~static_cast<EType_t>(e_type)
      );
  }

  inline OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    EType operator&(
      EType const& lhs,
      EType const& rhs)
  {
    return static_cast<EType>(
      static_cast<EType_t>(lhs)
      & static_cast<EType_t>(rhs)
    );
  }

  inline OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    EType operator|(EType lhs, EType rhs)
  {
    return static_cast<EType>(
      static_cast<EType_t>(lhs) | static_cast<EType_t>(rhs)
      );
  }

  class Type final
  {
    public: OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
      explicit Type();
    public: OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
      explicit Type(EType e_type);
    public: OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
      ~Type() = default;

    public: OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
      operator EType() const;

    public: OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
      Type& operator=(EType const& e_type);

    //REM: `.contains(unsigned int e_types): unsigned int`
    public: inline OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
      EType operator[](EType e_type) const
    {
      return ((e_type & this->m_e_type) != e_type)
        ? EType::NONE : e_type;
    }

    public: OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
      char const* to_cstr() const;

    private: OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
      EType m_e_type;

    public: OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
      inline static const EType NONE
      = EType::NONE;
    public: OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
      inline static const EType UNKNOWN
      = EType::UNKNOWN;
    public: OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
      inline static const EType ITEM
      = EType::ITEM;
    public: OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
      inline static const EType ACTOR
      = EType::ACTOR;
  };
}

#if defined(OBJECT_ORIENTED_PROGRAMMING_BUILD_HHO)
# include "j_arb/learn_cpp/object_oriented_programming/abstraction/demo_i/Type.cpp"
#endif

#endif