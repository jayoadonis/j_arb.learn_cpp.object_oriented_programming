#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_EXAMPLE_NON_PLAYABLE_CHARACTER_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_EXAMPLE_NON_PLAYABLE_CHARACTER_H

#include <string>

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation::example
{
  class NonPlayableCharacter
  {
    public: explicit NonPlayableCharacter()
    {
    }
    public: virtual ~NonPlayableCharacter() = default;
    public: template<typename T, typename U>
      explicit NonPlayableCharacter(
        T&& id, T&& code_name
      )
    {
      this->set_id(std::forward<T>(id));
      this->set_code_name(std::forward<U>(code_name));
    }
    public: void set_id(char const* id)
    {
      if (id == nullptr || id[0] == '\0')
      {
        this->id.clear();
        return;
      }
      this->set_id(static_cast<std::string_view>(id));
      return;
    }
    public: void set_id(std::string_view id)
    {
      if(id.data() == nullptr || id.size() == 0)
      {
        this->id.clear();
        return;
      }
      if (this->id == id) return;
      this->id = id;
      return;
    }
    public: std::string get_id() const&
    {
      return this->id;
    }
    public: std::string get_id()&&
    {
      return std::move(this->id);
    }
    public: std::string get_id() const&& = delete;
    public: void set_code_name(char const* code_name)
    {
      if(code_name == nullptr || code_name[0] == '\0')
      {
        this->code_name.clear();
        return;
      }
      this->set_code_name(static_cast<std::string_view>(code_name));
      return;
    }
    public: void set_code_name(std::string_view code_name)
    {
      if(code_name.data() == nullptr || code_name.size() == 0)
      {
        this->code_name.clear();
        return;
      }
      if (this->code_name == code_name) return;
      this->code_name = code_name;
      return;
    }
    public: std::string get_code_name() const&
    {
      return this->code_name;
    }
    public: std::string get_code_name()&&
    {
      return std::move(this->code_name);
    }
    public: std::string get_code_name() const&& = delete;
    private: std::string id;
    private: std::string code_name;
  };
}

#endif