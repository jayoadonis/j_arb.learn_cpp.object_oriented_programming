#include <catch2/catch_test_macros.hpp>

#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/example/PlayableCharacter.h"
#include <iostream>
#include <limits>


TEST_CASE("ENCAPSULATION EXAMPLE PLAYABLE_CHARACTER", "[unit]")
{
  using namespace j_arb::learn_cpp::object_oriented_programming::
    encapsulation::example;

  SECTION("DEFAULT CTOR")
  {
    PlayableCharacter p_char;

    REQUIRE(p_char.get_id() == nullptr);
    REQUIRE(p_char.get_code_name() == nullptr);

    p_char.set_id("ID_123");

    REQUIRE(p_char.get_id() != nullptr);
    CHECK(std::string_view(p_char.get_id()) == "ID_123");
    REQUIRE(p_char.get_code_name() == nullptr);

    p_char.set_code_name("baker");

    REQUIRE(p_char.get_id() != nullptr);
    CHECK(std::string_view(p_char.get_id()) == "ID_123");
    REQUIRE(p_char.get_code_name() != nullptr);
    CHECK(std::string_view(p_char.get_code_name()) == "baker");

    p_char.set_id(nullptr);

    REQUIRE(p_char.get_id() == nullptr);
    REQUIRE(p_char.get_code_name() != nullptr);
    CHECK(std::string_view(p_char.get_code_name()) == "baker");

    p_char.set_code_name(nullptr);

    REQUIRE(p_char.get_id() == nullptr);
    REQUIRE(p_char.get_code_name() == nullptr);
  }

  SECTION("CUSTOM CTOR")
  {
    PlayableCharacter p_char(nullptr, nullptr);

    REQUIRE(p_char.get_id() == nullptr);
    REQUIRE(p_char.get_code_name() == nullptr);

    PlayableCharacter p_char_i("ID_123", nullptr);

    REQUIRE(p_char_i.get_id() != nullptr);
    CHECK(std::string_view(p_char_i.get_id()) == "ID_123");
    REQUIRE(p_char_i.get_code_name() == nullptr);

    PlayableCharacter p_char_ii(nullptr, "baker");

    REQUIRE(p_char_ii.get_id() == nullptr);
    REQUIRE(p_char_ii.get_code_name() != nullptr);
    CHECK(std::string_view(p_char_ii.get_code_name()) == "baker");

    PlayableCharacter p_char_iii("ID_123", "baker");

    REQUIRE(p_char_iii.get_id() != nullptr);
    CHECK(std::string_view(p_char_iii.get_id()) == "ID_123");
    REQUIRE(p_char_iii.get_code_name() != nullptr);
    CHECK(std::string_view(p_char_iii.get_code_name()) == "baker");
  }

  SECTION("LIFETIME WITH GETTER")
  {
    std::string owning_id;
    std::string owning_code_name;
    {
      PlayableCharacter p_char("ID_123", "baker");
      owning_id = p_char.get_id();
      owning_code_name = p_char.get_code_name();
    }
    CHECK(owning_id == "ID_123");
    CHECK(owning_code_name == "baker");
  }
}

TEST_CASE("ENCAPSULATION EXAMPLE PLAYABLE_CHARACTER WITH SAN", "[.unit]")
{
  WARN("Hidden test, require sanitizer flags enabled.");

  using namespace j_arb::learn_cpp::object_oriented_programming::
    encapsulation::example;

  SECTION("UB LIFETIME WITH GETTER")
  {
    int k = std::numeric_limits<int>::max();
    k += 1; //REM: signed interger overflow
    std::cout << k << "\n";

    char const* observer_id;
    char const* observer_code_name;
    {
      PlayableCharacter p_char("ID_123", "baker");
      observer_id = p_char.get_id();
      observer_code_name = p_char.get_code_name();
    } //REM: p_char freed
    //REM: IMPORTANT; Need compiler sanitizer flags
    //REM: TODO; Try use GoogleTest with `EXPECT_DEATH`
    volatile char use_after_free_read_id
      = observer_id[0]; 
    (void)use_after_free_read_id;
    volatile char use_after_free_read_code_name
      = observer_code_name[0];
    (void)use_after_free_read_code_name;
  }
}