
#include "../src/rational_geometry/Rational.hpp"

#include "doctest.h"

#include <climits>
#include <cstdint>
#include <sstream>

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

      SUBCASE("Rational(SignedIntT, SignedIntT)")
      {
        Rational<int, 12> a{2, 3};

        CHECK(a.numerator() == 8);
        CHECK(a.denominator() == 12);
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

    SUBCASE("Operators")
    {
      SUBCASE("double conversion")
      {
        MyRationalT a{3, 2};

        CHECK(static_cast<long double>(a) == doctest::Approx(1.5));
      }

      SUBCASE("operator++")
      {
        MyRationalT a{};
        REQUIRE(a == 0);

        CHECK(++a == 1);
        CHECK(a++ == 1);
        CHECK(a == 2);
      }

      SUBCASE("operator--")
      {
        MyRationalT a{};
        REQUIRE(a == 0);

        CHECK(--a == -1);
        CHECK(a-- == -1);
        CHECK(a == -2);
      }

      SUBCASE("unary operator-")
      {
        MyRationalT a{1};

        CHECK(-a == -1);
      }
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

      SUBCASE("Rational<same> == Rational<same>")
      {
        MyRationalT a{23};
        MyRationalT b{23};
        MyRationalT c{57};

        bool are_equal;
        are_equal = a == a;
        CHECK(are_equal);

        are_equal = a == b;
        CHECK(are_equal);

        are_equal = b == a;
        CHECK(are_equal);

        are_equal = b == c;
        CHECK_FALSE(are_equal);
      }
    }

    SUBCASE("binary +")
    {
      /// \todo  figure out why this works without being implemented.
      SUBCASE("Rational<same> + Rational<same>")
      {
        MyRationalT a{2};
        MyRationalT b{3};
        MyRationalT c{5};

        MyRationalT r_2_3{2, 3};
        MyRationalT r_1_4{1, 4};
        MyRationalT r_11_12{11, 12};

        bool are_equal;

        are_equal = a + b == c;
        CHECK(are_equal);

        are_equal = r_2_3 + r_1_4 == r_11_12;
        CHECK(are_equal);

        are_equal = a + b == r_2_3;
        CHECK_FALSE(are_equal);
      }
    }
  }
}


} // namespace rational_geometry

// vim:set et ts=2 sw=2 sts=2:

