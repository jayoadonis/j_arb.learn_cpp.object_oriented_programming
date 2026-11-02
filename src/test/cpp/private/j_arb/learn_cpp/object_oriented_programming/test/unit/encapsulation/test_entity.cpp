#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_TEST_UNIT_ENCAPSULATION_TEST_ENTITY_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_TEST_UNIT_ENCAPSULATION_TEST_ENTITY_H

#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <iostream>

#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/Entity.h"
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/rule_of_zero/Entity.h"
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/demo_i/Entity.h"
#include "j_arb/learn_cpp/object_oriented_programming/encapsulation/demo_i/Type.h"

TEST_CASE("ENCAPSULATION ENTITY", "[unit]") {

  using namespace j_arb::learn_cpp;

  SECTION("I") {

    object_oriented_programming::
      encapsulation::Entity entity_i;

    std::fprintf(stdout, 
      "::: ENCAPSULATION ENTITY, I: '%s'\n", entity_i.get_id());

    REQUIRE(std::strcmp(entity_i.get_id(), "") == 0);

    entity_i.set_id("test_123");

    std::fprintf(stdout, 
      "::: ENCAPSULATION ENTITY, I: '%s'\n", entity_i.get_id());

    REQUIRE(std::strcmp(entity_i.get_id(), "test_123") == 0);

    object_oriented_programming::
      encapsulation::Entity * entity_ii 
        = entity_i.clone();

    REQUIRE(std::strcmp(entity_ii->get_id(), "test_123") == 0);

    delete entity_ii;
    entity_ii = nullptr;
  }

  SECTION("RULE OF ZERO", "[unit]") {

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

  SECTION("DEMO I") {
    
    object_oriented_programming::
      encapsulation::demo_i::
        Entity entity_i;

    REQUIRE(std::string_view(entity_i.get_id()) == "");
    REQUIRE(
      entity_i.get_type() 
      == 
      object_oriented_programming::
        encapsulation::demo_i:: 
          Type::UNKNOWN);

    object_oriented_programming::
      encapsulation::demo_i::
        Entity entity_ii;

    REQUIRE(entity_ii == entity_i);

    entity_ii.set_id("e-123");

    REQUIRE(entity_ii != entity_i);

    std::cout << "<<<<< " << entity_i << "\n";
    std::cout << "<<<<< " << entity_ii << "\n";
    
    object_oriented_programming::
      encapsulation::demo_i::
        Entity entity_iii;

    std::cout << "<<<<< " << entity_iii << "\n";

    entity_iii.set_id("e-123");

    std::cout << "<<<<< " << entity_iii << "\n";

    entity_iii.set_type(object_oriented_programming::
      encapsulation::demo_i::Type::ITEM
    );

    std::cout << "<<<<< " << entity_iii << "\n";


  }
}

#endif