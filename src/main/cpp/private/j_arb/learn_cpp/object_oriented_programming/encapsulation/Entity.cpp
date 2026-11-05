#include "./Entity.h"
#include <cstring>

#include "j_arb/learn_cpp/object_oriented_programming/export.h"

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation {

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE 
    Entity::Entity() : m_id_ptr(new char[1]) 
  {
    this->m_id_ptr[0] = '\0';
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE 
    Entity::~Entity() 
  {
    delete[] this->m_id_ptr;
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE 
    Entity* Entity::clone() const 
  {
    return new Entity(*this);
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE 
    Entity::Entity(Entity const& other) 
  {
    std::size_t len = std::strlen(other.m_id_ptr);
    this->m_id_ptr = new char[len + 1];
    std::strcpy(this->m_id_ptr, other.m_id_ptr);
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE 
    Entity& Entity::operator=(Entity const& other) 
  {
    if (this != &other) {

      this->set_id(other.m_id_ptr);
    }
    return *this;
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE 
    Entity::Entity(Entity&& other) noexcept : 
      m_id_ptr(other.m_id_ptr) 
  {
    other.m_id_ptr = nullptr;
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE 
    Entity& Entity::operator=(Entity&& other) noexcept 
  {
    if (this != &other) {
      delete[] this->m_id_ptr;
      this->m_id_ptr = other.m_id_ptr;
      other.m_id_ptr = nullptr;
    }
    return *this;
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE 
    void Entity::set_id(char const* id) 
  {
    if (!id) return;

    if (std::strcmp(id, this->m_id_ptr) == 0) {
      return;
    }

    std::size_t len = std::strlen(id);

    char* new_buffer = new char[len + 1];
    std::strcpy(new_buffer, id);

    delete[] this->m_id_ptr;
    this->m_id_ptr = new_buffer;
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE 
    char const* Entity::get_id() const 
  {
    return this->m_id_ptr ? this->m_id_ptr : "";
  }
}