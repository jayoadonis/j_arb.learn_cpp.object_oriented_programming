
#include <cstdio>
#include <string_view>
#include "j_arb/learn_cpp/object_oriented_programming/App.h"

int main(int arg_c, char **arg_v) {

  using namespace j_arb::learn_cpp;

  object_oriented_programming::App app;

  std::string title("OOP");

  app.set_title(title);
  
  std::fprintf(stdout, "::: %s\n", app.get_title().c_str());

  app.set_title("OOP_II");
  
  std::fprintf(stdout, "::: %s\n", app.get_title().c_str());

  std::fprintf(stdout, "::: %s\n", "Hi there!");
  return 0;
}