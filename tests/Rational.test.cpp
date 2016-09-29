
#include "../src/rational_geometry/Rational.hpp"

#include "doctest.h"

#include <climits>
#include <cstdint>
#include <sstream>
#include <string>
#include <typeinfo>

namespace rational_geometry {


using namespace std::string_literals;

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

        SUBCASE("Exceptional")
        {
          try {
            Rational<int, 12> a{3, 17};
            CHECK(false);
          }
          catch (unrepresentable_operation_error<int> e) {
            using namespace std::literals;
            // Because it's vendor dependent:
            auto int_name = typeid(int).name();
            CHECK(std::string(e.what())
                  == "Inexact construction of a Rational<"s + int_name
                         + ", 12>"s);
          }
        }
      }

      SUBCASE("Rational(long double)")
      {
        MyRationalT a{23.0L};
        CHECK(a == 23);

        MyRationalT expected{51, 50};
        MyRationalT b{51.0L / 50};
        CHECK(expected == b);
      }

      SUBCASE("Rational(double)")
      {
        MyRationalT a{23.0};
        CHECK(a == 23);

        MyRationalT expected{51, 50};
        MyRationalT b{1.02};
        CHECK(expected == b);
      }

      SUBCASE("Rational(float)")
      {
        MyRationalT a{23.0f};
        CHECK(a == 23);

        MyRationalT expected{51, 50};
        MyRationalT b{1.02f};

        if (sizeof(float) * CHAR_BIT <= 32) {
          // float is too small a type for the large denominator!
          CHECK_FALSE(expected == b);
        }
        else {
          CHECK(expected == b);
        }

        Rational<int, 100> expected_smaller_denom{51, 50};
        Rational<int, 100> b_smaller_denom{51.0f / 50};
        CHECK(expected_smaller_denom == b_smaller_denom);
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
    SUBCASE("Comparison")
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

      SUBCASE("!=")
      {
        SUBCASE("Rational != int")
        {
          MyRationalT a{23};

          CHECK(a != 24);
        }

        SUBCASE("int == Rational")
        {
          MyRationalT a{23};

          CHECK(24 != a);
        }

        SUBCASE("Rational<same> == Rational<same>")
        {
          MyRationalT a{23};
          MyRationalT b{23};
          MyRationalT c{57};

          CHECK(a != c);
          CHECK(c != a);
          CHECK_FALSE(a != b);
        }
      }

      SUBCASE("<")
      {
        SUBCASE("Rational < int")
        {
          MyRationalT a{5};
          CHECK(a < 6);
          CHECK_FALSE(a < 4);
        }

        SUBCASE("int < Rational")
        {
          MyRationalT a{5};
          CHECK(4 < a);
          CHECK_FALSE(6 < a);
        }

        SUBCASE("Rational<same> < Rational<same>")
        {
          MyRationalT a{5};
          MyRationalT b{5};
          MyRationalT c{7};

          CHECK(a < c);
          CHECK_FALSE(a < b);
        }
      }

      SUBCASE(">")
      {
        SUBCASE("Rational > int")
        {
          MyRationalT a{5};
          CHECK(6 > a);
          CHECK_FALSE(a > 6);
        }

        SUBCASE("int > Rational")
        {
          MyRationalT a{5};
          CHECK(a > 4);
          CHECK_FALSE(4 > a);
        }

        SUBCASE("Rational<same> > Rational<same>")
        {
          MyRationalT a{5};
          MyRationalT b{5};
          MyRationalT c{7};

          CHECK(c > a);
          CHECK_FALSE(a > b);
        }
      }

      SUBCASE("<=")
      {
        SUBCASE("Rational <= int")
        {
          MyRationalT a{5};
          CHECK(a <= 6);
          CHECK(a <= 5);
          CHECK_FALSE(a <= 4);
        }

        SUBCASE("int <= Rational")
        {
          MyRationalT a{5};
          CHECK(4 <= a);
          CHECK(5 <= a);
          CHECK_FALSE(6 <= a);
        }

        SUBCASE("Rational<same> <= Rational<same>")
        {
          MyRationalT a{5};
          MyRationalT b{5};
          MyRationalT c{7};

          CHECK(a <= c);
          CHECK(a <= b);
        }
      }

      SUBCASE(">=")
      {
        SUBCASE("Rational >= int")
        {
          MyRationalT a{5};
          CHECK(a >= 4);
          CHECK(a >= 5);
          CHECK_FALSE(a >= 6);
        }

        SUBCASE("int >= Rational")
        {
          MyRationalT a{5};
          CHECK(6 >= a);
          CHECK(5 >= a);
          CHECK_FALSE(4 >= a);
        }

        SUBCASE("Rational<same> >= Rational<same>")
        {
          MyRationalT a{5};
          MyRationalT b{5};
          MyRationalT c{7};

          CHECK(c >= a);
          CHECK(a >= b);
        }
      }
    }

    SUBCASE("Arithmetic")
    {
      SUBCASE("binary *")
      {
        SUBCASE("Rational * int")
        {
          MyRationalT a{2};

          CHECK(a * 3 == 6);

          MyRationalT b{2, 3};

          std::string result_type_name{typeid(b * 3).name()};
          std::string b_type_name{typeid(b).name()};
          CHECK(b_type_name == result_type_name);

          CHECK(2 == b * 3);
        }

        SUBCASE("int * Rational")
        {
          MyRationalT a{2};

          CHECK(3 * a == 6);

          MyRationalT b{2, 3};

          std::string result_type_name{typeid(3 * b).name()};
          std::string b_type_name{typeid(b).name()};
          CHECK(b_type_name == result_type_name);

          CHECK(2 == 3 * b);
        }

        SUBCASE("Rational<same> * Rational<same>")
        {
          MyRationalT a{2};
          MyRationalT b{3};
          MyRationalT c{6};

          MyRationalT r_2_3{2, 3};
          MyRationalT r_1_4{1, 4};
          MyRationalT r_1_6{1, 6};

          CHECK(a * b == c);
          CHECK(r_2_3 * r_1_4 == r_1_6);
          CHECK_FALSE(a * b == r_2_3);

          SUBCASE("Exceptional")
          {
            using SmallerRat = Rational<int, 12>;

            SmallerRat a{1, 3};
            SmallerRat b{2, 3};

            CHECK_THROWS_AS(a * b, unrepresentable_operation_error<int>);
          }
        }
      }

      SUBCASE("/")
      {
        SUBCASE("Rational / int")
        {
          MyRationalT a{18};

          CHECK(a / 3 == 6);

          MyRationalT b{2};
          MyRationalT expected{2, 3};

          std::string result_type_name{typeid(b / 3).name()};
          std::string b_type_name{typeid(b).name()};
          CHECK(b_type_name == result_type_name);

          CHECK(expected == b / 3);

          SUBCASE("Exceptional")
          {
            using RatI18 = Rational<int, 18>;
            RatI18 a{1};
            CHECK_THROWS_AS(a / 27, unrepresentable_operation_error<int>);

            // Doesn't throw
            RatI18 b{3};
            RatI18 expected{1, 9};
            CHECK(expected == b / 27);

            try {
              a / 27;
            }
            catch (unrepresentable_operation_error<int> e) {
              CHECK(e.get_minimum_fix_factor() == 3);
              auto expected = "Inexact operation in ("s + typeid(a).name()
                              + " 18/18 / "s + typeid(int).name()
                              + " 27):  18/27 -> "s + typeid(int).name();
              CHECK(e.what() == expected);
            }
          }
        }

        SUBCASE("int / Rational")
        {
          MyRationalT a{3};

          CHECK(18 / a == 6);

          MyRationalT b{3};
          MyRationalT expected{2, 3};

          std::string result_type_name{typeid(2 / b).name()};
          std::string b_type_name{typeid(b).name()};
          CHECK(b_type_name == result_type_name);

          CHECK(expected == 2 / b);

          SUBCASE("Exceptional")
          {
            using RatI18 = Rational<int, 18>;
            RatI18 a{5, 18};
            CHECK_THROWS_AS(1 / a, unrepresentable_operation_error<int>);

            // Doesn't throw
            CHECK(18 == 5 / a);

            try {
              1 / a;
            }
            catch (unrepresentable_operation_error<int> e) {
              CHECK(e.get_minimum_fix_factor() == 5);
              auto expected = "Inexact operation in ("s + typeid(int).name()
                              + " 1 / "s + typeid(a).name()
                              + " 5/18):  324/5 -> "s + typeid(int).name();
              CHECK(expected == std::string(e.what()));
            }
          }
        }

        SUBCASE("Rational<same> / Rational<same>")
        {
          MyRationalT a{2};
          MyRationalT b{3};
          MyRationalT c{6};

          MyRationalT r_2_3{2, 3};
          MyRationalT r_1_4{1, 4};
          MyRationalT r_1_6{1, 6};

          CHECK(a == c / b);
          CHECK(r_2_3 * r_1_4 == r_1_6);
          CHECK(r_1_4 == r_1_6 / r_2_3);
          CHECK_FALSE(a * b == r_2_3);
          CHECK_FALSE(r_2_3 / b == a);
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

    SUBCASE("ostream output")
    {
      std::stringstream a{};

      a << Rational<int, 4>{1, 4};
      CHECK(a.str() == "1/4");

      a.str("");
      a << Rational<int, 12>{1, 3};
      CHECK(a.str() == "4/12");
    }
  }
}


} // namespace rational_geometry

// vim:set et ts=2 sw=2 sts=2:

