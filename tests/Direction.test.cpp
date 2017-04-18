#include "../src/rational_geometry/Direction.hpp"

#include "doctest.h"

#include <string>
#include <typeinfo>

namespace rational_geometry {


TEST_CASE("Testing Direction.hpp")
{
  using Direction3D = Direction<int, 3>;
  using Direction2D = Direction<int, 2>;

  const Direction3D null_dir_3{0, 0, 0};

  const Direction3D x3{1, 0, 0};
  const Direction3D y3{0, 1, 0};
  const Direction3D z3{0, 0, 1};

  const Direction2D null_dir_2{0, 0};
  const Direction2D arb_vals{1, 7};

  const Direction2D x2{1, 0};
  const Direction2D y2{0, 1};

  SUBCASE("Direction<> class")
  {
    SUBCASE("constructors")
    {
      SUBCASE("Direction()")
      {
        Direction2D a{};

        CHECK(a[0] == 0);
        CHECK(a[1] == 0);
      }

      SUBCASE("Direction(std::initializer_list)")
      {
        Direction3D val_a{1, 2, 3};
        CHECK(val_a.size() == 3);

        Direction3D val_b{2, 5};
        CHECK(val_b[1] == 5);
        CHECK(val_b[2] == 0);

        SUBCASE("normalization")
        {
          Direction3D val_c{2,4,6};
          CHECK(val_c [0] == 1);
          CHECK(val_c [1] == 2);
          CHECK(val_c [2] == 3);

          Direction3D val_d{-2,4,6};
          CHECK(val_d [0] == -1);
          CHECK(val_d [1] == 2);
          CHECK(val_d [2] == 3);

          Direction3D val_e{-2,0,6};
          CHECK(val_e [0] == -1);
          CHECK(val_e [1] == 0);
          CHECK(val_e [2] == 3);

          Direction3D val_f{-2,-4,-6};
          CHECK(val_f [0] == -1);
          CHECK(val_f [1] == -2);
          CHECK(val_f [2] == -3);
        }
      }

      SUBCASE("Direction(std::array<>)")
      {
        std::array<int, 3> argument{1, 2, 3};

        Direction3D val_a{argument};

        CHECK(val_a.size() == 3);
      }
    }

    SUBCASE("accessors")
    {
      SUBCASE("first_present_dimension()")
      {
        Direction3D val_a{1,1,0};
        CHECK(val_a.first_present_dimension() == 0);

        Direction3D val_b{0,1,0};
        CHECK(val_b.first_present_dimension() == 1);

        Direction3D val_c{0,0,5};
        CHECK(val_c.first_present_dimension() == 2);

        Direction3D val_d{0,0,0};
        CHECK(val_d.first_present_dimension() == 3); // 1 past the end!
      }
#if 0
      SUBCASE("as_point()")
      {
        static const size_t dimension        = 3;
        static const size_t result_dimension = dimension + 1;

        Point<int, dimension> val_a{1, 2, 3};

        auto val_b = val_a.as_point();

        CHECK(val_b.size() == result_dimension);
        CHECK(val_b[result_dimension - 1] == 1);

        std::string expected_value{typeid(Point<int, 4>).name()};
        CHECK(expected_value == typeid(val_b).name());
      }

      SUBCASE("as_vector()")
      {
        static const size_t dimension        = 3;
        static const size_t result_dimension = dimension + 1;

        Point<int, dimension> val_a{1, 2, 3};

        auto val_b = val_a.as_vector();

        CHECK(val_b.size() == result_dimension);
        CHECK(val_b[result_dimension - 1] == 0);

        std::string expected_value{typeid(Point<int, 4>).name()};
        CHECK(expected_value == typeid(val_b).name());
      }

      SUBCASE("as_simpler()")
      {
        IPoint3D val_a{1, 2, 3};
        IPoint2D expected{1, 2};

        CHECK(expected == val_a.as_simpler());

        // They can't even == compare if they're not the same type, but who
        // <i>knows</i> what the future could bring?
        std::string expected_type{typeid(Point<int, 2>).name()};
        CHECK(expected_type == typeid(expected).name());
      }


      auto a = origin2;
      const IPoint2D expected{1, 7};

      SUBCASE("Point[size_t]")
      {
        a[0] = 1;
        a[1] = 7;

        CHECK(expected == a);
      }

      SUBCASE("at(size_t)")
      {
        a.at(0) = 1;
        a.at(1) = 7;

        CHECK(expected == a);

        CHECK_THROWS_AS(a.at(2), std::out_of_range);
      }


      SUBCASE("get an iterator")
      {
        using namespace std;

        SUBCASE("begin()")
        {
          REQUIRE(a == origin2);

          auto it = begin(a);
          *(it++) = 1;
          *it     = 7;

          CHECK(expected == a);
        }

        SUBCASE("end()")
        {
          REQUIRE(a == origin2);

          auto it = end(a);
          --it;
          *(it--) = 7;
          *it     = 1;

          CHECK(expected == a);
        }

        SUBCASE("cbegin()")
        {
          auto it = cbegin(arb_vals);

          CHECK(*it == 1);
          ++it;
          CHECK(*it == 7);
        }

        SUBCASE("cend()")
        {
          auto it = cend(arb_vals);

          --it;
          CHECK(*it == 7);
          --it;
          CHECK(*it == 1);
        }
      }
#endif
    }
  }
}


} // namespace rational_geometry

// vim:set et ts=2 sw=2 sts=2:

