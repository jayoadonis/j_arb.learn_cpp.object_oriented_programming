
#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_RULE_OF_THREE_MODEL_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_RULE_OF_THREE_MODEL_H

#include <memory>
#include <cstring>

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation::rule_of_three 
{

  class Model 
  {

    public: explicit Model() : m_id_uptr(nullptr) 
    {
      this->set_id_from_raw("N/a");
    }

    public: explicit Model(std::unique_ptr<char[]> id_uptr)
      : m_id_uptr(std::move(id_uptr)) 
    {
    }

    //REM: [x] Copy ctor
    protected: explicit Model(Model const& other) 
    {
      if (other.m_id_uptr) {
        this->set_id_from_raw(other.m_id_uptr.get());
      }
      else {
        this->m_id_uptr = nullptr;
      }
    }

    protected: explicit Model(Model&& other) noexcept = default;

    //REM: [x] Copy assignment operator
    protected: Model& operator=(Model const& other) 
    {
      if (this != &other) {
        if (other.m_id_uptr) {
          this->set_id_from_raw(other.m_id_uptr.get());
        }
        else {
          this->m_id_uptr = nullptr;
        }
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

    public: virtual void set_id(std::unique_ptr<char[]> id_uptr) 
    {
      this->m_id_uptr = std::move(id_uptr);
    }

    public: void set_id_from_raw(const char* id_ptr) 
    {
      if (id_ptr) {
        std::size_t size = std::strlen(id_ptr) + 1;
        std::unique_ptr<char[]> copy 
          = std::make_unique<char[]>(size);
        std::memcpy(copy.get(), id_ptr, size);
        this->m_id_uptr = std::move(copy);
      }
      else {
        this->m_id_uptr = nullptr;
      }
    }

    public: virtual char const* get_id() const 
    {
      return this->m_id_uptr ? this->m_id_uptr.get() : "";
    }

    private: std::unique_ptr<char[]> m_id_uptr; 
  };
}


#endif