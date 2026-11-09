#include "j_arb/learn_cpp/object_oriented_programming/export.h"
#include "j_arb/learn_cpp/object_oriented_programming/abstraction/demo_i/Type.h"
#include <iostream>

#include <type_traits>
#include <string>

namespace j_arb::learn_cpp::object_oriented_programming::abstraction::demo_i
{

  //REM: Do this if you want an internal/private Enum;
  //REM: Note external testing/Usage will not work with it,
  //REM: even if you have a forward declaration.
  // OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  // OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
  //   enum class EType : unsigned int
  // {
  //   UNKNOWN = 1u,
  //   ITEM = 1u << 1u,
  //   ACTOR = 1u << 2u,
  //   _END
  // };

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    std::string get_type_as_string(EType const& type)
  {

    //REM: underlying type with given instance.
    //REM: Use `std::decay_t<T>` it remove ref `&` and cv-qualifiers (const-volatile) `const`
    // using EType_tt = std::underlying_type_t<std::dacay_t<decltype(type)>>;

    EType_t t = static_cast<EType_t>(type);

    //REM: Or use with `volatile Type const & type_volatile = type;`,

    std::string result;

    switch (type)
    {
    case EType::ALL:
      // return result = "ALL (" + std::to_string(t) + ")";
      result = "ALL";
      break;
    case EType::ACTOR:
      // return "ACTOR (" + std::to_string(t) + ")";
      result = "ACTOR";
      break;
    case EType::ITEM:
      // return "ITEM (" + std::to_string(t) + ")";
      result = "ITEM";
      break;
    case EType::UNKNOWN:
      // return "UNKNOWN (" + std::to_string(t) + ")";
      result = "UNKNOWN";
      break;
    case EType::NONE:
      // return "NONE (" + std::to_string(t) + ")";
      result = "NONE";
      break;
    // case static_cast<unsigned int>(EType::UNKNOWN):
    //   [[fallthrough]];
    default:
      // return "UNION_FLAGS: " + std::string(typeid(EType_t).name()) 
        // +  " (" + std::to_string(t) + ")";
      result = "UNION_FLAGS: " + std::string(typeid(EType_t).name());
    }

    return ( result + " (" + std::to_string(t) + ")" );
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    std::ostream& operator<<(std::ostream& os, EType const& type)
  {
    return os << get_type_as_string(type);
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    Type::Type() : Type(EType::UNKNOWN)
  {
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    Type::Type(EType e_type) :
    m_e_type(e_type)
  {
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    Type& Type::operator=(EType const& e_type)
  {
    this->m_e_type = e_type;
    return *this;
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    Type::operator EType() const
  {
    return this->m_e_type;
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  OBJECT_ORIENTED_PROGRAMMING_API_INTERNAL
    char const* Type::to_cstr() const
  {
    return get_type_as_string(*this).c_str();
  }

}