
#include "j_arb/learn_cpp/object_oriented_programming/App.h"

namespace j_arb::learn_cpp::object_oriented_programming {

  App::App() : m_title("N/a") {

  }

  App::~App() {

  }

  void App::set_title(std::string const& title) {

    this->set_title(title.c_str());
  }
  
  void App::set_title(char const* title) {

    this->m_title = title;
  }

  std::string App::get_title() const {

    return this->m_title;
  }
}