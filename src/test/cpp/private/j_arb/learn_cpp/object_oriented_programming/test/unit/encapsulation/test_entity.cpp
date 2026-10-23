#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/Entity.h"
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/rule_of_zero/Entity.h"

TEST_CASE("ENCAPSULATION ENTITY", "[unit]") {

  SECTION("I") {

    using namespace j_arb::learn_cpp;

    object_oriented_programming::
      encapsulation::Entity entity_i;

    std::fprintf(stdout, 
      "::: ENCAPSULATION ENTITY, I: '%s'\n", entity_i.get_id());

    REQUIRE(std::strcmp(entity_i.get_id(), "") == 0);

    entity_i.set_id("test_123");

    std::fprintf(stdout, 
      "::: ENCAPSULATION ENTITY, I: '%s'\n", entity_i.get_id());
    REQUIRE(std::strcmp(entity_i.get_id(), "test_123") == 0);
  }

  SECTION("RULE OF ZERO", "[unit]") {

    using namespace j_arb::learn_cpp;

    object_oriented_programming::
      encapsulation::
      rule_of_zero::Entity entity_i;

    std::fprintf(stdout, 
      "::: ENCAPSULATION ENTITY RULE OF ZERO, '%s'\n",
      entity_i.get_id());

    REQUIRE(std::strcmp(entity_i.get_id(), "") == 0);

    entity_i.set_id("test_321");

    std::fprintf(stdout,
      "::: ENCAPSULATION ENTITY RULE OF ZERO, '%s'\n",
      entity_i.get_id());

    REQUIRE(std::strcmp(entity_i.get_id(), "test_321") == 0);
  }
}