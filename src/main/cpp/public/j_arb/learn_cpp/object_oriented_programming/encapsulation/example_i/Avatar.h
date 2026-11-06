#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_EXAMPLE_I_AVATAR_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_EXAMPLE_I_AVATAR_H

#include "j_arb/learn_cpp/object_oriented_programming/export.h"

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation::example_i
{
  class OBJECT_ORIENTED_PROGRAMMING_API Avatar
  {
    public: explicit Avatar();
    public: explicit Avatar(
      char const* id_ptr, char const* name_ptr
    );
    public: virtual ~Avatar();

    public: explicit Avatar(Avatar const& other);
    public: explicit Avatar(Avatar&& other) noexcept;

    public: virtual Avatar& operator=(Avatar const& other);
    public: virtual Avatar& operator=(Avatar&& other) noexcept;

    public: virtual void set_id(char const* id_ptr);
    public: virtual char const* get_id() const& noexcept;
    public: char const* get_id() const&& noexcept = delete;
    public: char const* get_id() && noexcept = delete;

    public: virtual void set_name(char const* name_ptr);
    public: virtual char const* get_name() const& noexcept;
    public: char const* get_name() const&& noexcept = delete;
    public: char const* get_name() && noexcept = delete;

#   if defined(_MSC_VER)
#     pragma warning(push)
#     pragma warning(disable: 4251)
#   endif
    //REM: Avoid C4251 (unexported-member),
    //REM: Suppress warning or use `Pimpl`.
    // private: std::string m_code_name;
#   if defined(_MSC_VER)
#     pragma warning(pop)
#   endif
    //REM: primitives bypass export warnings
    private: char* m_id_ptr;
    private: char* m_name_ptr;
  };
}

#if defined(OBJECT_ORIENTED_PROGRAMMING_BUILD_HHO)
# include "j_arb/learn_cpp/object_oriented_programming/encapsulation/example_i/Avatar.cpp"
#endif

#endif