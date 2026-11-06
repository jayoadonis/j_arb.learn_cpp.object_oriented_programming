
#include <catch2/catch_test_macros.hpp>
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/example_i/Avatar.h"

TEST_CASE("ENCAPSULATION EXAMPLE_I", "[unit]")
{ 
  using namespace j_arb::learn_cpp::
    object_oriented_programming::encapsulation;

  SECTION("DEFAULT CTOR")
  {
    example_i::Avatar avatar_i;

    REQUIRE(avatar_i.get_id() == nullptr);
    REQUIRE(avatar_i.get_name() == nullptr);

    
    example_i::Avatar avatar_ii{};

    REQUIRE(avatar_ii.get_id() == nullptr);
    REQUIRE(avatar_ii.get_name() == nullptr);
  }

  SECTION("CUSTOM CTOR")
  {
    example_i::Avatar avatar_i(nullptr, nullptr);

    REQUIRE(avatar_i.get_id() == nullptr);
    REQUIRE(avatar_i.get_name() == nullptr);

    example_i::Avatar avatar_ii("ID_123", "name_ii");

    REQUIRE(avatar_ii.get_id() != nullptr);
    REQUIRE(avatar_ii.get_name() != nullptr);
    REQUIRE(
      std::string_view(avatar_ii.get_id()) == "ID_123"
    );
    REQUIRE(
      std::string_view(avatar_ii.get_name()) == "name_ii"
    );

  }

  SECTION("SETTER")
  {
    example_i::Avatar avatar_i;

    REQUIRE(avatar_i.get_id() == nullptr);
    REQUIRE(avatar_i.get_name() == nullptr);

    avatar_i.set_id("ID_123");
    avatar_i.set_name("name_i");

    REQUIRE(avatar_i.get_id() != nullptr);
    REQUIRE(avatar_i.get_name() != nullptr);
    REQUIRE(
      std::string_view(avatar_i.get_id())
      ==
      "ID_123"
    );
    REQUIRE(
      std::string_view(avatar_i.get_name())
      ==
      "name_i"
    );
  }

}