#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_MODEL_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_MODEL_H

#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/Type.h"
#include "j_arb/learn_cpp/object_oriented_programming/export.h"
#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <utility>
#include <typeinfo>

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation
{

  class Model
  {

    public: inline explicit Model() : Model(nullptr, Type::UNKNOWN)
    {

    }

    public: inline explicit Model(
      char const* id, Type const& type
    ) :
      m_id_ptr(new char[std::strlen(id ? id : "N/a") + 1]),
      m_type(type)
    {
      std::strcpy(this->m_id_ptr, id ? id : "N/a");
    }

    protected: inline explicit Model(Model const& other)
    {
      std::size_t len = std::strlen(other.m_id_ptr);
      this->m_id_ptr = new char[len + 1];
      std::strcpy(this->m_id_ptr, other.m_id_ptr);

      this->m_type = other.m_type;
    }

    protected: inline virtual Model& operator=(
      Model const& other
      ) {
      if (this != &other) {
        this->set_id(other.m_id_ptr);
        this->set_type(other.m_type);
      }
      return *this;
    }

    protected: inline explicit Model(Model&& other) noexcept :
      m_id_ptr(other.m_id_ptr),
      m_type(other.m_type)
    {
      other.m_id_ptr = nullptr;
      other.m_type = Type::UNKNOWN;
    }

    protected: inline Model& operator=(
      Model&& other
      ) noexcept
    {
      if (this != &other) {
        delete[] this->m_id_ptr;
        this->m_id_ptr = other.m_id_ptr;
        other.m_id_ptr = nullptr;

        this->m_type = other.m_type;
        other.m_type = Type::UNKNOWN;
      }
      return *this;
    }

    public: virtual ~Model()
    {
      delete[] this->m_id_ptr;
      this->m_id_ptr = nullptr;
    }

    public: virtual Model* clone() const
    {
      return new Model(*this);
    }

    public: virtual void move_into(
      Model& dest
    ) noexcept {
      dest = std::move(*this);
    }

    public: inline virtual void set_id(
      char const* id
    ) {
      if (!id) return;

      if (std::strcmp(id, this->m_id_ptr) == 0)
        return;

      std::size_t len = std::strlen(id);
      char* new_buffer = new char[len + 1];
      std::strcpy(new_buffer, id);

      delete[] this->m_id_ptr;
      this->m_id_ptr = new_buffer;
    }

    public: inline virtual
      char const* get_id() const
    {
      return this->m_id_ptr;
    }

    public: inline virtual void set_type(
      Type const& type
    ) {
      constexpr unsigned int max_valid_type
        = static_cast<unsigned int>(Type::_END);

      if (static_cast<unsigned int>(type)
        >= max_valid_type) return;

      this->m_type = type;
    }

    public: inline virtual Type get_type() const
    {
      return this->m_type;
    }

    public: virtual bool equals(Model const* other) const
    {
      if (other == nullptr) return false;

      if (this == other) return true;

      if (typeid(*this) != typeid(*other)) return false;

      return std::string_view(this->get_id()) == other->get_id()
        && this->get_type() == other->get_type();
    }

    public: virtual std::size_t hash_code() const
    {
      //REM: Identity Hash
      // return reinterpret_cast<std::size_t>(this);

      //REM: Value Hash
      std::size_t type_hash = typeid(*this).hash_code();
      std::size_t id_hash = std::hash<std::string>{}(this->get_id());

      return type_hash
        ^ (id_hash + 0x9e3779b9 + (type_hash << 6)
          + (type_hash >> 2));
    }

    public: virtual std::string to_string() const
    {
      std::ostringstream oss;
      //REM: TODO fix mangle named for GNU/CLANG
      oss << typeid(*this).name()
        << '@' << std::hex << this->hash_code()
        << "[m_id_ptr='" << this->get_id()
        << "', m_type=''" << object_oriented_programming::
        encapsulation::get_type_cstr(this->get_type()) << "'"
        << ']';

      return oss.str();
    }

    public: friend std::ostream& operator<<(
      std::ostream& os, Model const& model)
    {
      os << model.to_string();
      return os;
    }

    public: friend bool operator==(Model const& lhs, Model const& rhs)  
    {
      return lhs.equals(&rhs);
    }

    private: char* m_id_ptr;
    private: Type m_type;
  };
}

#endif