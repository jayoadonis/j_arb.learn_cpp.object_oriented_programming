
#include <catch2/catch_test_macros.hpp>
#include "j_arb/learn_cpp/object_oriented_programming/abstraction/demo_i/Type.h"

TEST_CASE("ABSTRACTION TYPE", "[unit]")
{

  using namespace j_arb::learn_cpp::
    object_oriented_programming;

  SECTION("DEFAULT CTOR") 
  {
    abstraction::demo_i::Type type_i;

    REQUIRE(type_i == abstraction::demo_i::EType::UNKNOWN);
    REQUIRE(type_i == abstraction::demo_i::Type::UNKNOWN);

    REQUIRE(
      type_i[abstraction::demo_i::Type::NONE]
      ==
      abstraction::demo_i::Type::NONE
    );
    REQUIRE(
      type_i[abstraction::demo_i::Type::ACTOR]
      ==
      abstraction::demo_i::Type::NONE
    );
    REQUIRE(
      type_i[abstraction::demo_i::Type::UNKNOWN]
      !=
      abstraction::demo_i::Type::NONE
    );
    REQUIRE(
      type_i[abstraction::demo_i::Type::UNKNOWN]
      ==
      abstraction::demo_i::Type::UNKNOWN
    );
    REQUIRE(
      type_i[
        abstraction::demo_i::Type::UNKNOWN | abstraction::demo_i::Type::ACTOR
      ]
      ==
      abstraction::demo_i::Type::NONE
    );
  }

  SECTION("CUSTOM CTOR")
  {
    abstraction::demo_i::Type 
      type_i(abstraction::demo_i::Type::ACTOR);

    REQUIRE(type_i == abstraction::demo_i::Type::ACTOR);

    abstraction::demo_i::Type
      type_ii(type_i);

    REQUIRE(type_ii == abstraction::demo_i::Type::ACTOR);
    REQUIRE(type_ii == type_i);

    abstraction::demo_i::Type
      type_iii(type_i);

    REQUIRE(type_iii == abstraction::demo_i::Type::ACTOR);
    REQUIRE(type_iii != abstraction::demo_i::Type::UNKNOWN);
    REQUIRE(
      type_iii[abstraction::demo_i::Type::NONE] 
      ==
      abstraction::demo_i::Type::NONE
    );
    REQUIRE(
      type_iii[abstraction::demo_i::Type::UNKNOWN] 
      ==
      abstraction::demo_i::Type::NONE
    );
    REQUIRE(
      type_iii[abstraction::demo_i::Type::ACTOR] 
      ==
      abstraction::demo_i::Type::ACTOR
    );

    abstraction::demo_i::EType e_types
      = abstraction::demo_i::EType::ITEM | abstraction::demo_i::EType::ACTOR;

    abstraction::demo_i::Type type_iv(e_types);

    REQUIRE(
      type_iv[abstraction::demo_i::Type::ITEM]
      ==
      abstraction::demo_i::EType::ITEM
    );

    REQUIRE(
      type_iv[abstraction::demo_i::Type::ACTOR]
      ==
      abstraction::demo_i::EType::ACTOR
    );

    REQUIRE(
      type_iv[abstraction::demo_i::Type::ACTOR | abstraction::demo_i::Type::ITEM]
      ==
      (abstraction::demo_i::EType::ACTOR | abstraction::demo_i::Type::ITEM)
    );

    abstraction::demo_i::Type type_v(
      abstraction::demo_i::EType::ITEM | abstraction::demo_i::EType::ACTOR
    );

    REQUIRE(type_v == type_iv);

    REQUIRE(
      (type_v << static_cast<abstraction::demo_i::EType>(1u)) 
      == 
      static_cast<abstraction::demo_i::EType>((4u + 2u) << 1u)
    );
    
    REQUIRE(
      (type_v >> static_cast<abstraction::demo_i::EType>(1u)) 
      == 
      static_cast<abstraction::demo_i::EType>((4u + 2u) >> 1u)
    );
  }
}
