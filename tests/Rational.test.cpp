
#include "../src/rational_geometry/Rational.hpp"

#include "doctest.h"

#include <climits>
#include <cstdint>
#include <string>
//#include <sstream>
#include <typeinfo>

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

      SUBCASE("as_long_double")
      {
        MyRationalT a{3, 2};

        CHECK(a.as_long_double() == doctest::Approx(1.5));
      }
    }

    SUBCASE("Operators")
    {
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

        CHECK(a == 23);
      }

      SUBCASE("int == Rational")
      {
        MyRationalT a{23};

        CHECK(23 == a);
      }

      SUBCASE("Rational<same> == Rational<same>")
      {
        MyRationalT a{23};
        MyRationalT b{23};
        MyRationalT c{57};

        CHECK(a == a);
        CHECK(a == b);
        CHECK(b == a);
        CHECK_FALSE(b == c);
      }
    }

    SUBCASE("binary +")
    {
      SUBCASE("Rational + int")
      {
        MyRationalT a{2};

        CHECK(a + 1 == 3);

        MyRationalT b{2, 3};
        MyRationalT expected{5, 3};

        std::string result_type_name{typeid(b + 1).name()};
        std::string b_type_name{typeid(b).name()};
        CHECK(b_type_name == result_type_name);

        CHECK(expected == b + 1);
      }

      SUBCASE("int + Rational")
      {
        MyRationalT a{2};

        CHECK(1 + a == 3);

        MyRationalT b{2, 3};
        MyRationalT expected{5, 3};

        std::string result_type_name{typeid(1 + b).name()};
        std::string b_type_name{typeid(b).name()};
        CHECK(b_type_name == result_type_name);

        CHECK(expected == 1 + b);
      }

      SUBCASE("Rational<same> + Rational<same>")
      {
        MyRationalT a{2};
        MyRationalT b{3};
        MyRationalT c{5};

        MyRationalT r_2_3{2, 3};
        MyRationalT r_1_4{1, 4};
        MyRationalT r_11_12{11, 12};

        CHECK(a + b == c);
        CHECK(r_2_3 + r_1_4 == r_11_12);
        CHECK_FALSE(a + b == r_2_3);
      }
    }

    SUBCASE("binary -")
    {
      SUBCASE("Rational - int")
      {
        MyRationalT a{3};

        CHECK(a - 2 == 1);
      }

      SUBCASE("int - Rational")
      {
        MyRationalT a{2};

        CHECK(3 - a == 1);
      }

      SUBCASE("Rational - Rational")
      {
        MyRationalT a{3};
        MyRationalT b{2};

        CHECK(a - b == 1);
      }
    }
  }
}


} // namespace rational_geometry

// vim:set et ts=2 sw=2 sts=2:

