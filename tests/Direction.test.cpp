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

        CHECK(a.get(0) == 0);
        CHECK(a.get(1) == 0);
      }

      SUBCASE("Direction(std::initializer_list<SignedIntT>)")
      {
        Direction3D val_a{1, 2, 3};
        CHECK(val_a.get_dimensionality() == 3);

        Direction3D val_b{2, 5};
        CHECK(val_b.get(1) == 5);
        CHECK(val_b.get(2) == 0);

        SUBCASE("normalization")
        {
          Direction3D val_c{2, 4, 6};
          CHECK(val_c.get(0) == 1);
          CHECK(val_c.get(1) == 2);
          CHECK(val_c.get(2) == 3);

          Direction3D val_d{-2, 4, 6};
          CHECK(val_d.get(0) == -1);
          CHECK(val_d.get(1) == 2);
          CHECK(val_d.get(2) == 3);

          Direction3D val_e{-2, 0, 6};
          CHECK(val_e.get(0) == -1);
          CHECK(val_e.get(1) == 0);
          CHECK(val_e.get(2) == 3);

          Direction3D val_f{-2, -4, -6};
          CHECK(val_f.get(0) == -1);
          CHECK(val_f.get(1) == -2);
          CHECK(val_f.get(2) == -3);
        }
      }

      SUBCASE("Direction(std::array<>)")
      {
        std::array<int, 3> argument{1, 2, 3};

        Direction3D val_a{argument};

        CHECK(val_a.get_dimensionality() == 3);
      }

      SUBCASE("Direction(std::initializer_list<std::pair>)")
      {
        Direction3D a{{1, 6}, {1, 4}, {-1, 3}};
        CHECK(a.get(0) == 2);
        CHECK(a.get(1) == 3);
        CHECK(a.get(2) == -4);

        Direction3D b{{0, 1}, {1, 4}, {1, 6}};
        CHECK(b.get(0) == 0);
        CHECK(b.get(1) == 3);
        CHECK(b.get(2) == 2);
      }
    }

    SUBCASE("accessors")
    {
      SUBCASE("get()")
      {
        std::array<int, 3> an_array{3, 4, 5};
        Direction3D a_direction{3, 4, 5};

        CHECK(an_array == a_direction.get());

        SUBCASE("Invariants in effect")
        {
          Direction3D equivalent_direction{6, 8, 10};

          CHECK(an_array == equivalent_direction.get());
        }
      }

      SUBCASE("get(size_t)")
      {
        Direction3D a_value{4, 5, 6};

        CHECK(a_value.get(0) == 4);
        CHECK(a_value.get(1) == 5);
        CHECK(a_value.get(2) == 6);
      }

      SUBCASE("first_present_dimension()")
      {
        Direction3D val_a{1, 1, 0};
        CHECK(val_a.first_present_dimension() == 0);

        Direction3D val_b{0, 1, 0};
        CHECK(val_b.first_present_dimension() == 1);

        Direction3D val_c{0, 0, 5};
        CHECK(val_c.first_present_dimension() == 2);

        Direction3D val_d{0, 0, 0};
        CHECK(val_d.first_present_dimension() == 3); // 1 past the end!
      }
    }
  }
}


} // namespace rational_geometry

// vim:set et ts=2 sw=2 sts=2:

