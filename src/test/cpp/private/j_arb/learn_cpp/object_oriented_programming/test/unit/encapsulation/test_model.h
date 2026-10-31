#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_TEST_UNIT_ENCAPSULATION_TEST_MODEL_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_TEST_UNIT_ENCAPSULATION_TEST_MODEL_H

#include <cstring>
#include <string_view>
#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/Type.h"
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/Model.h"
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/rule_of_three/Model.h"
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/rule_of_three/Player.h"

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
          encapsulation::get_type_cstr(
            model_i.get_type()
          )
      )
      == "ITEM"
    );
  }

  SECTION("TO_STRING") {

    object_oriented_programming::
      encapsulation::Model model_i;
    std::fprintf(stdout, ">>>> %s\n", model_i.to_string().c_str());

    object_oriented_programming::
      encapsulation::Model model_ii;
    std::cout << "<<<< " << model_ii << "\n";

    model_ii.set_id("m-123");
    std::cout << "<<<< " << model_ii << "\n";

    object_oriented_programming::
      encapsulation::Model model_iii;

    model_iii.set_id("m-456");
    std::cout << "<<<< " << model_iii << "\n";
  }

  SECTION("EQUALS") {

    object_oriented_programming::
      encapsulation::Model model_i;

    REQUIRE(model_i.equals(&model_i));

    object_oriented_programming::
      encapsulation::Model model_ii;

    REQUIRE(model_ii.equals(&model_i));

    model_ii.set_id("m-123");

    REQUIRE(model_ii.equals(&model_i) != true);

    model_i.set_id("m-123");

    REQUIRE(model_ii.equals(&model_i));

    model_i.set_type(object_oriented_programming::
      encapsulation::Type::ITEM);

    REQUIRE(model_ii.equals(&model_i) != true);

    model_ii.set_type(object_oriented_programming::
      encapsulation::Type::ITEM);

    REQUIRE(model_ii.equals(&model_i));
  }

  SECTION("RULE OF THREE") {

    object_oriented_programming::
      encapsulation::rule_of_three::
        Model model_i;

    REQUIRE(std::string_view(model_i.get_id()) == "N/a");

    model_i.set_id("m-123");

    REQUIRE(std::string_view(model_i.get_id()) == "m-123");

    model_i.set_id(nullptr);

    REQUIRE(std::string_view(model_i.get_id()) == "N/a");

    object_oriented_programming::
      encapsulation::rule_of_three::
        Model model_ii(nullptr);
      
    REQUIRE(std::string_view(model_ii.get_id()) == "N/a");
    
    object_oriented_programming::
      encapsulation::rule_of_three::
        Model model_iii(
          nullptr, 
          object_oriented_programming::
            encapsulation::Type::ITEM
        );

    REQUIRE(
      model_iii.get_type() 
      == 
      object_oriented_programming::
        encapsulation::Type::ITEM
    );

    REQUIRE(
      std::string_view(model_iii.get_id())
      == 
      object_oriented_programming::
        encapsulation::rule_of_three::
          Model::get_default_id().get()
    );

    std::cout << ">>>>101: " << model_i << "\n";

    REQUIRE(model_i == model_i);
    REQUIRE(model_i == model_ii);
    REQUIRE(model_ii == model_i);
    REQUIRE(model_iii != model_i);
    REQUIRE(model_iii != model_ii);

    object_oriented_programming::
      encapsulation::rule_of_three::
        Model * model_iv
          = new object_oriented_programming::
              encapsulation::rule_of_three::
                Player();

    std::cout << ">>>>102: " << *model_iv << "\n";

    object_oriented_programming::
      encapsulation::rule_of_three::
        Model model_v(nullptr);

    std::cout << ">>>>103: " << model_v << "\n";

    object_oriented_programming::
      encapsulation::rule_of_three::
        Player player_i;

    std::cout << ">>>>104: " << player_i << "\n";

    object_oriented_programming::
      encapsulation::rule_of_three::
        Player player_ii("p-1");

    std::cout << ">>>>105: " << player_ii << "\n";

    object_oriented_programming::
      encapsulation::rule_of_three::
        Player player_iii(nullptr);

    std::cout << ">>>>106: " << player_iii << "\n";

  }
}

#endif