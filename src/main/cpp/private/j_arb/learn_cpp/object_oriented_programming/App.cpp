
#include <iostream>
#include "j_arb/learn_cpp/object_oriented_programming/export.h"
#include "j_arb/learn_cpp/object_oriented_programming/App.h"
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/example_ii/PlayableCharacter.h"

namespace j_arb::learn_cpp::object_oriented_programming {

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  OBJECT_ORIENTED_PROGRAMMING_API   
    App::App() : m_title("N/a") 
  {
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  OBJECT_ORIENTED_PROGRAMMING_API   
    App::~App() = default;

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  OBJECT_ORIENTED_PROGRAMMING_API   
    void App::set_title(std::string const& title) 
  {
    this->set_title(title.c_str());
  }
  
  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  OBJECT_ORIENTED_PROGRAMMING_API   
    void App::set_title(char const* title) 
  {
    this->m_title = title;
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  OBJECT_ORIENTED_PROGRAMMING_API   
    std::string App::get_title() const 
  {
    
    object_oriented_programming::encapsulation::
      example_ii::PlayableCharacter p_char;

    std::cout << "<1> " << p_char.get_id() << "\n";
    std::cout << "<1> " << p_char.get_name() << "\n";

    return this->m_title;
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE
  OBJECT_ORIENTED_PROGRAMMING_API
  void func_app(int arg_c, char** arg_v)
  {
    std::cout << "<3> " << arg_c << "\n";
    std::cout << "<3> " << (
      arg_v 
        ? arg_v[0] ? arg_v[0] : "--nope--" 
        : "--none--"
    ) << "\n";
  }
}