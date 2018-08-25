
#include "../src/rational_geometry/FixedRational.hpp"

#include "doctest.h"

#include <climits>
#include <cstdint>
#include <sstream>
#include <string>
#include <typeinfo>

namespace rational_geometry {


using namespace std::string_literals;

TEST_CASE("Testing FixedRational.hpp")
{
  // log_2 of the constant below, rounded up. add 1 for sign bit
  // (Too late to test this after actually defining the constant!)
  REQUIRE(sizeof(intmax_t) * CHAR_BIT > 29 + 1);

  // 2^4 * 3^3 * 5^4 * 7 * 11 * 13
  // Or, the 10th superior highly composite number (720720) * 5^3 (for
  // friendlier base 10) * 3 (for good measure)
  const intmax_t arbitrary_composite = 270'270'000;

  using MyRationalT = FixedRational<intmax_t, arbitrary_composite>;
  using ApproxRat   = FixedRational<intmax_t, 12, false>;
  using TinyRat     = FixedRational<int8_t, 12>;

  SUBCASE("FixedRational<> class")
  {
    SUBCASE("Constructors")
    {
      SUBCASE("FixedRational()")
      {
        MyRationalT a{};
        CHECK(a == 0);

        ApproxRat b{};
        CHECK(b == 0);
      }

      SUBCASE("FixedRational<a>(FixedRational<b>)")
      {
        MyRationalT a{2};

        // a would be too big to fit in b if not for deeper templating
        TinyRat b{a};

        CHECK(b == 2);

        ApproxRat c{a};
        CHECK(c == 2);
      }

      SUBCASE("FixedRational(IntT)")
      {
        MyRationalT a{23u}; // Different type than underlying type.

        CHECK(a == 23);

        ApproxRat b{23};
        CHECK(b == 23);
      }

      SUBCASE("FixedRational(IntT, IntT)")
      {
        FixedRational<int, 12> a{2, 3};

        CHECK(a.numerator() == 8);
        CHECK(a.denominator() == 12);

        SUBCASE("Different integer type")
        {
          FixedRational<int8_t, 12> a{1024, 512};

          CHECK(a == 2);
        }

        SUBCASE("Exceptional")
        {
          try {
            FixedRational<int, 12> a{3, 17};
            CHECK(false);
          }
          catch (unrepresentable_operation_error<int> e) {
            using namespace std::literals;
            // Because it's vendor dependent:
            auto int_name = typeid(int).name();
            CHECK(std::string(e.what())
                  == "Inexact construction of a FixedRational<"s + int_name
                         + ", 12>"s);
          }
        }

        SUBCASE("Approximate")
        {
          ApproxRat inexact{3, 17};
          ApproxRat expected{1, 6};
          CHECK(expected == inexact);
        }
      }

      SUBCASE("FixedRational(long double)")
      {
        MyRationalT a{23.0L};
        CHECK(a == 23);

        MyRationalT expected{51, 50};
        MyRationalT b{51.0L / 50};
        CHECK(expected == b);

        ApproxRat c{1.0L};
        CHECK(c == 1);
      }

      SUBCASE("FixedRational(double)")
      {
        MyRationalT a{23.0};
        CHECK(a == 23);

        MyRationalT expected{51, 50};
        MyRationalT b{1.02};
        CHECK(expected == b);

        ApproxRat c{1.0};
        CHECK(c == 1);
      }

      SUBCASE("FixedRational(float)")
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

        FixedRational<int, 100> expected_smaller_denom{51, 50};
        FixedRational<int, 100> b_smaller_denom{51.0f / 50};
        CHECK(expected_smaller_denom == b_smaller_denom);

        ApproxRat c{1.0f};
        CHECK(c == 1);
      }
    }

    SUBCASE("Accessors")
    {
      FixedRational<int, 12> a{6};
      ApproxRat b{6};

      SUBCASE("numerator()")
      {
        CHECK(a.numerator() == 72);
        CHECK(b.numerator() == 72);
      }

      SUBCASE("denominator()")
      {
        CHECK(a.denominator() == 12);
        CHECK(b.denominator() == 12);
      }

      SUBCASE("as_long_double()")
      {
        MyRationalT a{3, 2};
        ApproxRat b{3, 2};

        CHECK(a.as_long_double() == doctest::Approx(1.5));
        CHECK(b.as_long_double() == doctest::Approx(1.5));
      }

      SUBCASE("as_simplified()")
      {
        FixedRational<int, 12> a{3};
        auto result_a = a.as_simplified();

        CHECK(result_a.first == 3);
        CHECK(result_a.second == 1);

        FixedRational<int, 8> b{2, 4};
        auto result_b = b.as_simplified();
        CHECK(result_b.first == 1);
        CHECK(result_b.second == 2);

        ApproxRat c{3};
        auto result_c = c.as_simplified();
        CHECK(result_c.first == 3);
        CHECK(result_c.second == 1);
      }
    }

    SUBCASE("Functions")
    {
      MyRationalT a(5, 7);
      MyRationalT b(5, 7);
      MyRationalT c(-5, 7);
      CHECK(abs(a) == a);
      CHECK(abs(a) == b);
      CHECK(a == abs(c));

      // sanity check, lol
      CHECK(abs(c) != c);
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

        ApproxRat b{};
        REQUIRE(b == 0);

        CHECK(++b == 1);
        CHECK(b++ == 1);
        CHECK(b == 2);
      }

      SUBCASE("operator--")
      {
        MyRationalT a{};
        REQUIRE(a == 0);

        CHECK(--a == -1);
        CHECK(a-- == -1);
        CHECK(a == -2);

        ApproxRat b{};
        REQUIRE(b == 0);

        CHECK(--b == -1);
        CHECK(b-- == -1);
        CHECK(b == -2);
      }

      SUBCASE("unary operator-")
      {
        MyRationalT a{1};
        CHECK(-a == -1);

        ApproxRat b{1};
        CHECK(-b == -1);
      }
    }
  }

  SUBCASE("Related Operators")
  {
    SUBCASE("Comparison")
    {
      SUBCASE("==")
      {
        SUBCASE("FixedRational == int")
        {
          MyRationalT a{23};
          CHECK(a == 23);

          ApproxRat b{23};
          CHECK(b == 23);
        }

        SUBCASE("int == FixedRational")
        {
          MyRationalT a{23};
          CHECK(23 == a);

          ApproxRat b{23};
          CHECK(23 == b);
        }

        SUBCASE("FixedRational<same> == FixedRational<same>")
        {
          MyRationalT a{23};
          MyRationalT b{23};
          MyRationalT c{57};

          CHECK(a == a);
          CHECK(a == b);
          CHECK(b == a);
          CHECK_FALSE(b == c);

          ApproxRat d{23};
          CHECK(d == d);
        }
      }

      SUBCASE("!=")
      {
        SUBCASE("FixedRational != int")
        {
          MyRationalT a{23};
          CHECK(a != 24);

          ApproxRat b{23};
          CHECK(b != 24);
        }

        SUBCASE("int != FixedRational")
        {
          MyRationalT a{23};
          CHECK(24 != a);

          ApproxRat b{23};
          CHECK(24 != b);
        }

        SUBCASE("FixedRational<same> != FixedRational<same>")
        {
          MyRationalT a{23};
          MyRationalT b{23};
          MyRationalT c{57};

          CHECK(a != c);
          CHECK(c != a);
          CHECK_FALSE(a != b);

          ApproxRat d{23};
          CHECK_FALSE(d != d);
        }
      }

      SUBCASE("<")
      {
        SUBCASE("FixedRational < int")
        {
          MyRationalT a{5};
          CHECK(a < 6);
          CHECK_FALSE(a < 4);

          using SmallerRat = FixedRational<int, 12>;
          SmallerRat b{11, 12};
          SmallerRat c{12, 12};
          SmallerRat d{13, 12};

          CHECK(b < 1);
          CHECK_FALSE(c < 1);
          CHECK_FALSE(d < 1);

          CHECK_FALSE(-b < -1);
          CHECK_FALSE(-c < -1);
          CHECK(-d < -1);

          ApproxRat e{5};
          CHECK(e < 6);
        }

        SUBCASE("int < FixedRational")
        {
          MyRationalT a{5};
          CHECK(4 < a);
          CHECK_FALSE(6 < a);

          using SmallerRat = FixedRational<int, 12>;
          SmallerRat b{11, 12};
          SmallerRat c{12, 12};
          SmallerRat d{13, 12};

          CHECK_FALSE(1 < b);
          CHECK_FALSE(1 < c);
          CHECK(1 < d);

          CHECK(-1 < -b);
          CHECK_FALSE(-1 < -c);
          CHECK_FALSE(-1 < -d);

          ApproxRat e{5};
          CHECK(4 < e);
        }

        SUBCASE("FixedRational<same> < FixedRational<same>")
        {
          MyRationalT a{5};
          MyRationalT b{5};
          MyRationalT c{7};

          CHECK(a < c);
          CHECK_FALSE(a < b);

          ApproxRat d{5};
          CHECK_FALSE(d < d);
        }
      }

      SUBCASE(">")
      {
        SUBCASE("FixedRational > int")
        {
          MyRationalT a{5};
          CHECK(a > 4);
          CHECK_FALSE(a > 6);

          ApproxRat b{5};
          CHECK(b > 4);
        }

        SUBCASE("int > FixedRational")
        {
          MyRationalT a{5};
          CHECK(6 > a);
          CHECK_FALSE(4 > a);

          ApproxRat b{5};
          CHECK(6 > b);
        }

        SUBCASE("FixedRational<same> > FixedRational<same>")
        {
          MyRationalT a{5};
          MyRationalT b{5};
          MyRationalT c{7};

          CHECK(c > a);
          CHECK_FALSE(a > b);

          ApproxRat d{5};
          CHECK_FALSE(d > d);
        }
      }

      SUBCASE("<=")
      {
        SUBCASE("FixedRational <= int")
        {
          MyRationalT a{5};
          CHECK(a <= 6);
          CHECK(a <= 5);
          CHECK_FALSE(a <= 4);

          ApproxRat b{5};
          CHECK(b <= 5);
        }

        SUBCASE("int <= FixedRational")
        {
          MyRationalT a{5};
          CHECK(4 <= a);
          CHECK(5 <= a);
          CHECK_FALSE(6 <= a);

          ApproxRat b{5};
          CHECK(5 <= b);
        }

        SUBCASE("FixedRational<same> <= FixedRational<same>")
        {
          MyRationalT a{5};
          MyRationalT b{5};
          MyRationalT c{7};

          CHECK(a <= c);
          CHECK(a <= b);

          ApproxRat d{5};
          CHECK(d <= d);
        }
      }

      SUBCASE(">=")
      {
        SUBCASE("FixedRational >= int")
        {
          MyRationalT a{5};
          CHECK(a >= 4);
          CHECK(a >= 5);
          CHECK_FALSE(a >= 6);

          ApproxRat b{5};
          CHECK(b >= 5);
        }

        SUBCASE("int >= FixedRational")
        {
          MyRationalT a{5};
          CHECK(6 >= a);
          CHECK(5 >= a);
          CHECK_FALSE(4 >= a);

          ApproxRat b{5};
          CHECK(5 >= b);
        }

        SUBCASE("FixedRational<same> >= FixedRational<same>")
        {
          MyRationalT a{5};
          MyRationalT b{5};
          MyRationalT c{7};

          CHECK(c >= a);
          CHECK(a >= b);

          ApproxRat d{5};
          CHECK(d >= d);
        }
      }
    }

    SUBCASE("Arithmetic")
    {
      SUBCASE("binary *")
      {
        SUBCASE("FixedRational * int")
        {
          MyRationalT a{2};

          CHECK(a * 3 == 6);

          MyRationalT b{2, 3};

          std::string result_type_name{typeid(b * 3).name()};
          std::string b_type_name{typeid(b).name()};
          CHECK(b_type_name == result_type_name);

          CHECK(2 == b * 3);

          ApproxRat c{3};
          CHECK(c * 1 == c);
        }

        SUBCASE("int * FixedRational")
        {
          MyRationalT a{2};

          CHECK(3 * a == 6);

          MyRationalT b{2, 3};

          std::string result_type_name{typeid(3 * b).name()};
          std::string b_type_name{typeid(b).name()};
          CHECK(b_type_name == result_type_name);

          CHECK(2 == 3 * b);

          ApproxRat c{3};
          CHECK(1 * c == c);
        }

        SUBCASE("FixedRational<same> * FixedRational<same>")
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
            using SmallerRat = FixedRational<int, 12>;

            SmallerRat a{1, 3};
            SmallerRat b{2, 3};

            CHECK_THROWS_AS(a * b, unrepresentable_operation_error<int>);

            // Doesn't throw
            CHECK((3 * a) * b == b);

            try {
              // clang-format off
              a * b;
              // clang-format on
              CHECK(false);
            }
            catch (unrepresentable_operation_error<int> e) {
              CHECK(e.get_minimum_fix_factor() == 3);
              auto expected = "Inexact operation in ("s + typeid(a).name()
                              + " 4/12 * "s + typeid(b).name()
                              + " 8/12):  8/3 -> "s + typeid(int).name();
              CHECK(expected == std::string(e.what()));
            }
          }

          SUBCASE("Approximate")
          {
            ApproxRat a{1, 3};
            ApproxRat expected{1, 12};

            // 1/9 is unrepresentable.
            CHECK(a * a == expected);
          }
        }
      }

      SUBCASE("/")
      {
        SUBCASE("FixedRational / int")
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
            using RatI18 = FixedRational<int, 18>;
            RatI18 a{1};
            CHECK_THROWS_AS(a / 27, unrepresentable_operation_error<int>);

            // Doesn't throw
            RatI18 b{3};
            RatI18 expected{1, 9};
            CHECK(expected == b / 27);

            try {
              a / 27;
              CHECK(false);
            }
            catch (unrepresentable_operation_error<int> e) {
              CHECK(e.get_minimum_fix_factor() == 3);
              auto expected = "Inexact operation in ("s + typeid(a).name()
                              + " 18/18 / "s + typeid(int).name()
                              + " 27):  2/3 -> "s + typeid(int).name();
              CHECK(expected == std::string(e.what()));
            }
          }

          SUBCASE("Approximate")
          {
            ApproxRat a{1, 3};
            ApproxRat expected{1, 12};

            CHECK(a / 3 == expected);
          }
        }

        SUBCASE("int / FixedRational")
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
            using RatI18 = FixedRational<int, 18>;
            RatI18 a{5, 18};
            CHECK_THROWS_AS(1 / a, unrepresentable_operation_error<int>);

            // Doesn't throw
            CHECK(18 == 5 / a);

            try {
              1 / a;
              CHECK(false);
            }
            catch (unrepresentable_operation_error<int> e) {
              CHECK(e.get_minimum_fix_factor() == 5);
              auto expected = "Inexact operation in ("s + typeid(int).name()
                              + " 1 / "s + typeid(a).name()
                              + " 5/18):  324/5 -> "s + typeid(int).name();
              CHECK(expected == std::string(e.what()));
            }
          }

          SUBCASE("Approximate")
          {
            ApproxRat a{9};
            ApproxRat expected{1, 12};

            CHECK(1 / a == expected);
          }
        }

        SUBCASE("FixedRational<same> / FixedRational<same>")
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

          SUBCASE("Exceptional")
          {
            using RatI18 = FixedRational<int, 18>;
            RatI18 a{5, 18};
            RatI18 b{1};
            CHECK_THROWS_AS(b / a, unrepresentable_operation_error<int>);

            // Doesn't throw
            CHECK(18 == (5 * b) / a);

            try {
              b / a;
              CHECK(false);
            }
            catch (unrepresentable_operation_error<int> e) {
              CHECK(e.get_minimum_fix_factor() == 5);
              auto expected = "Inexact operation in ("s + typeid(b).name()
                              + " 18/18 / "s + typeid(a).name()
                              + " 5/18):  324/5 -> "s + typeid(int).name();
              CHECK(expected == std::string(e.what()));
            }
          }

          SUBCASE("Approximate")
          {
            ApproxRat a{1, 3};
            ApproxRat b{3};
            ApproxRat expected{1, 12};

            CHECK(a / b == expected);
          }
        }
      }

      SUBCASE("%")
      {
        SUBCASE("FixedRational<same> % FixedRational<same>")
        {
          MyRationalT a{116};
          MyRationalT b{50};

          CHECK(a % b == 16);

          ApproxRat c{116};
          ApproxRat d{50};

          CHECK(c % d == 16);
        }
      }

      SUBCASE("binary +")
      {
        SUBCASE("FixedRational + int")
        {
          MyRationalT a{2};

          CHECK(a + 1 == 3);

          MyRationalT b{2, 3};
          MyRationalT expected{5, 3};

          std::string result_type_name{typeid(b + 1).name()};
          std::string b_type_name{typeid(b).name()};
          CHECK(b_type_name == result_type_name);

          CHECK(expected == b + 1);

          ApproxRat c{2};
          CHECK(c + 1 == 3);
        }

        SUBCASE("int + FixedRational")
        {
          MyRationalT a{2};

          CHECK(1 + a == 3);

          MyRationalT b{2, 3};
          MyRationalT expected{5, 3};

          std::string result_type_name{typeid(1 + b).name()};
          std::string b_type_name{typeid(b).name()};
          CHECK(b_type_name == result_type_name);

          CHECK(expected == 1 + b);

          ApproxRat c{2};
          CHECK(1 + c == 3);
        }

        SUBCASE("FixedRational<same> + FixedRational<same>")
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

          ApproxRat d{2};
          CHECK(d + d == 4);
        }
      }

      SUBCASE("binary -")
      {
        SUBCASE("FixedRational - int")
        {
          MyRationalT a{3};
          CHECK(a - 2 == 1);

          ApproxRat b{3};
          CHECK(b - 2 == 1);
        }

        SUBCASE("int - FixedRational")
        {
          MyRationalT a{2};
          CHECK(3 - a == 1);

          ApproxRat b{2};
          CHECK(3 - b == 1);
        }

        SUBCASE("FixedRational - FixedRational")
        {
          MyRationalT a{3};
          MyRationalT b{2};

          CHECK(a - b == 1);

          ApproxRat c{2};
          CHECK(c - c == 0);
        }
      }
    }

    SUBCASE("ostream output")
    {
      std::stringstream a{};

      a << FixedRational<int, 4, true>{1, 4};
      CHECK(a.str() == "1/4");

      a.str("");
      a << FixedRational<int, 12>{1, 3};
      CHECK(a.str() == "4/12");

      a.str("");
      a << FixedRational<int, 4, false>{1, 4};
      CHECK(a.str() == "1/4");
    }
  }
}


} // namespace rational_geometry

// vim:set et ts=2 sw=2 sts=2:

