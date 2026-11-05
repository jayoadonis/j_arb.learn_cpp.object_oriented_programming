
#include "j_arb/learn_cpp/object_oriented_programming/utility/EType.h"
#include "j_arb/learn_cpp/object_oriented_programming/utility/demangle.h"
#include <istream>
#include <iostream>
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <string_view>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("UTILITY E TYPE", "[unit]")
{

  using namespace j_arb::learn_cpp::object_oriented_programming;

  SECTION("UNDERLYING TYPE")
  {
    REQUIRE(
      utility::demangle(typeid(utility::EType_t).name())
      ==
      std::string_view("unsigned int")
    );
    REQUIRE(
      std::string(utility::demangle(typeid(utility::EType).name())).find("utility::EType")
      != std::string::npos
    );
  }

  SECTION("DECLARATION WITH EMPTY LIST-INITIALIZER")
  {
    //REM: enum declaration with list-initializer `{}` is a must,
    //REM: default it to `NONE(0)`
    utility::EType e_type{};

    REQUIRE(e_type == utility::EType::NONE);
  }

  SECTION("INITIALIZATION")
  {
    utility::EType e_type = utility::EType::UNKNOWN;

    REQUIRE(
      static_cast<utility::EType_t>(e_type) 
      == 
      static_cast<utility::EType_t>(1)
    );
  }

  SECTION("BASIC COMPARISON")
  {
    REQUIRE(
      static_cast<utility::EType_t>(utility::EType::UNKNOWN)
      == static_cast<utility::EType_t>(1)
    );

    REQUIRE(
      get_e_type_as_cstr(utility::EType::UNKNOWN)
      == std::string_view("UNKNOWN")
    );

    REQUIRE(
      get_e_type_as_string_dump(utility::EType::UNKNOWN).find("UNKNOWN")
      != std::string::npos
    );

    REQUIRE(
      get_e_type_as_string_dump(utility::EType::UNKNOWN).find("value=1")
      != std::string::npos
    );
  }

  SECTION("BASIC IO")
  {

    std::istringstream iss;
    utility::EType flag{};

    iss >> flag;

    REQUIRE(iss.fail() == true);
    REQUIRE(flag == utility::EType::NONE);
    REQUIRE(iss.str() == "");

    std::istringstream iss_ii("RANDOM_FLAG");
    utility::EType flag_ii = utility::EType::ACTOR; 

    iss_ii >> flag_ii;

    REQUIRE(iss_ii.fail() == true);
    REQUIRE(flag_ii == utility::EType::ACTOR);
    REQUIRE(iss_ii.str() == "RANDOM_FLAG");

  }

  SECTION("BASIC BITWISE")
  {
    utility::EType e_type 
      = utility::EType::ITEM | utility::EType::ACTOR;

    REQUIRE(
      e_type 
      ==
      (utility::EType::ITEM | utility::EType::ACTOR)
    );
    
    REQUIRE(
      e_type 
      !=
      (utility::EType::UNKNOWN | utility::EType::ACTOR)
    );
    
    utility::EType e_type_ii 
      = utility::EType::UNKNOWN | utility::EType::ITEM 
        | utility::EType::ACTOR;

    REQUIRE(
      e_type_ii
      ==
      (utility::EType::UNKNOWN | utility::EType::ITEM | utility::EType::ACTOR)
    );   

    utility::EType e_type_iii 
      = utility::EType::ITEM & utility::EType::ACTOR;

    REQUIRE(
      e_type_iii 
      ==
      (utility::EType::ITEM & utility::EType::ACTOR)
    );
    
    REQUIRE(
      e_type_iii == utility::EType::NONE
    );
    
    REQUIRE(
      (utility::EType::ITEM & utility::EType::ITEM)
      ==
      utility::EType::ITEM
    );
    
    REQUIRE(
      ((utility::EType::ITEM | utility::EType::ACTOR) 
      & utility::EType::ACTOR)
      ==
      utility::EType::ACTOR
    );

    REQUIRE(
      (~utility::EType::NONE)
      ==
      static_cast<utility::EType>(-1)
    );

    //REM: ON (default)
    utility::EType toggle_item = utility::EType::ITEM;

    REQUIRE(toggle_item == utility::EType::ITEM);

    //REM: OFF
    toggle_item = toggle_item ^ utility::EType::ITEM;
    
    REQUIRE(toggle_item == utility::EType::NONE);

    //REM: ON
    toggle_item = toggle_item ^ utility::EType::ITEM;

    REQUIRE(toggle_item == utility::EType::ITEM);
  }

  SECTION("CONTAIN")
  {
    utility::EType flags_ii 
      = utility::EType::ITEM | utility::EType::ACTOR;
    
    REQUIRE(
      utility::contain(flags_ii, utility::EType::ITEM)
      ==
      utility::EType::ITEM
    );
    
    REQUIRE(
      utility::contain(flags_ii, utility::EType::ACTOR)
      ==
      utility::EType::ACTOR
    );
    
    REQUIRE(
      utility::contain(flags_ii, utility::EType::ITEM | utility::EType::ACTOR)
      ==
      (utility::EType::ITEM | utility::EType::ACTOR)
    );
      
    REQUIRE(
      utility::contain(flags_ii, utility::EType::NONE)
      ==
      utility::EType::NPOS
    );

    REQUIRE(
      utility::contain(flags_ii, utility::EType::UNKNOWN)
      ==
      utility::EType::NPOS
    );

    REQUIRE(
      utility::contain(flags_ii, utility::EType::NONE | utility::EType::ITEM)
      ==
      utility::EType::ITEM
    );
  }
}

