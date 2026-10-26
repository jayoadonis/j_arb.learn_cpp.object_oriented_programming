#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_MODEL_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_MODEL_H

#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/Type.h"
#include "j_arb/learn_cpp/object_oriented_programming/export.h"
#include <cstring>
#include <utility>

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation 
{

  class Model 
  {

    public: inline explicit Model() : Model("N/a", Type::UNKNOWN)
    {

    }

    public: inline explicit Model(
      char const* id, Type const& type
    ) : 
      m_id_ptr(new char[std::strlen(id) + 1]),
      m_type(type)
    {
      std::strcpy(this->m_id_ptr, id);
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
      Model & dest
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

      if(static_cast<unsigned int>(type) 
          >= max_valid_type) return;

      this->m_type = type;
    }

    public: inline virtual Type get_type() const 
    {
      return this->m_type;
    }

    private: char* m_id_ptr;
    private: Type m_type;
  };
}

#endif