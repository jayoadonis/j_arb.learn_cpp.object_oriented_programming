#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
#include "j_arb/learn_cpp/object_oriented_programming/export.h"
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/example_i/Avatar.h"

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation::example_i
{
  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    Avatar::Avatar() : Avatar(nullptr, nullptr)
  {
  }
  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    Avatar::Avatar(
      char const* id_ptr, char const* name_ptr
    ) : m_id_ptr(nullptr), m_name_ptr(nullptr)
  {
    this->set_id(id_ptr);
    this->set_name(name_ptr);
  }
  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    Avatar::~Avatar()
  {
    delete[] this->m_id_ptr;
    delete[] this->m_name_ptr;
  }
  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    Avatar::Avatar(Avatar const& other) :
    Avatar(other.get_id(), other.get_name())
  {
  }
  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    Avatar::Avatar(Avatar&& other) noexcept :
    m_id_ptr(other.m_id_ptr),
    m_name_ptr(other.m_name_ptr)
  {
    other.m_id_ptr = nullptr;
    other.m_name_ptr = nullptr;
  }
  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    Avatar& Avatar::operator=(Avatar const& other)
  {
    if (this == &other) return *this;
    this->set_id(other.get_id());
    this->set_name(other.get_name());
    return *this;
  }
  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    Avatar& Avatar::operator=(Avatar&& other) noexcept
  {
    if (this == &other) return *this;

    delete[] this->m_id_ptr;
    this->m_id_ptr = other.m_id_ptr;
    other.m_id_ptr = nullptr;

    delete[] this->m_name_ptr;
    this->m_name_ptr = other.m_name_ptr;
    other.m_name_ptr = nullptr;

    return *this;
  }
  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    void Avatar::set_id(char const* id_ptr)
  {
    if (id_ptr == nullptr) return;

    if (
      this->m_id_ptr
      && std::strcmp(id_ptr, this->m_id_ptr) == 0
      ) {
      return;
    }

    std::size_t rhs_len = std::strlen(id_ptr);

    char* new_buff = new char[rhs_len + 1];
    std::copy_n(id_ptr, rhs_len, new_buff);
    new_buff[rhs_len] = '\0';

    delete[] this->m_id_ptr;
    this->m_id_ptr = new_buff;
  }
  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    char const* Avatar::get_id() const& noexcept
  {
    std::cout << ">>>>> ok" << "\n";
    return this->m_id_ptr;
  }
  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    void Avatar::set_name(char const* name_ptr)
  {
    if (name_ptr == nullptr) return;

    if (
      this->m_name_ptr
      && std::strcmp(name_ptr, this->m_name_ptr) == 0
      ) {
      return;
    }

    std::size_t rhs_len = std::strlen(name_ptr);

    char* new_buff = new char[rhs_len + 1];
    std::copy_n(name_ptr, rhs_len, new_buff);
    new_buff[rhs_len] = '\0';

    delete[] this->m_name_ptr;
    this->m_name_ptr = new_buff;
  }
  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    char const* Avatar::get_name() const& noexcept
  {
    return this->m_name_ptr;
  }
}