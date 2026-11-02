#ifndef J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_TEST_UNIT_TEST_I_H
#define J_ARB_LEARN_CPP_OBJECT_ORIENTED_PROGRAMMING_TEST_UNIT_TEST_I_H

#include <catch2/catch_test_macros.hpp>
#include <cstdio>

TEST_CASE("TEST I", "[unit]") {

  SECTION("SEC I") {

    std::fprintf(stdout, "::: TEST I, %s\n",
      "SEC I");
  }

  SECTION("SEC II") {

    std::fprintf(stdout, "::: TEST I, %s\n",
      "SEC II");
  }
}

#endif