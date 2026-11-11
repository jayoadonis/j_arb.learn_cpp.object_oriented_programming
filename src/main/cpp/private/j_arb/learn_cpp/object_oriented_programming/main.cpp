
#include <cstdio>
#include <iostream>
#include <string_view>
#include "j_arb/learn_cpp/object_oriented_programming/App.h"

#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/example_ii/PlayableCharacter.h"

int main(int arg_c, char** arg_v) {

  using namespace j_arb::learn_cpp;

  object_oriented_programming::App app;

  std::string title("OOP");

  app.set_title(title);
  
  std::fprintf(stdout, "::: %s\n", app.get_title().c_str());

  app.set_title("OOP_II");
  
  std::fprintf(stdout, "::: %s\n", app.get_title().c_str());

  std::fprintf(stdout, "::: %s\n", "Hi there!");

  // int max_val = std::numeric_limits<int>::max();
  // //REM: UB (signed-integer-overflow)
  // int overflowed = max_val + 1;
  // (void)overflowed;
  
  //REM: Address HEAP (use-after-free)
  // int* x = new int(1);
  // delete x;
  // //REM: Using volatile (REQUIRED) prevents the compiler from 
  // //REM: optimizing the read away.
  // int volatile y = *x;
  // (void)y;

  object_oriented_programming::encapsulation::
    example_ii::PlayableCharacter p_char;

  std::cout << "<0> " << p_char.get_id() << "\n";
  std::cout << "<0> " << p_char.get_name() << "\n";

  object_oriented_programming::func_app(arg_c, arg_v);


  return 0;
}