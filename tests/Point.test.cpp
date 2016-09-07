
#include "../src/rational_geometry/Point.hpp"

#include "doctest.h"

#include <string>
#include <typeinfo>

namespace rational_geometry {


TEST_CASE("Testing Point.hpp")
{
  typedef Point<int, 3> IPoint3D;
  typedef Point<int, 2> IPoint2D;

  /// \todo  Consider opening use of these to library users.
  const IPoint3D origin3{0, 0, 0};

  const IPoint3D i3{1, 0, 0};
  const IPoint3D j3{0, 1, 0};
  const IPoint3D k3{0, 0, 1};

  const IPoint2D origin2{0, 0};

  const IPoint2D i2{1, 0};
  const IPoint2D j2{0, 1};

  SUBCASE("Point<> class")
  {
    SUBCASE("Point<>::Point(std::initializer_list)")
    {
      IPoint3D val_a{1, 2, 3};
      CHECK(val_a.values_.size() == 3);

      IPoint3D val_b{2, 5};
      CHECK(val_b.values_[1] == 5);
      CHECK(val_b.values_[2] == 0);
    }

    SUBCASE("Point<>::Point(std::array<>)")
    {
      std::array<int, 3> argument{1, 2, 3};

      IPoint3D val_a{argument};

      CHECK(val_a.values_.size() == 3);
    }

    SUBCASE("Point<>::as_point()")
    {
      static const size_t dimension        = 3;
      static const size_t result_dimension = dimension + 1;

      Point<int, dimension> val_a{1, 2, 3};

      auto val_b = val_a.as_point();

      CHECK(val_b.values_.size() == result_dimension);
      CHECK(val_b.values_[result_dimension - 1] == 1);

      std::string expected_value{typeid(Point<int, 4>).name()};
      CHECK(expected_value == typeid(val_b).name());
    }

    SUBCASE("Point<>::as_vector()")
    {
      static const size_t dimension        = 3;
      static const size_t result_dimension = dimension + 1;

      Point<int, dimension> val_a{1, 2, 3};

      auto val_b = val_a.as_vector();

      CHECK(val_b.values_.size() == result_dimension);
      CHECK(val_b.values_[result_dimension - 1] == 0);

      std::string expected_value{typeid(Point<int, 4>).name()};
      CHECK(expected_value == typeid(val_b).name());
    }

    SUBCASE("Point<>::as_simpler()")
    {
      IPoint3D val_a{1, 2, 3};
      IPoint2D expected{1, 2};

      CHECK(expected == val_a.as_simpler());

      // They can't even == compare if they're not the same type, but who
      // <i>knows</i> what the future could bring?
      std::string expected_type{typeid(Point<int, 2>).name()};
      CHECK(expected_type == typeid(expected).name());
    }
  }

  SUBCASE("Point<> [comparator operator] Point<>")
  {
    IPoint3D all_ones{1, 1, 1};
    IPoint3D another_all_ones{1, 1, 1};

    IPoint3D unequal_value{1, 0, 1};

    IPoint3D larger_z{1, 1, 2};
    IPoint3D larger_y{1, 2, 0};
    IPoint3D larger_x{2, 0, 0};

    SUBCASE("Basic checks")
    {
      CHECK(all_ones == another_all_ones);
      CHECK(all_ones <= another_all_ones);
      CHECK(all_ones >= another_all_ones);

      CHECK(all_ones != unequal_value);

      CHECK(all_ones < larger_z);
      CHECK(all_ones <= larger_z);
      CHECK(all_ones < larger_y);
      CHECK(all_ones < larger_x);

      CHECK(larger_z > all_ones);
      CHECK(larger_z >= all_ones);
      CHECK(larger_y > all_ones);
      CHECK(larger_x > all_ones);
    }

    SUBCASE("Logical inverse")
    {
      CHECK_FALSE(all_ones == unequal_value);
      CHECK_FALSE(larger_z <= all_ones);
      CHECK_FALSE(all_ones >= larger_z);

      CHECK_FALSE(all_ones != another_all_ones);

      CHECK_FALSE(all_ones > larger_z);
      CHECK_FALSE(all_ones >= larger_z);
      CHECK_FALSE(all_ones > larger_y);
      CHECK_FALSE(all_ones > larger_x);

      CHECK_FALSE(larger_z < all_ones);
      CHECK_FALSE(larger_z <= all_ones);
      CHECK_FALSE(larger_y < all_ones);
      CHECK_FALSE(larger_x < all_ones);
    }

    SUBCASE("Different types")
    {
      Point<char, 3> c_all_ones{1, 1, 1};

      CHECK(c_all_ones == another_all_ones);
      CHECK(c_all_ones <= another_all_ones);
      CHECK(c_all_ones >= another_all_ones);

      CHECK(c_all_ones != unequal_value);

      CHECK(c_all_ones < larger_z);
      CHECK(c_all_ones <= larger_z);
      CHECK(c_all_ones < larger_y);
      CHECK(c_all_ones < larger_x);

      CHECK(larger_z > c_all_ones);
      CHECK(larger_z >= c_all_ones);
      CHECK(larger_y > c_all_ones);
      CHECK(larger_x > c_all_ones);
    }
  }

  SUBCASE("Point<> + Point<>")
  {
    IPoint3D a{origin3};
    IPoint3D b{1, 2, 3};
    IPoint3D c{10, 20, 30};
    IPoint3D d{4, 5, 6};

    for (const auto& val : {a, b, c, d}) {
      CHECK(val == val + origin3);
    }

    IPoint3D a_b{1, 2, 3};
    CHECK(a_b == a + b);

    IPoint3D b_b{2, 4, 6};
    CHECK(b_b == b + b);

    IPoint3D b_c{11, 22, 33};
    CHECK(b_c == b + c);

    IPoint3D b_d{5, 7, 9};
    CHECK(b_d == b + d);
  }

  SUBCASE("Point<> * RatT")
  {
    IPoint3D a{3, 5, 7};
    static const int factor{2};
    IPoint3D expected{6, 10, 14};

    CHECK(expected == a * factor);
  }

  SUBCASE("RatT * Point<>")
  {
    static const int factor{2};
    IPoint3D a{3, 5, 7};
    IPoint3D expected{6, 10, 14};

    CHECK(expected == factor * a);
  }

  SUBCASE("dot(Point<>, Point<>)")
  {
    IPoint2D i{i2};
    IPoint2D j{j2};

    auto i_j = dot(i, j);
    CHECK(0 == i_j);

    auto orthogonal = dot(3 * i + 4 * j, -4 * i + 3 * j);
    CHECK(0 == orthogonal);

    auto something_different = dot(3 * i, 2 * i);
    CHECK(6 == something_different);
  }

  SUBCASE("cross(Point<>, Point<>)")
  {
    static const IPoint3D i{i3};
    static const IPoint3D j{j3};
    static const IPoint3D k{k3};

    CHECK(i == cross(j, k));
    CHECK(j == cross(k, i));
    CHECK(k == cross(i, j));

    // Non-commutative
    CHECK(-1 * i == cross(k, j));
    CHECK(-1 * j == cross(i, k));
    CHECK(-1 * k == cross(j, i));
  }
}


} // namespace rational_geometry

// vim:set et ts=2 sw=2 sts=2:

