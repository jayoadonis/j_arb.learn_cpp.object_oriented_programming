
#include <cstring>
#include <algorithm>
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/example_ii/PlayableCharacter.h"

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation::example_ii
{

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    PlayableCharacter::PlayableCharacter()
    : PlayableCharacter(nullptr, nullptr)
  {
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    PlayableCharacter::~PlayableCharacter()
  {
    delete[] this->m_id_ptr;
    delete[] this->m_name_ptr;
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    PlayableCharacter::PlayableCharacter(
      char const* id_ptr, char const* name_ptr
    ) : m_id_ptr(nullptr), m_name_ptr(nullptr)
  {
    this->set_id(id_ptr);
    this->set_name(name_ptr);
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    void PlayableCharacter::set_id(char const* id_ptr)
  {
    if (id_ptr == nullptr) id_ptr = "N/a";

    if (this->m_id_ptr
      && std::strcmp(this->m_id_ptr, id_ptr) == 0)
    {
      return;
    }

    std::size_t in_len = std::strlen(id_ptr);

    char* out_buff = new char[in_len + 1];
    std::copy_n(id_ptr, in_len, out_buff);
    out_buff[in_len] = '\0';

    delete[] this->m_id_ptr;
    this->m_id_ptr = out_buff;

    return;
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    char const* PlayableCharacter::get_id() const& noexcept
  {
    return this->m_id_ptr;
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    void PlayableCharacter::set_name(char const* name_ptr)
  {
    if(name_ptr == nullptr) name_ptr = "N/a";

    if(
      this->m_name_ptr
      && std::strcmp(this->m_name_ptr, name_ptr) == 0)
    {
      return;
    }

    std::size_t in_len = std::strlen(name_ptr);

    char * out_buff = new char[in_len + 1];
    std::copy_n(name_ptr, in_len, out_buff);
    out_buff[in_len] = '\0';

    delete[] this->m_name_ptr;
    this->m_name_ptr = out_buff;

    return;
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
    char const* PlayableCharacter::get_name() const& noexcept
  {
    return this->m_name_ptr;
  }
}