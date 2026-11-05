#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ABSTRACTION_DEMO_I_MODEL_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ABSTRACTION_DEMO_I_MODEL_H

#include <algorithm>
#include <stdexcept>

#include "j_arb/learn_cpp/object_oriented_programming/abstraction/demo_i/Object.h"

namespace j_arb::learn_cpp::object_oriented_programming::abstraction::demo_i 
{

  class Model : public Object 
  {
    public: explicit Model() : Model("")
    {
    }

    public: explicit Model(std::string_view id_sv) : 
      // Object(),
      m_id_ptr(nullptr)
    {
      this->set_id(id_sv);
    }

    public: virtual ~Model() override
    {
      delete[] this->m_id_ptr;
    }

    protected: explicit Model(Model const & other) :
      // Object(other),
      m_id_ptr(nullptr)
    {
      this->set_id(other.get_id());
    }

    protected: Model& operator=(Model const & other) 
    {
      if(this != &other)
      {
        // Object::operator=(other);
        this->set_id(other.get_id());
      }
      return *this;
    }

    protected: explicit Model(Model && other) noexcept :
      // Object(std::move(other)),
      m_id_ptr(other.m_id_ptr)
    {
      other.m_id_ptr = nullptr;
    }

    protected: Model& operator=(Model && other) noexcept
    {
      if(this != &other)
      {
        // Object::operator=(std::move(other));

        delete[] this->m_id_ptr;
        this->m_id_ptr = other.m_id_ptr;
        other.m_id_ptr = nullptr;
      }
      return *this;
    }

    public: virtual void set_id(
      std::string_view id_sv
    ) {
      if(this->get_id() == id_sv) return;

      std::size_t len = id_sv.size();
      char * next_buff_ptr = new char[len + 1];
      if(len > 0 && id_sv.data() != nullptr && id_sv.data()[0] != '\0')
      {
        std::copy_n(
          id_sv.data(),
          len,
          next_buff_ptr
        );
      }
      next_buff_ptr[len] = '\0';

      delete[] this->m_id_ptr;
      this->m_id_ptr = next_buff_ptr;
    }

    public: virtual char const * try_get_id() const
    {
      return this->m_id_ptr
        ? this->m_id_ptr
        : throw std::runtime_error("Null Pointer Exception");
    }

    public: virtual char const* get_id() const noexcept
    {
      return this->m_id_ptr 
        ? this->m_id_ptr : "";
    }

    private: char * m_id_ptr;
  };
}

#endif