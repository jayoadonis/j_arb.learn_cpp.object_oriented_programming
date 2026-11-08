
#include <string_view>
#include <catch2/catch_test_macros.hpp>
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/example_ii/PlayableCharacter.h"

TEST_CASE(
  "ENCAPSULATION EXAMPLE_II PLAYABLE_CHARACTER", "[unit]")
{
  using namespace j_arb::learn_cpp::
    object_oriented_programming::encapsulation;
    
  SECTION("DEFAULT CTOR")
  {
    example_ii::PlayableCharacter p_char;

    REQUIRE(p_char.get_id() != nullptr);
    REQUIRE(
      std::string_view(p_char.get_id())
      == "N/a"
    );

    REQUIRE(p_char.get_name() != nullptr);
    REQUIRE(
      std::string_view(p_char.get_name())
      == "N/a"
    );
  }

  SECTION("CUSTOM CTOR")
  {
    example_ii::PlayableCharacter 
      p_char("ID_123", "code_name_i");

    REQUIRE(p_char.get_id() != nullptr);
    REQUIRE(
      std::string_view(p_char.get_id())
      == "ID_123"
    );

    REQUIRE(p_char.get_name() != nullptr);
    REQUIRE(
      std::string_view(p_char.get_name())
      == "code_name_i"
    );
  }
    
  SECTION("SETTER")
  {
    example_ii::PlayableCharacter p_char;

    p_char.set_id("ID_321");

    REQUIRE(p_char.get_id() != nullptr);
    REQUIRE(
      std::string_view(p_char.get_id())
      == "ID_321"
    );

    p_char.set_id("ID_543");

    REQUIRE(p_char.get_id() != nullptr);
    REQUIRE(
      std::string_view(p_char.get_id())
      == "ID_543"
    );
    
    p_char.set_name("code_name_ii");

    REQUIRE(p_char.get_name() != nullptr);
    REQUIRE(
      std::string_view(p_char.get_name())
      == "code_name_ii"
    );
    
    p_char.set_name("code_name_iii");

    REQUIRE(p_char.get_name() != nullptr);
    REQUIRE(
      std::string_view(p_char.get_name())
      == "code_name_iii"
    );
  }

  SECTION("CLONE COPY_AND_MOVE")
  {
    example_ii::PlayableCharacter p_char;

    p_char.set_id("ID_0");

    std::unique_ptr<example_ii::PlayableCharacter> 
      p_char_copy_ptr = p_char.clone();

    p_char_copy_ptr->set_id("ID_1");

    REQUIRE(std::string_view(p_char.get_id()) == "ID_0");
    REQUIRE(
      std::string_view(p_char_copy_ptr->get_id()) == "ID_1"
    );

    std::unique_ptr<example_ii::PlayableCharacter> 
      p_char_move_uptr = std::move(*p_char_copy_ptr).clone();

    REQUIRE(
      std::string_view(p_char_copy_ptr->get_id()) == "N/a"
    );
    REQUIRE(
      std::string_view(p_char_move_uptr->get_id()) == "ID_1"
    );

    p_char_move_uptr->set_id("ID_3");

    REQUIRE(
      std::string_view(p_char_copy_ptr->get_id()) == "N/a"
    );
    REQUIRE(
      std::string_view(p_char_move_uptr->get_id()) == "ID_3"
    );

    std::unique_ptr<example_ii::PlayableCharacter> 
      p_char_move_ii_uptr 
        = example_ii::PlayableCharacter().clone();
    
    REQUIRE(
      std::string_view(p_char_move_ii_uptr->get_id()) == "N/a"
    );

  }
}