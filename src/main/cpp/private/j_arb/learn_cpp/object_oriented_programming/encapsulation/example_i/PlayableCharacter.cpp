
#include <cstring>
#include <algorithm>
#include "j_arb/learn_cpp/object_oriented_programming/export.h"
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/example_i/PlayableCharacter.h"

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation::example_i
{
  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    OBJECT_ORIENTED_PROGRAMMING_API
    PlayableCharacter::PlayableCharacter()
    : PlayableCharacter(nullptr, nullptr)
  {
  }
  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    OBJECT_ORIENTED_PROGRAMMING_API
    PlayableCharacter::PlayableCharacter(
      char const* id_ptr, char const* name_ptr [[maybe_unused]]
    ) : m_id_ptr(nullptr), m_name_ptr(nullptr)
  {
    this->set_id(id_ptr);
  }
  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    OBJECT_ORIENTED_PROGRAMMING_API
    PlayableCharacter::~PlayableCharacter()
  {
    delete[] this->m_id_ptr;
    delete[] this->m_name_ptr;
  }
  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    OBJECT_ORIENTED_PROGRAMMING_API
    void PlayableCharacter::set_id(char const* id_ptr)
  {
    if (id_ptr == nullptr) return;

    if (
      this->m_id_ptr
      && std::strcmp(this->m_id_ptr, id_ptr) == 0
      ) return;

    std::size_t new_len = std::strlen(id_ptr);

    char* new_buff = new char[new_len + 1];
    std::copy_n(id_ptr, new_len, new_buff);
    new_buff[new_len] = '\0';

    delete[] this->m_id_ptr;
    this->m_id_ptr = new_buff;

    return;
  }
  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    OBJECT_ORIENTED_PROGRAMMING_API
    char const* PlayableCharacter::get_id() const& noexcept
  {
    return this->m_id_ptr ? this->m_id_ptr : "N/a";
  }
}