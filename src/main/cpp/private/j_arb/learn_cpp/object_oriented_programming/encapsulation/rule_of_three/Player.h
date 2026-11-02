#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_RULE_OF_THREE_PLAYER_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_ENCAPSULATION_RULE_OF_THREE_PLAYER_H

#include "./Model.h"
#include <memory>
#include <string_view>

namespace j_arb::learn_cpp::object_oriented_programming::encapsulation::rule_of_three {

  class Player : public Model {

    public: explicit Player() : Player("p-123") { }

    public: explicit Player(std::string_view id) :
      Player(id, "cn-123") 
    {
    }
    
    public: explicit Player(
      std::string_view id, 
      std::string_view code_name
    ) :
      Model(std::move(std::string(id.data(), id.size())))
    {
      this->set_code_name(code_name);
    }

    public: virtual ~Player() override = default;

    public: virtual std::string to_string() const override
    {
      std::ostringstream oss;
      oss << this->Model::to_string()
        << "@@@"
        << "["
        << "code_name='" << this->get_code_name() << ", "
        << "]";
      return oss.str();
    }

    public: virtual void set_code_name(
      std::string_view id
    ) {
      this->m_code_name = id;
    }

    public: virtual std::string get_code_name() const
    {
      return this->m_code_name;
    }

    private: std::string m_code_name;
  };
}

#endif