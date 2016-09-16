
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
    SUBCASE("Constructors")
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
        Matrix<int, 3, 2> a{
            {11, 12},
            {21, 22},
            {31, 32}};
        // clang-format on

        CHECK(a.values_.size() == 3);
        CHECK(a.values_[0].size() == 2);

        CHECK(a.values_[0][1] == 12);
      }
    }

    // clang-format off
    IMat2 a{
        {1, 2},
        {3, 4}};

    Matrix<int, 2, 3> rectangular{
        {1,2,3},
        {4,5,6}};

    Matrix<int, 3, 2> vert_rectangular{
        {1,4},
        {2,5},
        {3,6}};
    // clang-format on

    SUBCASE("Getters")
    {
      SUBCASE("get_row(size_t) const")
      {
        Point<int, 2> row_0{1, 2};
        Point<int, 2> row_1{3, 4};

        CHECK(a.get_row(0) == row_0);
        CHECK(a.get_row(1) == row_1);

        SUBCASE("rectangular")
        {
          Point<int, 3> row_0{1, 2, 3};
          Point<int, 3> row_1{4, 5, 6};

          CHECK(rectangular.get_row(0) == row_0);
          CHECK(rectangular.get_row(1) == row_1);
        }
      }

      SUBCASE("get_column(size_t) const")
      {
        Point<int, 2> column_0{1, 3};
        Point<int, 2> column_1{2, 4};

        CHECK(a.get_column(0) == column_0);
        CHECK(a.get_column(1) == column_1);

        SUBCASE("rectangular")
        {
          Point<int, 2> column_0{1, 4};
          Point<int, 2> column_1{2, 5};
          Point<int, 2> column_2{3, 6};

          CHECK(rectangular.get_column(0) == column_0);
          CHECK(rectangular.get_column(1) == column_1);
          CHECK(rectangular.get_column(2) == column_2);
        }
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
        CHECK(a.values_[0][1] == 6);
        CHECK(a.values_[1][0] == 3);
        CHECK(a.values_[1][1] == 4);

        SUBCASE("rectangular")
        {
          Point<int, 3> new_row{7, 8, 9};
          rectangular.set_row(1, new_row);

          CHECK(rectangular.values_[1][2] == 9);
        }
      }

      SUBCASE("set_column(Point<>, int)")
      {
        Point<int, 2> new_column{5, 6};
        a.set_column(0, new_column);

        /// \todo:
        // a.set_column(0, {5, 6});

        CHECK(a.values_[0][0] == 5);
        CHECK(a.values_[0][1] == 2);
        CHECK(a.values_[1][0] == 6);
        CHECK(a.values_[1][1] == 4);

        SUBCASE("rectangular")
        {
          Point<int, 3> new_column{7, 8, 9};
          vert_rectangular.set_column(1, new_column);

          CHECK(vert_rectangular.values_[2][1] == 9);
        }
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

      SUBCASE("Different member types")
      {
        Matrix<char, 2> a_char{{1, 2}, {3, 4}};

        CHECK(a == a_char);

        SUBCASE("Logical inverse")
        {
          CHECK_FALSE(not_a == a_char);
        }
      }
    }

    SUBCASE("Matrix<> != Matrix<>")
    {
      CHECK(a != not_a);

      SUBCASE("Logical inverse")
      {
        CHECK_FALSE(a != another_a);
      }

      SUBCASE("Different member types")
      {
        Matrix<char, 2> a_char{{1, 2}, {3, 4}};

        CHECK(not_a != a_char);

        SUBCASE("Logical inverse")
        {
          CHECK_FALSE(a != a_char);
        }
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
          {4, 3},
          {4, 4}};

      IMat2 lesser10 {
          {4, 4},
          {3, 4}};

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

      SUBCASE("Different member types")
      {
        Matrix<char, 2> arbitrary_char{{4, 4}, {4, 4}};

        CHECK(lesser00 < arbitrary_char);

        SUBCASE("Logical inverse")
        {
          CHECK_FALSE(arbitrary_char < lesser00);
        }
      }
    }

    SUBCASE("Matrix<> * Matrix<>")
    {
      // clang-format off
      Matrix<int, 2, 3> lop_mat{
          {1, 2, 3},
          {4, 5, 6}};

      Matrix<int, 3, 2> rop_mat{
          { 7,  8},
          { 9, 10},
          {11, 12}};

      Matrix<int, 2, 2> expected{
          { 58,  64},
          {139, 154}};
      // clang-format on

      auto answer = lop_mat * rop_mat;

      CHECK(expected == answer);

      SUBCASE("Different member types")
      {
        // clang-format off
        Matrix<char, 2> char_rot{
            {0,1},
            {1,0}};
        // clang-format on

        auto result = char_rot * a;

        std::string expected_type{typeid(Matrix<int, 2>).name()};
        CHECK(expected_type == typeid(result).name());

        expected_type = typeid(Matrix<char, 2>).name();
        CHECK(expected_type != typeid(result).name());
      }
    }

    SUBCASE("Matrix<> * Point<>")
    {
      typedef Matrix<int, 3> IMat3;
      typedef Point<int, 2> IPoint2;
      // clang-format off
      const IMat3 identity{
          {1, 0, 0},
          {0, 1, 0},
          {0, 0, 1}};

      const IMat3 translator{
          {1, 0, 2},
          {0, 1, 3},
          {0, 0, 1}};

      const IMat3 rotator{
          {0,-1, 0},
          {1, 0, 0},
          {0, 0, 1}};

      const IMat3 scaler{
          {5, 0, 0},
          {0, 5, 0},
          {0, 0, 1}};
      // clang-format on

      IPoint2 origin{0, 0};

      auto translated = translator * origin.as_point();
      IPoint2 translated_expected{2, 3};
      REQUIRE(translated.as_simpler() == translated_expected);

      auto rotated = rotator * translated;
      IPoint2 rotated_expected{-3, 2};
      REQUIRE(rotated.as_simpler() == rotated_expected);

      auto scaled = scaler * rotated;
      IPoint2 scaled_expected{-15, 10};
      REQUIRE(scaled.as_simpler() == scaled_expected);

      auto identitied = identity * scaled;
      IPoint2 identitied_expected{-15, 10};
      REQUIRE(identitied.as_simpler() == identitied_expected);
      REQUIRE(identitied == scaled);

      auto all_together =
          identity * scaler * rotator * translator * origin.as_point();
      IPoint2 all_together_expected = identitied.as_simpler();
      CHECK(all_together.as_simpler() == all_together_expected);
    }
  }

  SUBCASE("Related functions")
  {
    SUBCASE("make_translation()")
    {
      Point<int, 2> a{2, 3};

      auto mat_a = make_translation(a);
      // clang-format off
      Matrix<int, 3> expected{
          {1, 0, 2},
          {0, 1, 3},
          {0, 0, 1}};
      // clang-format on

      CHECK(expected == mat_a);
    }

    SUBCASE("make_rotation()")
    {
      typedef std::array<Point<int, 2>, 2> Rotation;

      Point<int, 2> i{0, 1};
      Point<int, 2> j{-1, 0};
      auto result = make_rotation(Rotation{i, j});

      // clang-format off
      Matrix<int, 3> expected{
          {0, -1, 0},
          {1,  0, 0},
          {0,  0, 1}};
      // clang-format on

      CHECK(expected == result);
    }

    SUBCASE("make_scale()")
    {
      const auto kSize = 2;
      auto a           = make_scale<kSize>(5);

      // clang-format off
      Matrix<int, kSize + 1> expected{
          {5, 0, 0},
          {0, 5, 0},
          {0, 0, 1}};
      // clang-format on

      CHECK(expected == a);
    }
  }
}


} // namespace rational_geometry

// vim:set et ts=2 sw=2 sts=2:

