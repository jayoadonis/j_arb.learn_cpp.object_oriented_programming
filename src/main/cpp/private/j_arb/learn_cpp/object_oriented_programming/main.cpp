
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

  //REM: BUG UBSAN
  // int max_val = std::numeric_limits<int>::max();
  // //REM: UB (signed-integer-overflow)
  // int overflowed = max_val + 1;
  // (void)overflowed;

  // int* arr = new int[4];
  // arr[0] = 10;
  // //REM: BUG ASAN: Out-of-bounds write (index 4 is past the 4-element array)
  // arr[4] = 99;
  // delete[] arr;

  // int* ptr = new int(42);
  // delete ptr;
  // //REM: BUG ASAN: Reading memory that has already been freed
  // int leaked_val = *ptr;
  // std::cout << "[2] Survived heap-use-after-free! Value: " << leaked_val << "\n";
  
  // int* null_ptr = nullptr;
  // //REM: BUG ASAN: Fatal OS access violation (unrecoverable even with halt_on_error=0)
  // *null_ptr = 123;

  object_oriented_programming::encapsulation::
    example_ii::PlayableCharacter p_char;

  std::cout << "<0> " << p_char.get_id() << "\n";
  std::cout << "<0> " << p_char.get_name() << "\n";

  object_oriented_programming::func_app(arg_c, arg_v);


  return 0;
}