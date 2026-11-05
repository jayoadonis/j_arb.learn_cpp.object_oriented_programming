#include "j_arb/learn_cpp/object_oriented_programming/utility/EType.h"
#include "j_arb/learn_cpp/object_oriented_programming/export.h"

namespace j_arb::learn_cpp::object_oriented_programming::utility
{

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    char const* get_e_type_as_cstr(EType e_type)
  {
    switch (e_type)
    {
    case EType::ALL:      return "ALL";
    case EType::ACTOR:    return "ACTOR";
    case EType::ITEM:     return "ITEM";
    case EType::UNKNOWN:  return "UNKNOWN";
    case EType::NONE:     return "NONE";
    default:              
      if (e_type > EType::ALL)
      {
        return "OUT_OF_BOUNDS";
      }
      else {
        return "UNION_FLAGS";
      }
    }
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    std::string get_e_type_as_string(EType e_type)
  {
    EType_t const t = static_cast<EType_t>(e_type);

    return std::string(get_e_type_as_cstr(e_type)) 
      + "(" + std::to_string(t) +")";
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    std::string get_e_type_as_string_dump(EType e_type)
  {
    EType_t const t = static_cast<EType_t>(e_type);

    return std::string(get_e_type_as_cstr(e_type))
      + "@[type_name='" + typeid(t).name() + "', value=" + std::to_string(t) + "]\n";
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    constexpr EType get_e_type_from_string(
      std::string_view sv
  ) {
    if (sv == "ALL") return EType::ALL;
    if (sv == "ACTOR") return EType::ACTOR;
    if (sv == "ITEM")  return EType::ITEM;
    if (sv == "UNKNOWN") return EType::UNKNOWN;
    return EType::NONE;
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    std::ostream& operator<<(
      std::ostream& os, EType e_type)
  {
    os << get_e_type_as_cstr(e_type);
    return os;
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    std::istream& operator>>(
      std::istream& is, EType& e_type)
  {
    std::string token;
    if (is >> token) 
    {
      EType tmp = get_e_type_from_string(token);

      if(tmp == EType::NONE)
      {
        is.setstate(std::ios::failbit);
      }
      else
      {
        e_type = tmp;
      }
    }
    return is;
  }
}