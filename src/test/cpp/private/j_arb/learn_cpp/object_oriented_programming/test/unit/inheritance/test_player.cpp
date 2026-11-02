#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_TEST_UNIT_INHERITANCE_TEST_PLAYER_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_TEST_UNIT_INHERITANCE_TEST_PLAYER_H

#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <string_view>
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

  SECTION("MOVE SEMANTIC") {

    object_oriented_programming::
      inheritance::Player player_i("Hunt");

    REQUIRE(std::string_view(player_i.get_name())
      == "Hunt");

    object_oriented_programming::
      inheritance::Player player_ii;

    REQUIRE(std::string_view(player_ii.get_name())
      == "N/a");

    player_i.move_into(player_ii);

    REQUIRE(player_i.get_name() == nullptr);

    REQUIRE(std::string_view(player_ii.get_name())
      == "Hunt");
  }
}

#endif