#include "j_arb/learn_cpp/object_oriented_programming/utility/EType.h"
#include "j_arb/learn_cpp/object_oriented_programming/export.h"

namespace j_arb::learn_cpp::object_oriented_programming::utility
{

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE 
    char const * get_e_type_as_cstr(EType e_type)
  {
    switch(e_type)
    {
      case EType::ALL:      return "ALL";
      case EType::ACTOR:    return "ACTOR";
      case EType::ITEM:     return "ITEM";
      case EType::UNKNOWN:  return "UNKNOWN";
      case EType::NONE:     return "NONE";
      default:              return "UNION_FLAGS";
    }
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE 
    std::string get_e_type_as_string(EType e_type)
  {
    EType_t const t = static_cast<EType_t>(e_type);

    return std::string(get_e_type_as_cstr(e_type)) 
      + "@[type_name='" + typeid(t).name() + "', value=" + std::to_string(t) + "]\n";
  }
}