
#include "../src/rational_geometry/Matrix.hpp"

#include "doctest.h"

#include <string>
#include <typeinfo>

namespace rational_geometry {


TEST_CASE("Testing Matrix.hpp")
{
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

    SUBCASE("Getters")
    {
      SUBCASE("get_row(size_t) const")
      {
        // clang-format off
        Matrix<int, 2> a{
            {1, 2},
            {3, 4}};
        // clang-format on

        Point<int, 2> row_0{1, 2};
        Point<int, 2> row_1{3, 4};

        CHECK(a.get_row(0) == row_0);
        CHECK(a.get_row(1) == row_1);
      }

      SUBCASE("get_column(size_t) const")
      {
        // clang-format off
        Matrix<int, 2> a{
            {1, 2},
            {3, 4}};
        // clang-format on

        Point<int, 2> column_0{1, 3};
        Point<int, 2> column_1{2, 4};

        CHECK(a.get_column(0) == column_0);
        CHECK(a.get_column(1) == column_1);
      }
    }
  }
}


} // namespace rational_geometry

// vim:set et ts=2 sw=2 sts=2:

