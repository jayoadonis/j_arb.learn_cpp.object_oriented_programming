#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ABSTRACTION_DEMO_I_OBJECT_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ABSTRACTION_DEMO_I_OBJECT_H

#include <memory>
#include <cstddef>
#include <typeinfo>
#include <string>
#include <sstream>
#include <iomanip>

#include "j_arb/learn_cpp/object_oriented_programming/abstraction/demo_i/ICloneable.h"
#include "j_arb/learn_cpp/object_oriented_programming/abstraction/demo_i/IMoveable.h"
#include "j_arb/learn_cpp/object_oriented_programming/utility/demangle.h"

namespace j_arb::learn_cpp::object_oriented_programming::abstraction::demo_i {

  class Object :
    public ICloneable<Object>,
    public IMoveable<Object>
  {
    public: explicit Object() = default;
    public: virtual ~Object() override = default;

    protected: explicit Object(Object const& other) = default;
    protected: explicit Object(Object&& other) noexcept = default;

    protected: Object& operator=(Object const& other) = default;
    protected: Object& operator=(Object&& other) noexcept = default;

    public: virtual bool equals(Object const& other) const
    {
      return this == &other;
    }

    public: bool operator==(Object const& other) const
    {
      return this->equals(other);
    }

    public: bool operator!=(Object const& other) const
    {
      return !this->equals(other);
    }

    public: virtual std::size_t hash_code() const
    {
      // return typeid(*this).hash_code();
      return std::hash<const void*>{}(
        static_cast<const void*>(this)
        );
    }

    public: virtual std::string to_string() const
    {
      std::ostringstream oss;
      oss << utility::demangle(typeid(*this).name())
        << "@"
        << std::hex
        << std::setw(sizeof(std::size_t) * 2)
        << std::setfill('0')
        << this->hash_code();

      return oss.str();
    }

    public: friend std::ostream& operator<<(
      std::ostream& os, Object const& other
      )
    {
      os << other.to_string();
      return os;
    }
  };

}

#endif
