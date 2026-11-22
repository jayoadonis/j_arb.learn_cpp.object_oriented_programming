#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_EXAMPLE_PLAYABLE_CHARACER_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_EXAMPLE_PLAYABLE_CHARACER_H

#include <cstring>
#include "j_arb/learn_cpp/object_oriented_programming/compiler_warning.h"

OBJECT_ORIENTED_PROGRAMMING_COMPILER_ALLOW_SHADOW_BEGIN
namespace j_arb::learn_cpp::object_oriented_programming::encapsulation::example
{
  class PlayableCharacter
  {
    public: explicit PlayableCharacter() :
      PlayableCharacter(nullptr, nullptr)
    {
    }
    public: virtual ~PlayableCharacter()
    {
      delete[] this->id_ptr;
      delete[] this->code_name_ptr;
    }
    public: explicit PlayableCharacter(
      char const* id_ptr, char const* code_name_ptr
    ) : id_ptr(nullptr), code_name_ptr(nullptr)
    {
      this->set_id(id_ptr);
      this->set_code_name(code_name_ptr);
    }
    public: void set_id(char const* id_ptr)
    {
      if(id_ptr == nullptr) 
      {
        return PlayableCharacter::clear_cstr(this->id_ptr);
      }
      if(this->id_ptr != nullptr
        && std::strcmp(this->id_ptr, id_ptr) == 0
      ) return;

      return PlayableCharacter::filled_cstr(this->id_ptr, id_ptr);
    }
    public: char const* get_id() const& noexcept
    {
      return this->id_ptr;
    }
    public: char const* get_id() && = delete;
    public: char const* get_id() const&& = delete;
    public: void set_code_name(char const* code_name_ptr)
    {
      if(code_name_ptr == nullptr)
      {
        return PlayableCharacter::clear_cstr(this->code_name_ptr);
      }
      if(this->code_name_ptr != nullptr
        && std::strcmp(this->code_name_ptr, code_name_ptr) == 0
      ) return;

      return PlayableCharacter::filled_cstr(this->code_name_ptr, code_name_ptr);
    }
    public: char const* get_code_name() const& noexcept
    {
      return this->code_name_ptr;
    }
    public: char const* get_code_name() && = delete;
    public: char const* get_code_name() const&& = delete;
    private: static void filled_cstr(char*& dst, char const* src) 
    {
      std::size_t len = std::strlen(src);
      std::size_t new_buff_cap = len + 1; //REM: +1, for null-terminator char

      char * new_buff = new char[new_buff_cap];
      std::memcpy(new_buff, src, len); //REM: best practice [1^]
      new_buff[len] = '\0'; //REM: [1] best practice

      delete[] dst;
      dst = new_buff;
      
      return;
    }
    private: static void clear_cstr(char*& ptr) noexcept
    {
      delete[] ptr;
      ptr = nullptr;
      return;
    }
    private: char* id_ptr;
    private: char* code_name_ptr;
  };
}
OBJECT_ORIENTED_PROGRAMMING_COMPILER_ALLOW_SHADOW_END

#endif