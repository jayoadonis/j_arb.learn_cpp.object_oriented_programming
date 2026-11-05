#include "j_arb/learn_cpp/object_oriented_programming/export.h"
#include "j_arb/learn_cpp/object_oriented_programming/App.h"

namespace j_arb::learn_cpp::object_oriented_programming {

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE   
    App::App() : m_title("N/a") 
  {
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE   
    App::~App() = default;

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE   
    void App::set_title(std::string const& title) 
  {
    this->set_title(title.c_str());
  }
  
  OBJECT_ORIENTED_PROGRAMMING_API_INLINE   
    void App::set_title(char const* title) 
  {
    this->m_title = title;
  }

  OBJECT_ORIENTED_PROGRAMMING_API_INLINE   
    std::string App::get_title() const 
  {
    return this->m_title;
  }
}