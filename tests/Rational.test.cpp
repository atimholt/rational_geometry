
#include "../src/rational_geometry/Rational.hpp"

#include "doctest.h"

#include <climits>
#include <cstdint>

namespace rational_geometry {


TEST_CASE("Testing Rational.hpp")
{
  // log_2 of the constant below, rounded up. add 1 for sign bit
  // (Too late to test this after actually defining the constant!)
  REQUIRE(sizeof(intmax_t) * CHAR_BIT > 29 + 1);

  // 2^4 * 3^3 * 5^4 * 7 * 11 * 13
  // Or, the 10th superior highly composite number (720720) * 5^3 (for
  // friendlier base 10) * 3 (for good measure)
  const intmax_t arbitrary_composite = 270'270'000;

  typedef Rational<intmax_t, arbitrary_composite> MyRationalT;

  SUBCASE("Rational<> class")
  {
    SUBCASE("Constructors")
    {
      SUBCASE("Rational()")
      {
        MyRationalT a{};

        CHECK(a == 0);
      }

      SUBCASE("Rational(SignedIntT)")
      {
        MyRationalT a{23};

        CHECK(a == 23);
      }
    }

    SUBCASE("Accessors")
    {
      Rational<int, 12> a{6};

      SUBCASE("numerator()")
      {
        CHECK(a.numerator() == 72);
      }

      SUBCASE("denominator()")
      {
        CHECK(a.denominator() == 12);
      }
    }

    SUBCASE("Related Operators")
    {
      SUBCASE("==")
      {
        SUBCASE("Rational == int")
        {
          MyRationalT a{23};

          bool are_equal = a == 23;
          CHECK(are_equal);
        }

        SUBCASE("int == Rational")
        {
          MyRationalT a{23};

          bool are_equal = 23 == a;
          CHECK(are_equal);
        }
      }
    }
  }
}


} // namespace rational_geometry

// vim:set et ts=2 sw=2 sts=2:

