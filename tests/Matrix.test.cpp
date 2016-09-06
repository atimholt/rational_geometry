
#include "../src/rational_geometry/Matrix.hpp"

#include "doctest.h"

#include <string>
#include <typeinfo>

namespace rational_geometry {


TEST_CASE("Testing Matrix.hpp")
{
  SUBCASE("Testing Matrix<> class")
  {
    SUBCASE("Testing Matrix<>::Matrix()")
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
  }
}


} // namespace rational_geometry

// vim:set et ts=2 sw=2 sts=2:

