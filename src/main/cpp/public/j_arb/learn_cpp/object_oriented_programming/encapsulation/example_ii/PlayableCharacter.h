#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_EXAMPLE_II_PLAYABLE_CHARACTER_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_EXAMPLE_II_PLAYABLE_CHARACTER_H

#include "j_arb/learn_cpp/object_oriented_programming/export.h"

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation::example_ii
{
  class PlayableCharacter
  {
    public: OBJECT_ORIENTED_PROGRAMMING_API
      explicit PlayableCharacter();
    public: OBJECT_ORIENTED_PROGRAMMING_API
      virtual ~PlayableCharacter();

    public: OBJECT_ORIENTED_PROGRAMMING_API
      explicit PlayableCharacter(
        char const* id_ptr, char const* name_ptr
      );

    public: OBJECT_ORIENTED_PROGRAMMING_API
      virtual void set_id(char const* id_ptr);
    public: OBJECT_ORIENTED_PROGRAMMING_API
      virtual char const* get_id() const& noexcept;
    public: char const* get_id() const&& noexcept = delete;
    public: char const* get_id() && noexcept = delete;

    public: OBJECT_ORIENTED_PROGRAMMING_API
      virtual void set_name(char const* name_ptr);
    public: OBJECT_ORIENTED_PROGRAMMING_API
      virtual char const* get_name() const& noexcept;
    public: char const* get_name() const&& noexcept = delete;
    public: char const* get_name() && noexcept = delete;

    private: char* m_id_ptr;
    private: char* m_name_ptr;
  };
}

#if defined(OBJECT_ORIENTED_PROGRAMMING_BUILD_HHO)
# include "j_arb/learn_cpp/object_oriented_programming/encapsulation/example_ii/PlayableCharacter.cpp"
#endif

#endif