#include <cstring>
#include <string_view>
#include <catch2/catch_test_macros.hpp>
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/Type.h"
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

  SECTION("SET TYPE") {

    object_oriented_programming::
      encapsulation::Model model_i;

    REQUIRE(model_i.get_type() == 
      object_oriented_programming::
        encapsulation::Type::UNKNOWN
    );

    object_oriented_programming::
      encapsulation::Type type
        = object_oriented_programming::
            encapsulation::Type::ITEM;

    model_i.set_type(type);

    REQUIRE(model_i.get_type() == 
      object_oriented_programming::
        encapsulation::Type::ITEM
    );

    REQUIRE(
      std::string_view(
        object_oriented_programming::
          encapsulation::get_type_str(
            model_i.get_type()
          )
      )
      == "ITEM"
    );
  }
}