#include "./Entity.h"
#include <cstring>

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation {

  Entity::Entity() : m_id(new char[1]) { 
    this->m_id[0] = '\0';
  }

  Entity::~Entity() {
    delete[] this->m_id;
  }

  Entity* Entity::clone() const {
    return new Entity(*this);
  }

  Entity::Entity(Entity const & other) {
    std::size_t len = std::strlen(other.m_id);
    this->m_id = new char[len+1];
    std::strcpy(this->m_id, other.m_id);
  }

  Entity& Entity::operator=(Entity const & other) {
    if(this != &other) {

      this->set_id(other.m_id);
    }
    return *this;
  }

  Entity::Entity(Entity && other) noexcept : m_id(other.m_id) {
    other.m_id = nullptr;
  }

  Entity& Entity::operator=(Entity && other) noexcept {
    if(this != &other) {
      delete[] this->m_id;
      this->m_id = other.m_id;
      other.m_id = nullptr;
    }
    return *this;
  }

  void Entity::set_id(char const * id) {

    if(!id) return;

    if(std::strcmp(id, this->m_id) == 0) {
      return;
    }

    std::size_t len = std::strlen(id);

    char* new_buffer = new char[len+1];
    std::strcpy(new_buffer, id);

    delete[] this->m_id;
    this->m_id = new_buffer;
  }

  char const * Entity::get_id() const {
    return this->m_id ? this->m_id : "";
  }
}