
#include <cstring>
#include <algorithm>
#include <memory>
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/example_ii/PlayableCharacter.h"

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation::example_ii
{

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  OBJECT_ORIENTED_PROGRAMMING_API
    PlayableCharacter::PlayableCharacter()
    : PlayableCharacter(nullptr, nullptr)
  {
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
    PlayableCharacter::PlayableCharacter(
      char const* id_ptr, char const* name_ptr
    ) : m_id_ptr(nullptr), m_name_ptr(nullptr)
  {
    this->set_id(id_ptr);
    this->set_name(name_ptr);
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  OBJECT_ORIENTED_PROGRAMMING_API
    PlayableCharacter::PlayableCharacter(
      PlayableCharacter const& other
    ) : PlayableCharacter(other.get_id(), other.get_name())
  {
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  OBJECT_ORIENTED_PROGRAMMING_API
    PlayableCharacter::PlayableCharacter(
      PlayableCharacter&& other
    ) noexcept :
    m_id_ptr(other.m_id_ptr),
    m_name_ptr(other.m_name_ptr)
  {
    other.m_id_ptr = nullptr;
    other.m_name_ptr = nullptr;
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  OBJECT_ORIENTED_PROGRAMMING_API
    PlayableCharacter& PlayableCharacter::operator=
    (PlayableCharacter const& other)
  {
    if (this == &other) return *this;
    this->set_id(other.get_id());
    this->set_name(other.get_name());
    return *this;
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  OBJECT_ORIENTED_PROGRAMMING_API
    PlayableCharacter& PlayableCharacter::operator=
    (PlayableCharacter&& other) noexcept
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
  OBJECT_ORIENTED_PROGRAMMING_API
    void PlayableCharacter::set_id(char const* id_ptr)
  {
    // if (id_ptr == nullptr) id_ptr = "N/a";
    if (id_ptr == nullptr) return;

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
  OBJECT_ORIENTED_PROGRAMMING_API
    char const* PlayableCharacter::get_id() const& noexcept
  {
    return this->m_id_ptr
      ? this->m_id_ptr : "N/a";
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  OBJECT_ORIENTED_PROGRAMMING_API
    void PlayableCharacter::set_name(char const* name_ptr)
  {
    // if(name_ptr == nullptr) name_ptr = "N/a";
    if (name_ptr == nullptr) return;

    if (
      this->m_name_ptr
      && std::strcmp(this->m_name_ptr, name_ptr) == 0)
    {
      return;
    }

    std::size_t in_len = std::strlen(name_ptr);

    char* out_buff = new char[in_len + 1];
    std::copy_n(name_ptr, in_len, out_buff);
    out_buff[in_len] = '\0';

    delete[] this->m_name_ptr;
    this->m_name_ptr = out_buff;

    return;
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  OBJECT_ORIENTED_PROGRAMMING_API
    char const* PlayableCharacter::get_name() const& noexcept
  {
    return this->m_name_ptr
      ? this->m_name_ptr : "N/a";
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  OBJECT_ORIENTED_PROGRAMMING_API
    std::unique_ptr<PlayableCharacter>
    PlayableCharacter::clone() const&
  {
    return std::unique_ptr<PlayableCharacter>(
      new PlayableCharacter(*this)
    );
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  OBJECT_ORIENTED_PROGRAMMING_API
    std::unique_ptr<PlayableCharacter>
    PlayableCharacter::clone() &&
  {
    return std::unique_ptr<PlayableCharacter>(
      new PlayableCharacter(std::move(*this))
    );
  }
}