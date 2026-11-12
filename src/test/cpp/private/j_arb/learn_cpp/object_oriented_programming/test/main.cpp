
#include <catch2/catch_session.hpp>
#include "j_arb/learn_cpp/object_oriented_programming/sanitizer_hook.h"

int main(int arg_c, char **arg_v) {

  Catch::Session session;

  int result = session.applyCommandLine(
    arg_c, arg_v
  );

  if(result != 0) return result;

  return session.run();
}