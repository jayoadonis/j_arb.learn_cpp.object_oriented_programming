#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_EXAMPLE_II_PLAYABLE_CHARACTER_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_EXAMPLE_II_PLAYABLE_CHARACTER_H

#include <memory>
#include "j_arb/learn_cpp/object_oriented_programming/export.h"

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation::example_ii
{
  class PlayableCharacter
  {
    public: explicit OBJECT_ORIENTED_PROGRAMMING_API
      PlayableCharacter();
    public: virtual OBJECT_ORIENTED_PROGRAMMING_API
      ~PlayableCharacter();

    public: explicit OBJECT_ORIENTED_PROGRAMMING_API
      PlayableCharacter(
        char const* id_ptr, char const* name_ptr
      );

    protected: explicit OBJECT_ORIENTED_PROGRAMMING_API
      PlayableCharacter(PlayableCharacter const& other);
    protected: explicit OBJECT_ORIENTED_PROGRAMMING_API
      PlayableCharacter(PlayableCharacter&& other) noexcept;

    protected: virtual OBJECT_ORIENTED_PROGRAMMING_API
      PlayableCharacter& operator=
      (PlayableCharacter const& other);
    protected: virtual OBJECT_ORIENTED_PROGRAMMING_API
      PlayableCharacter& operator=
      (PlayableCharacter&& other) noexcept;

    public: virtual OBJECT_ORIENTED_PROGRAMMING_API
      void set_id(char const* id_ptr);
    public: virtual OBJECT_ORIENTED_PROGRAMMING_API
      char const* get_id() const& noexcept;
    public: [[nodiscard]]
      char const* get_id() const&& noexcept = delete;
    public: [[nodiscard]]
      char const* get_id() && noexcept = delete;

    public: virtual OBJECT_ORIENTED_PROGRAMMING_API
      void set_name(char const* name_ptr);
    public: virtual OBJECT_ORIENTED_PROGRAMMING_API
      char const* get_name() const& noexcept;
    public: [[nodiscard]]
      char const* get_name() const&& noexcept = delete;
    public: [[nodiscard]]
      char const* get_name() && noexcept = delete;

    public: virtual OBJECT_ORIENTED_PROGRAMMING_API
      std::unique_ptr<PlayableCharacter> clone() const&;
    public: [[nodiscard]] virtual 
      OBJECT_ORIENTED_PROGRAMMING_API
      std::unique_ptr<PlayableCharacter> clone() &&;

    private: char* m_id_ptr;
    private: char* m_name_ptr;
  };
}

#if defined(OBJECT_ORIENTED_PROGRAMMING_BUILD_HHO)
# include "j_arb/learn_cpp/object_oriented_programming/encapsulation/example_ii/PlayableCharacter.cpp"
#endif

#endif