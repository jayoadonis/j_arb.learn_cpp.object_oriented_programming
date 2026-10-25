#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_MODEL_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_MODEL_H

#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/Type.h"
#include "j_arb/learn_cpp/object_oriented_programming/export.h"
#include <cstring>

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation {

    class Model {

      public: inline
        Model() : Model("N/a") {

        }

      public: inline
        Model(char const * id) 
          : m_id(new char[std::strlen(id)+1]),
          m_type(Type::UNKNOWN) 
        {
          std::strcpy(this->m_id, id);
        }

      protected: inline
        Model(Model const & other) {
          std::size_t len = std::strlen(other.m_id);
          this->m_id = new char[len+1];
          std::strcpy(this->m_id, other.m_id);
        }

      protected: inline
        virtual Model& operator=(Model const & other) {
          if(this != &other) {
            this->set_id(other.m_id);
          }
          return *this;
        }

      protected: inline
        Model(Model && other) noexcept : m_id(other.m_id) {
          other.m_id = nullptr;
        }

      protected: inline
        Model& operator=(Model && other) noexcept {
          if(this != &other) {
            delete[] this->m_id;
            this->m_id = other.m_id;
            other.m_id = nullptr;
          }
          return *this;
        }

      public: inline
        virtual void set_id(char const * id) {
          if(!id) return;

          if(std::strcmp(id, this->m_id) == 0)
            return;

          std::size_t len = std::strlen(id);
          char * new_buffer = new char[len+1];
          std::strcpy(new_buffer, id);

          delete[] this->m_id;
          this->m_id = new_buffer;
        }

      public: inline
        virtual char const * get_id() const {
          return this->m_id;
        }

      public: inline
        virtual void set_type(Type const & type) {
          this->m_type = type;
        }

      public: inline
        virtual Type get_type() const {
          return this->m_type;
        }

      private: char * m_id;
      private: Type m_type;
    };
}

#endif