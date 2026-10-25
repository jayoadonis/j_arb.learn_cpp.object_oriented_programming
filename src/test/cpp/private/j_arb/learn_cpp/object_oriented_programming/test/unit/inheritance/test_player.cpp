#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include "j_arb/learn_cpp/object_oriented_programming/inheritance/Player.h"

TEST_CASE("INHERITANCE PLAYER", "[unit]") {

  using namespace j_arb::learn_cpp;

  SECTION("CTOR") {

    object_oriented_programming::
      inheritance::Player player_i;

    REQUIRE(std::strcmp(player_i.get_name(), "N/a") == 0);
    REQUIRE(std::strcmp(player_i.get_id(), "player_123") == 0);
    REQUIRE(
      player_i.get_type()
      ==
      object_oriented_programming::
        encapsulation::Type::ACTOR
    );
  }
}