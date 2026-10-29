#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_DEMO_I_ENTITY_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_DEMO_I_ENTITY_H

#include <ostream>
#include <iomanip>
#include <sstream>
#include <string_view>
#include <algorithm>
#include <utility>
#include <typeinfo>
#include <memory>
#include "./Type.h"
#include "j_arb/learn_cpp/object_oriented_programming/utility/demangle.h"

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation::demo_i {

  class Entity {

    public: explicit Entity() :
      Entity(std::string_view{}, Type::UNKNOWN)
    {
    }

    public: explicit Entity(
      std::string_view id_sv, Type type
    ) :
      m_id_ptr(nullptr),
      m_type(Type::UNKNOWN)
    {
      this->set_id(id_sv);
      this->set_type(type);
    }

    public: virtual ~Entity()
    {
      delete[] this->m_id_ptr;
    }

    public: virtual void set_id(
      std::string_view id_sv
    ) {
      if (id_sv.empty()) return;

      if (this->m_id_ptr && id_sv == this->m_id_ptr)
        return;

      std::size_t len = id_sv.size();
      char* buff_ptr = new char[len + 1];
      std::copy_n(
        id_sv.data(), len, buff_ptr
      );
      buff_ptr[len] = '\0';

      delete[] this->m_id_ptr;
      this->m_id_ptr = buff_ptr;
    }

    public: virtual char const* get_id() const noexcept
    {
      return this->m_id_ptr
        ? this->m_id_ptr : "";
    }

    public: virtual void set_type(Type type) noexcept
    {
      if (type >= Type::_END) return;

      this->m_type = type;
    }

    public: virtual Type get_type() const noexcept
    {
      return this->m_type;
    }

    public: virtual bool equals(Entity const& other) const noexcept
    {
      if (this == &other) return true;

      if (typeid(*this) != typeid(other)) return false;

      return std::string_view(this->get_id()) == other.get_id()
        && this->get_type() == other.get_type();
    }

    public: virtual bool operator==(Entity const& other) const
    {
      return this->equals(other);
    }

    public: virtual bool operator!=(Entity const& other) const
    {
      return !this->equals(other);
    }

    public: virtual std::size_t hash_code() const
    {
      std::size_t seed_hash 
        = typeid(*this).hash_code();
      std::size_t id_hash 
        = std::hash<std::string_view>{}(this->get_id());
      std::size_t type_hash 
        = std::hash<Type>{}(this->get_type());

      void (*combine_hash)(std::size_t&, std::size_t) 
        = [](std::size_t& current_seed, std::size_t next_hash) {
          current_seed ^= next_hash 
            + 0x9e37'79b9U
            + (current_seed << 6) + (current_seed >> 2);
        };

      combine_hash(seed_hash, id_hash);
      combine_hash(seed_hash, type_hash);

      return seed_hash ^ 0xb00b'b135U;
    }

    public: virtual std::string to_string() const
    {

      constexpr std::size_t hex_width = sizeof(std::size_t) * 2;

      std::ostringstream oss;
      oss << utility::demangle(typeid(*this).name())
        << "@"
        << std::hex 
        << std::setw(hex_width) << std::setfill('0')
        << this->hash_code()
        << "["
        << "m_id_ptr='" << this->get_id() << "', "
        << "m_type='" << this->get_type() << "'"
        << "]";

      return oss.str();
    }

    public: friend std::ostream& operator<<(std::ostream& os, 
      Entity const& entity) 
    {
      os << entity.to_string();
      return os;
    }

    protected: explicit Entity(Entity const& other) :
      Entity(other.get_id(), other.get_type())
    {
    }

    protected: explicit Entity(Entity&& other) noexcept :
      m_id_ptr(nullptr),
      m_type(Type::UNKNOWN)
    {
      *this = std::move(other);
    }

    protected: virtual Entity& operator=(Entity const& other)
    {
      if (this != &other) {
        this->set_id(other.get_id());
        this->set_type(other.get_type());
      }
      return *this;
    }

    protected: virtual Entity& operator=(Entity&& other) noexcept
    {
      if (this != &other) {
        delete[] this->m_id_ptr;

        this->m_id_ptr = other.m_id_ptr;
        this->m_type = other.m_type;

        other.m_id_ptr = nullptr;
        other.m_type = Type::UNKNOWN;

      }
      return *this;
    }

    public: virtual Entity* cpy() const &
    {
      return new Entity(*this);
    }

    public: virtual Entity&& mv() && noexcept  
    {
      return std::move(*this);
    }

    public: virtual std::unique_ptr<Entity> clone() const&
    {
      return std::unique_ptr<Entity>(new Entity(*this));
    }

    //REM: Usage `std::move(value_instance).move()`
    public: virtual std::unique_ptr<Entity> move()&&
    {
      return std::unique_ptr<Entity>(new Entity(std::move(*this)));
    }

    private: char* m_id_ptr;
    private: Type m_type;
  };
}

#endif