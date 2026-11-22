#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_EXAMPLE_I_PLAYABLE_CHARACTER_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_EXAMPLE_I_PLAYABLE_CHARACTER_H

#include <string>
#include "j_arb/learn_cpp/object_oriented_programming/export.h"

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation::example_i
{
  class PlayableCharacter
  {
    public: explicit OBJECT_ORIENTED_PROGRAMMING_API
      PlayableCharacter();
    public: virtual OBJECT_ORIENTED_PROGRAMMING_API
      ~PlayableCharacter();

    public: explicit OBJECT_ORIENTED_PROGRAMMING_API
      PlayableCharacter(char const* id_ptr, std::string name = "");

    public: virtual OBJECT_ORIENTED_PROGRAMMING_API
      void set_id(char const* id_ptr);
    //REM: [x] const/non-const lvalue instance can invoke it.
    public: virtual OBJECT_ORIENTED_PROGRAMMING_API
      char const* get_id() const& noexcept;
    //REM: [x] Explicitly prevent const rvalue instance to call it.
    public: [[nodiscard]] virtual 
      // OBJECT_ORIENTED_PROGRAMMING_API
      char const* get_id() const&& noexcept = delete;
    //REM: [x] Explicitly prevent non-const rvalue instance to call it.
    public: [[nodiscard]] virtual 
      // OBJECT_ORIENTED_PROGRAMMING_API 
      char const* get_id() && noexcept = delete;

    public: virtual OBJECT_ORIENTED_PROGRAMMING_API
      void set_name(std::string name) noexcept;
    public: virtual OBJECT_ORIENTED_PROGRAMMING_API
      std::string get_name() const&;
    public: virtual OBJECT_ORIENTED_PROGRAMMING_API
      std::string get_name() && noexcept;

    private: char* m_id_ptr;
    private: std::string m_name;
  };
}

#if defined(OBJECT_ORIENTED_PROGRAMMING_BUILD_HHO)
# include "j_arb/learn_cpp/object_oriented_programming/encapsulation/example_i/PlayableCharacter.cpp"
#endif

#endif