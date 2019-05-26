
#include "../src/rational_geometry/unrepresentable_operation_error.hpp"

#include "doctest.h"

#include <ostream>
#include <string>
#include <typeinfo>

namespace rational_geometry {


TEST_CASE("Testing unrepresentable_operation_error.hpp")
{
  SUBCASE("class unrepresentable_operation_error")
  {
    SUBCASE("Constructors")
    {
      auto a =
          unrepresentable_operation_error<int>("This error is a test", 12, 8);

      CHECK(a.get_minimum_fix_factor() == 2);
    }

    SUBCASE("accumulate_fix_factor()")
    {
      int fix_factor{1};
      const int kDenom{12};

      try {
        throw unrepresentable_operation_error<int>{"An error", kDenom, 8};
      }
      catch (const unrepresentable_operation_error<int>& e) {
        e.accumulate_fix_factor(fix_factor);
      }

      CHECK(fix_factor == 2);

      try {
        throw unrepresentable_operation_error<int>{"An error", kDenom, 9};
      }
      catch (const unrepresentable_operation_error<int>& e) {
        e.accumulate_fix_factor(fix_factor);
      }

      REQUIRE(fix_factor == 2 * 3);

      // Runtime returns on what to put into the code manually:
      auto kFixFactorAccumulated = 2 * 3;

      auto kNewDenom = kDenom * kFixFactorAccumulated;

      // These don't throw:
      //
      // Rational<int, kNewDenom> a{[any int value]};
      //
      // a / 8; a / 9;
    }
  }
}


} // namespace rational_geometry

// vim:set et ts=2 sw=2 sts=2:

