
#include "../src/rational_geometry/common_factor.hpp"

#include "doctest.h"

#include <string>
#include <typeinfo>

namespace rational_geometry {

TEST_CASE("Testing common_factor.hpp")
{
  SUBCASE("constexpr abs<>()")
  {
    constexpr auto a = abs(2);
    CHECK(2 == a);

    constexpr auto b = abs(-2);
    CHECK(2 == b);
  }

  SUBCASE("constexpr gcd<>()")
  {
    // clang-format off
    const long a = 2 * 2 * 3 * 3 * 5 * 5 * 5;
    const long b =             3 * 5 * 5     * 7 * 11;
    const long expected =      3 * 5 * 5;
    // clang-format on

    SUBCASE("normal case")
    {
      constexpr long c = gcd(a, b);
      CHECK(expected == c);
    }

    SUBCASE("negative numbers")
    {
      constexpr long c1 = gcd(-a, b);
      constexpr long c2 = gcd(a, -b);
      constexpr long c3 = gcd(-a, -b);

      // Always positive results.
      CHECK(expected == c1);
      CHECK(expected == c2);
      CHECK(expected == c3);
    }
  }
}

} // namespace rational_geometry

// vim:set et ts=2 sw=2 sts=2:

