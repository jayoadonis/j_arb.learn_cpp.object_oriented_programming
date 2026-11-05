
#include "j_arb/learn_cpp/object_oriented_programming/utility/EType.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("UTILITY", "[unit]") 
{

  using namespace j_arb::learn_cpp::object_oriented_programming;
  
  SECTION("TEST E TYPE")
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
      get_e_type_as_string(utility::EType::UNKNOWN).find("UNKNOWN") 
      != std::string::npos
    );

    REQUIRE(
      get_e_type_as_string(utility::EType::UNKNOWN).find("value=1") 
      != std::string::npos
    );
  }
}

