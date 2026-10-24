#include <cstring>
#include <catch2/catch_test_macros.hpp>
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/Model.h"

TEST_CASE("ENCAPSULATION MODEL", "[unit]") {

  using namespace j_arb::learn_cpp;

  SECTION("CTOR") {

    object_oriented_programming::
      encapsulation::Model model_i;
    
    REQUIRE(std::strcmp(model_i.get_id(), "N/a") == 0);

    model_i.set_id("model_123");

    REQUIRE(std::strcmp(model_i.get_id(), "model_123") == 0);
  }
}