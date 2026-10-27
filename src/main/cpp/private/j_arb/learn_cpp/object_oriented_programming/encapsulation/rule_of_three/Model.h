
#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_RULE_OF_THREE_MODEL_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_RULE_OF_THREE_MODEL_H

#include <memory>
#include <cstring>
#include <typeinfo>
#include <iostream>
#include <sstream>
#include <utility>
#include <algorithm>
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/Type.h"
#include "j_arb/learn_cpp/object_oriented_programming/utility/demangle.h"

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation::rule_of_three
{

  class Model
  {

    public: explicit Model() : Model(
      Model::get_default_id()
    ) {
    }

    public: explicit Model(
      std::unique_ptr<char[]> id_uptr
    ) : Model(std::move(id_uptr), Type::UNKNOWN)
    {
    }

    public: explicit Model(
      std::unique_ptr<char[]> id_uptr,
      Type type
    ) :
      m_id_uptr(std::move(id_uptr)),
      m_type(type)
    {
      if (id_uptr == nullptr) {
        this->m_id_uptr = std::move(Model::get_default_id());
      }
    }

    //REM: [x] Copy ctor
    protected: explicit Model(Model const& other)
    {
      this->set_id(other.m_id_uptr.get());
    }

    protected: explicit Model(Model&& other) noexcept = default;

    //REM: [x] Copy assignment operator
    protected: Model& operator=(Model const& other)
    {
      if (this != &other) {
        this->set_id(other.m_id_uptr.get());
      }
      return *this;
    }

    protected: Model& operator=(Model&& other) noexcept = default;

    //REM: [x] Dtor
    public: virtual ~Model() = default;

    public: virtual Model* clone() const
    {
      return new Model(*this);
    }

    public: virtual void move_into(Model& dest) noexcept
    {
      dest = std::move(*this);
    }

    public: inline void set_id(std::unique_ptr<char[]> id_uptr)
    {
      this->m_id_uptr = std::move(id_uptr);
    }

    public: inline void set_id(std::string_view id)
    {
      this->set_id(id.data());
    }

    public: virtual void set_id(char const* id_ptr)
    {
      if (id_ptr != nullptr) {

        std::size_t len = std::strlen(id_ptr);
        if (len <= 0u)
          return;

        if (std::string_view(id_ptr) == this->get_id())
          return;

        std::size_t size = len + 1;
        std::unique_ptr<char[]> new_buffer_uptr
          = std::make_unique<char[]>(size);

        std::memcpy(new_buffer_uptr.get(), id_ptr, size);
        this->m_id_uptr = std::move(new_buffer_uptr);
      }
      else if (std::string_view(this->get_id())
        != Model::get_default_id().get())
      {
        this->m_id_uptr = std::move(Model::get_default_id());
      }
    }

    public: virtual char const* get_id() const
    {
      return this->m_id_uptr.get();
    }

    public: static std::unique_ptr<char[]> get_default_id()
    {
      return std::unique_ptr<char[]>(
        new char[4] {'N', '/', 'a', '\0'}
      );
    }

    public: virtual void set_type(Type type)
    {
      this->m_type = type;
    }

    public: virtual Type get_type() const
    {
      return this->m_type;
    }

    public: virtual std::size_t hash_code() const 
    {
      std::size_t typeid_hash = typeid(*this).hash_code();
      std::size_t id_hash = std::hash<std::string>{}(
        this->get_id()
      );
      std::size_t type_hash = std::hash<Type>{}(
        this->get_type()
      );

      return typeid_hash 
        ^ (id_hash + type_hash
          + 0x9e3779b9 
          + (typeid_hash << 6) + (typeid_hash >> 2));
    }

    public: virtual bool equals(Model const* other) const
    {
      if (other == nullptr) return false;

      if (this == other) return true;

      if (typeid(*this) != typeid(*other)) return false;

      return std::string_view(this->get_id()) == other->get_id()
        && this->get_type() == other->get_type();
    }

    public: friend bool operator==(
      Model const& lhs, Model const& rhs
      ) {
      return lhs.equals(&rhs);
    }

    public: friend bool operator!=(
      Model const& lhs, Model const& rhs
      ) {
      return !lhs.equals(&rhs);
    }

    public: virtual std::string to_string() const
    {
      std::ostringstream oss;
      // oss << utility::
      //  get_type_name_constexpr<decltype(*this)>()
      //REM: [x] demangle name for GNU/CLANG
      oss << utility::demangle(typeid(*this).name()) 
        << "@"
        << std::hex << this->hash_code()
        << "["
        << "m_id_ptr='" << this->get_id() << "', "
        << "m_type='" << this->get_type() << "'"
        << "]";

      return oss.str();
    }

    public: friend std::ostream& operator<<(
      std::ostream& os, Model const& model
      ) {
      os << model.to_string();
      return os;
    }

    private: std::unique_ptr<char[]> m_id_uptr = nullptr;
    private: Type m_type;
  };
}

#endif