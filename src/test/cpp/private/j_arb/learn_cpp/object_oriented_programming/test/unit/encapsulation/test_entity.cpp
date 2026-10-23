#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/Entity.h"

TEST_CASE("TEST ENTITY", "[unit]") {

  SECTION("I") {

    using namespace j_arb::learn_cpp;

    object_oriented_programming::
      encapsulation::Entity entity_i;

    std::fprintf(stdout, 
      "::: TEST ENTITY, I: '%s'\n", entity_i.get_id());

    REQUIRE(std::strcmp(entity_i.get_id(), "") == 0);

    entity_i.set_id("test_123");

    std::fprintf(stdout, 
      "::: TEST ENTITY, I: '%s'\n", entity_i.get_id());
    REQUIRE(std::strcmp(entity_i.get_id(), "test_123") == 0);
  }
}