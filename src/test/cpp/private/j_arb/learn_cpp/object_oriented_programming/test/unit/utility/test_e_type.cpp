
#include "j_arb/learn_cpp/object_oriented_programming/utility/EType.h"
#include "j_arb/learn_cpp/object_oriented_programming/utility/demangle.h"
#include <istream>
#include <iostream>
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <string_view>
#include <algorithm>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("UTILITY E TYPE", "[unit]")
{

  using namespace j_arb::learn_cpp::object_oriented_programming;

  SECTION("NONE, ALL, NPOS, OUT_OF_BOUNDS, UNION_FLAGS")
  {
    //REM: NONE (default) with list-initializer `{}`
    utility::EType none_flag{};
    REQUIRE(none_flag == utility::EType::NONE);
    REQUIRE(
      static_cast<utility::EType_t>(none_flag)
      ==
      static_cast<utility::EType_t>(0)
    );
    std::string name_info
      = utility::get_e_type_as_string_dump(none_flag);
    std::string search = "NONE";
    REQUIRE(
      std::search(
        name_info.begin(), name_info.end(),
        search.begin(), search.end()
      )
      !=
      name_info.end()
    );
    search = "value=0";
    REQUIRE(
      name_info.find(search)
      !=
      std::string_view::npos
    );

    //REM: ALL
    utility::EType all_flag = utility::EType::ALL;
    name_info = get_e_type_as_string_dump(all_flag);
    REQUIRE(all_flag == utility::EType::ALL);
    REQUIRE(all_flag == (utility::EType::UNKNOWN | utility::EType::ITEM | utility::EType::ACTOR));
    REQUIRE(
      name_info.find("ALL") != std::string::npos
    );
    search = "value="
      + std::to_string(static_cast<utility::EType_t>(
        utility::EType::UNKNOWN |
        utility::EType::ITEM |
        utility::EType::ACTOR
        ));
    REQUIRE(
      name_info.find(search) != std::string::npos
    );

    //REM: NPOS
    utility::EType npos_flag = utility::EType::NPOS;
    name_info = get_e_type_as_string_dump(npos_flag);
    REQUIRE(
      npos_flag == utility::EType::NPOS
    );
    REQUIRE(
      name_info.find("NPOS") != std::string::npos
    );
    search = "value="
      + std::to_string(std::numeric_limits<utility::EType_t>::max());
    REQUIRE(
      name_info.find(search) != std::string::npos
    );

    //REM: OUT_OF_BOUNDS
    utility::EType out_of_bounds = static_cast<utility::EType>(
      static_cast<utility::EType_t>(npos_flag) - 1
      );
    REQUIRE(
      get_e_type_as_string_dump(out_of_bounds).find("OUT_OF_BOUNDS")
      !=
      std::string::npos
    );
    search = "value="
      + std::to_string(static_cast<utility::EType_t>(
        out_of_bounds
        ));
    REQUIRE(
      get_e_type_as_string_dump(out_of_bounds).find(search)
      !=
      std::string::npos
    );

    //REM: UNION_FLAGS
    utility::EType union_flags = utility::EType::ITEM | utility::EType::ACTOR;
    REQUIRE(
      get_e_type_as_string_dump(union_flags).find("UNION_FLAGS")
      !=
      std::string::npos
    );
    search = "value="
      + std::to_string(static_cast<utility::EType_t>(
        union_flags
        ));
    REQUIRE(
      get_e_type_as_string_dump(union_flags).find(search)
      !=
      std::string::npos
    );
  }

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

    iss_ii.clear();
    iss_ii.str("ITEM ACTOR");

    unsigned int actor_count = 0;
    unsigned int item_count = 0;

    while (iss_ii >> flag_ii)
    {
      if (flag_ii == utility::EType::ACTOR)
      {
        actor_count++;
      }
      if (flag_ii == utility::EType::ITEM)
      {
        item_count++;
      }
    }

    REQUIRE(actor_count == 1);
    REQUIRE(item_count == 1);
    REQUIRE(iss_ii.eof() == true);
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
      utility::contain_e_type(flags_ii, utility::EType::ITEM)
      ==
      utility::EType::ITEM
    );

    REQUIRE(
      utility::contain_e_type(flags_ii, utility::EType::ACTOR)
      ==
      utility::EType::ACTOR
    );

    REQUIRE(
      utility::contain_e_type(flags_ii, utility::EType::ITEM | utility::EType::ACTOR)
      ==
      (utility::EType::ITEM | utility::EType::ACTOR)
    );

    REQUIRE(
      utility::contain_e_type(flags_ii, utility::EType::NONE)
      ==
      utility::EType::NPOS
    );

    REQUIRE(
      utility::contain_e_type(flags_ii, utility::EType::UNKNOWN)
      ==
      utility::EType::NPOS
    );

    REQUIRE(
      utility::contain_e_type(flags_ii, utility::EType::NONE | utility::EType::ITEM)
      ==
      utility::EType::ITEM
    );
  }

  SECTION("VALIDATE")
  {
    utility::EType e_type{};

    REQUIRE(
      utility::validate_e_type(e_type)
      ==
      utility::EType::NONE
    );

    e_type = utility::EType::NONE;

    REQUIRE(
      utility::validate_e_type(e_type)
      ==
      utility::EType::NONE
    );

    e_type = utility::EType::ACTOR;

    REQUIRE(
      utility::validate_e_type(e_type)
      ==
      utility::EType::ACTOR
    );

    e_type = utility::EType::ITEM | utility::EType::ACTOR;

    REQUIRE(
      utility::validate_e_type(e_type)
      ==
      (utility::EType::ACTOR | utility::EType::ITEM)
    );

    utility::EType fake_flag = static_cast<utility::EType>(
      static_cast<utility::EType_t>(utility::EType::ALL) + 1
      );

    REQUIRE(
      utility::validate_e_type(fake_flag)
      ==
      utility::EType::NPOS
    );

    utility::EType fake_mixed_flag
      = static_cast<utility::EType>(
        static_cast<utility::EType_t>(utility::EType::ALL) + 1
        ) | utility::EType::ITEM;

    REQUIRE(
      utility::validate_e_type(fake_mixed_flag)
      ==
      utility::EType::NPOS
    );
  }
}

