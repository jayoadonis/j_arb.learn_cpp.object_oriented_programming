
#include "j_arb/learn_cpp/object_oriented_programming/App.h"

namespace j_arb::learn_cpp::object_oriented_programming {

  App::App() : title_m("N/a") {

  }

  App::~App() {

  }

  void App::set_title(std::string const& title) {

    this->set_title(title.c_str());
  }
  
  void App::set_title(char const* title) {

    this->title_m = title;
  }

  std::string App::get_title() const {

    return this->title_m;
  }
}