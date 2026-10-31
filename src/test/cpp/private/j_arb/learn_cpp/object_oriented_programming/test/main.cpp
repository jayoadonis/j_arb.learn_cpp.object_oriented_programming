
#include <catch2/catch_session.hpp>

#include "j_arb/learn_cpp/object_oriented_programming/test/unit/test_i.h"
// #include "j_arb/learn_cpp/object_oriented_programming/test/unit/encapsulation/test_entity.h"
// #include "j_arb/learn_cpp/object_oriented_programming/test/unit/encapsulation/test_model.h"
#include "j_arb/learn_cpp/object_oriented_programming/test/unit/inheritance/test_player.h"

int main(int arg_c, char **arg_v) {

  Catch::Session session;

  int result = session.applyCommandLine(
    arg_c, arg_v
  );

  if(result != 0) return result;

  return session.run();
}