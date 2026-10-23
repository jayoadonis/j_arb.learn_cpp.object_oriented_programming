#include "./Entity.h"

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation::rule_of_zero {

  void Entity::set_id(std::string const & id) {
    if(id.empty() || id == this->m_id) {
      return;
    }
    this->m_id = id;
  }

  char const * Entity::get_id() const {

    return this->m_id.c_str();
  }
}