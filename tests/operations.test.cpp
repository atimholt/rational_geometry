#include "../src/rational_geometry/Operations.hpp"

#include "doctest.h"

#include <array>
#include <string>
#include <typeinfo>
#include <vector>

namespace rational_geometry {


TEST_CASE("Testing Operations.hpp")
{
  using std::array;

  SUBCASE("dot(...)") // aka scalar product
  {
    using IArray2 = array<int, 2>;

    IArray2 i{1, 0};
    IArray2 j{0, 1};

    // auto orthogonal = dot(3 * i + 4 * j, -4 * i + 3 * j);
    auto orthogonal = dot(IArray2{3, 4}, IArray2{-4, 3});
    CHECK(0 == orthogonal);

    // auto something_different = dot(3 * i, 2 * i);
    auto something_different = dot(IArray2{3, 0}, IArray2{2, 0});
    CHECK(6 == something_different);

    auto i_j = dot(i, j);

    CHECK(0 == i_j);
  }

  SUBCASE("cross(...)") // aka vector product. 3D only.
  {
    typedef std::array<int, 3> IArray3;

    IArray3 i{1, 0, 0};
    IArray3 j{0, 1, 0};
    IArray3 k{0, 0, 1};

    CHECK(i == cross(j, k));
    CHECK(j == cross(k, i));
    CHECK(k == cross(i, j));

    // Non-commutative
    IArray3 i_n{-1, 0, 0};
    IArray3 j_n{0, -1, 0};
    IArray3 k_n{0, 0, -1};

    CHECK(i_n == cross(k, j));
    CHECK(j_n == cross(i, k));
    CHECK(k_n == cross(j, i));
  }
}


} // namespace rational_geometry

// vim:set et ts=2 sw=2 sts=2:

