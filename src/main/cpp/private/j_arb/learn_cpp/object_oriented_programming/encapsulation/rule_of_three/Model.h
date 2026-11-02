
#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_RULE_OF_THREE_MODEL_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_RULE_OF_THREE_MODEL_H


#include <memory>
#include <cstring>
#include <typeinfo>
#include <ostream>
#include <iomanip>
#include <sstream>
#include <utility>
#include <algorithm>
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/Type.h"
#include "j_arb/learn_cpp/object_oriented_programming/utility/demangle.h"

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation::rule_of_three
{

  class Model {

    public: explicit Model() :
      Model("N/a")
    {
    }

    public: explicit Model(std::string id) :
      Model(std::move(id), Type::UNKNOWN)
    {
    }

    public: explicit Model(std::string id, Type type)
      // :
      // m_id(id.empty() ? "N/a" : std::move(id)),
      // m_type(type >= Type::_END ? Type::UNKNOWN : type)
      //REM: or default
      // m_id("N/a"),
      // m_type(Type::UNKNOWN)
    {
      this->set_id(std::move(id));
      this->set_type(type);
    }

    public: Model(Model const& other) :
      Model(other.m_id, other.m_type)
    {
    }

    public: Model(Model&& other) noexcept = delete;
    //   : 
    //   m_id(std::move(other.m_id)), 
    //   m_type(other.m_type) 
    // {
    //   other.m_id = "N/a";
    //   other.m_type = Type::UNKNOWN;
    // }

    public: Model& operator=(Model const& other)
    {
      if (this != &other) {
        this->set_id(other.m_id);
        this->set_type(other.m_type);
      }
      return *this;
    }

    public: Model& operator=(Model&& other) noexcept = delete;
    // {
    //   if (this != &other) {
    //     m_id = std::move(other.m_id);
    //     other.m_id = "N/a";

    //     m_type = other.m_type;
    //     other.m_type = Type::UNKNOWN;
    //   }
    //   return *this;
    // }

    public: virtual ~Model() = default;

    //REM: TODO with smartpointers as return type 'std::unique_ptr<Model>'
    public: [[nodiscard]] 
      virtual Model* clone() const&
    {
      return new Model(*this);
    }

    // public: virtual void move_into(Model& dest) noexcept 
    // {
    //   dest = std::move(*this);
    // }

    public: virtual void set_id(std::string id)
    {
      this->set_id(id.c_str());
    }

    public: virtual void set_id(std::string_view id)
    {
      this->set_id(id.data());
    }

    public: virtual void set_id(char const* id_ptr)
    {
      if (id_ptr == nullptr || *id_ptr == '\0')
      {
        this->m_id = "N/a";
      }
      else {
        this->m_id = id_ptr;
      }
    }

    public: virtual char const* get_id() const
    {
      return this->m_id.c_str();
    }

    public: virtual void set_type(Type type)
    {
      if (type >= Type::_END) return;
      this->m_type = type;
    }

    public: [[nodiscard]] 
      virtual Type get_type() const
    {
      return this->m_type;
    }

    public: [[nodiscard]] 
      virtual std::size_t hash_code() const
    {
      std::size_t typeid_hash 
        = typeid(*this).hash_code();
      const std::size_t id_hash 
        = std::hash<std::string>{}(this->m_id);
      const std::size_t type_hash 
        = std::hash<int>{}(static_cast<int>(this->m_type));

      typeid_hash
        ^= (id_hash + 0x9e3779b9 + (typeid_hash << 6) + (typeid_hash >> 2));
      typeid_hash
        ^= (type_hash + 0x9e3779b9 + (typeid_hash << 6) + (typeid_hash >> 2));

      return typeid_hash;
    }

    public: [[nodiscard]] virtual bool equals(
      Model const* other
    ) const {
      if (other == nullptr) return false;
      if (this == other) return true;
      if (typeid(*this) != typeid(*other)) return false;

      return this->m_id == other->m_id 
        && this->m_type == other->m_type;
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

    public: [[nodiscard]] 
      virtual std::string to_string() const
    {
      std::ostringstream oss;
      oss << utility::demangle(typeid(*this).name())
        << "@" 
        << std::hex 
        << std::setw(sizeof(std::size_t)*2)
        << std::setfill('0')
        << this->hash_code() 
        << "["
        << "m_id='" << this->get_id() << "', "
        << "m_type='" << this->get_type() << "'"
        << "]";
      return oss.str();
    }

    public: friend std::ostream& operator<<(
      std::ostream& os,
      Model const& model
      ) {
      os << model.to_string();
      return os;
    }

    private: std::string m_id;
    private: Type m_type;
  };
}

#endif