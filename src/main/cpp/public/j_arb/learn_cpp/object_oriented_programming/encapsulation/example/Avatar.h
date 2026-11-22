#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_EXAMPLE_AVATAR_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_EXAMPLE_AVATAR_H

#include <string>
#include <optional>

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation::example
{
  class Avatar
  {
    public: explicit Avatar();
    public: virtual ~Avatar();
    Avatar(const Avatar&) = default;
    Avatar& operator=(const Avatar&) = default;
    Avatar(Avatar&&) noexcept = default;
    Avatar& operator=(Avatar&&) noexcept = default;
    public: explicit Avatar(
      std::optional<std::string> id,
      std::optional<std::string> code_name
    );
    public: virtual void set_id(std::optional<std::string> id);
    public: virtual std::optional<std::string> get_id() const&;
    public: virtual std::optional<std::string> get_id() &&;
    public: std::optional<std::string> get_id() const&& = delete;
    public: virtual void set_code_name(std::optional<std::string> code_name);
    public: virtual std::optional<std::string> get_code_name() const&;
    public: virtual std::optional<std::string> get_code_name() &&;
    public: std::optional<std::string> get_code_name() const&& = delete;
    private: std::string id;
    private: std::string code_name;
  };
}

#endif