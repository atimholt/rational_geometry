
#include "../src/rational_geometry/Matrix.hpp"

#include "doctest.h"

#include <string>
#include <typeinfo>

namespace rational_geometry {


TEST_CASE("Testing Matrix.hpp")
{
  typedef Matrix<int, 2> IMat2;

  SUBCASE("Matrix<> class")
  {
    SUBCASE("Matrix()")
    {
      Matrix<int> a{};

      REQUIRE(a.values_.size() == 4);
      REQUIRE(a.values_[0].size() == 4);

      for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
          if (i == j) {
            CHECK(a.values_[i][j] == 1);
          }
          else {
            CHECK(a.values_[i][j] == 0);
          }
        }
      }

      Matrix<int, 3> b{};
      CHECK(b.values_.size() == 3);
      CHECK(b.values_[0].size() == 3);

      Matrix<int, 3, 5> c{};
      CHECK(c.values_.size() == 3);
      CHECK(c.values_[0].size() == 5);
    }

    SUBCASE("Matrix(initializer_list<array<>>)")
    {
      // clang-format off
      Matrix<int, 2, 3> a{
          {11, 12},
          {21, 22},
          {31, 32}};
      // clang-format on

      CHECK(a.values_.size() == 2);
      CHECK(a.values_[0].size() == 3);

      CHECK(a.values_[0][1] == 21);
    }

    // clang-format off
    IMat2 a{
        {1, 2},
        {3, 4}};
    // clang-format on

    SUBCASE("Getters")
    {
      SUBCASE("get_row(size_t) const")
      {
        Point<int, 2> row_0{1, 2};
        Point<int, 2> row_1{3, 4};

        CHECK(a.get_row(0) == row_0);
        CHECK(a.get_row(1) == row_1);
      }

      SUBCASE("get_column(size_t) const")
      {
        Point<int, 2> column_0{1, 3};
        Point<int, 2> column_1{2, 4};

        CHECK(a.get_column(0) == column_0);
        CHECK(a.get_column(1) == column_1);
      }
    }

    SUBCASE("Setters")
    {
      SUBCASE("set_row(Point<>, int)")
      {
        Point<int, 2> new_row{5, 6};
        a.set_row(0, new_row);

        /// \todo:
        // a.set_row(0, {5, 6});

        CHECK(a.values_[0][0] == 5);
        CHECK(a.values_[0][1] == 3);
        CHECK(a.values_[1][0] == 6);
        CHECK(a.values_[1][1] == 4);
      }

      SUBCASE("set_column(Point<>, int)")
      {
        Point<int, 2> new_column{5, 6};
        a.set_column(0, new_column);

        /// \todo:
        // a.set_column(0, {5, 6});

        CHECK(a.values_[0][0] == 5);
        CHECK(a.values_[0][1] == 6);
        CHECK(a.values_[1][0] == 2);
        CHECK(a.values_[1][1] == 4);
      }
    }
  }

  SUBCASE("Related operators")
  {
    // clang-format off
    IMat2 a{
        {1, 2},
        {3, 4}};

    IMat2 another_a{
        {1, 2},
        {3, 4}};

    IMat2 not_a{
        {1, 2},
        {3, 5}};
    // clang-format on

    SUBCASE("Matrix<> == Matrix<>")
    {
      CHECK(a == another_a);

      SUBCASE("Logical inverse")
      {
        CHECK_FALSE(a == not_a);
      }
    }

    SUBCASE("Matrix<> != Matrix<>")
    {
      CHECK(a != not_a);

      SUBCASE("Logical inverse")
      {
        CHECK_FALSE(a != another_a);
      }
    }

    SUBCASE("Matrix<> < Matrix<>")
    {
      // clang-format off
      IMat2 arbitrary {
          {4, 4},
          {4, 4}};

      IMat2 lesser00 {
          {3, 4},
          {4, 4}};

      IMat2 lesser01 {
          {4, 4},
          {3, 4}};

      IMat2 lesser10 {
          {4, 3},
          {4, 4}};

      IMat2 lesser11 {
          {4, 4},
          {4, 3}};
      // clang-format on

      CHECK(lesser00 < arbitrary);
      CHECK(lesser01 < arbitrary);
      CHECK(lesser10 < arbitrary);
      CHECK(lesser11 < arbitrary);

      SUBCASE("Logical inverse")
      {
        // clang-format off
        IMat2 arbitrary_2 {
            {4, 4},
            {4, 4}};
        // clang-format on
        CHECK_FALSE(arbitrary < arbitrary_2);

        CHECK_FALSE(arbitrary < lesser00);
        CHECK_FALSE(arbitrary < lesser01);
        CHECK_FALSE(arbitrary < lesser10);
        CHECK_FALSE(arbitrary < lesser11);
      }
    }
  }
}


} // namespace rational_geometry

// vim:set et ts=2 sw=2 sts=2:

