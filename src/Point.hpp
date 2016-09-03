// Point.hpp

#ifndef _RATIONAL_GEOMETRY_POINT_HPP_INCLUDED_
#define _RATIONAL_GEOMETRY_POINT_HPP_INCLUDED_

//│ ▼1 │ Preprocesser Flag Stuff
//└────┴─────────────────────────

#if !defined RATIONAL_GEOMETRY_THIS_IS_THE_TEST_BINARY                         \
    && !defined DOCTEST_CONFIG_DISABLE

#define DOCTEST_CONFIG_DISABLE

#endif

//│ ▼1 │ Includes
//└────┴──────────

#include <doctest.h>

#include <array>
#include <cstdarg>
#include <cstddef>

#ifndef DOCTEST_CONFIG_DISABLE
#include <string>
#include <typeinfo>
#endif

//┌────┬──────────
//│ ▲1 │ Includes

namespace rational_geometry {

//│ ▼1 │ Class Template Declaration
//└────┴────────────────────────────

//▼
/// A geometric point in n-space with rational number coordinates.
///
/// This class template should actually work with any type that has infinite
/// precision, or with any type in uses where the type is not used outside of
/// its full accuracy. One suggestion might be to throw an exception on a
/// inaccurate operation from your RatT type.
//▲
template <typename RatT, std::size_t kDimension = 3>
class Point
{
 public:
  // INTERNAL STATE
  std::array<RatT, kDimension> values_;

  // CONSTRUCTORS
  Point();
  Point(std::array<RatT, kDimension> values);
  Point(const Point<RatT, kDimension - 1>& smaller_point, RatT last);

  // ACCESSORS
  Point<RatT, kDimension + 1> as_point();
  Point<RatT, kDimension + 1> as_vector();
};

//│ ▼1 │ Convenience typedefs
//└────┴──────────────────────

template <typename RatT>
using Point3D = Point<RatT, 3>;

template <typename RatT>
using Point2D = Point<RatT, 2>;

//│ ▼1 │ Class Template Definitions
//└─┬──┴─┬──────────────────────────
//  │ ▼2 │ Constructors
//  └────┴──────────────

template <typename RatT, size_t kDimension>
Point<RatT, kDimension>::Point() : values_{}
{
}

template <typename RatT, size_t kDimension>
Point<RatT, kDimension>::Point(std::array<RatT, kDimension> values)
    : values_{values}
{
}
TEST_CASE("Testing Point<>::Point(std::array<>)")
{
  typedef Point3D<int> IPoint3D;

  IPoint3D val_a{{1, 2, 3}};

  CHECK(val_a.values_.size() == 3);
}

template <typename RatT, size_t kDimension>
Point<RatT, kDimension>::Point(
    const Point<RatT, kDimension - 1>& smaller_point, RatT last)
{
  using namespace std;

  copy(
      begin(smaller_point.values_), end(smaller_point.values_), begin(values_));
  *rbegin(values_) = last;
}
TEST_CASE(
    "Testing Point<RatT, kDimension>::Point(Point<RatT, kDimension - 1>, RatT)")
{
  typedef Point3D<int> IPoint3D;

  IPoint3D val_in{{1, 2, 3}};

  for (const auto& arbitrary_val : {-10, -1, 0, 1, 3, 50, 10'000}) {
    Point<int, 4> val_out{val_in, arbitrary_val};

    CHECK(val_out.values_[3] == arbitrary_val);
  }
}

//  │ ▼2 │ Accessors
//  └────┴───────────

template <typename RatT, size_t kDimension>
Point<RatT, kDimension + 1> Point<RatT, kDimension>::as_point()
{
  return {*this, 1};
}
TEST_CASE("Testing Point<>::as_point()")
{
  static const size_t dimension        = 3;
  static const size_t result_dimension = dimension + 1;

  Point<int, dimension> val_a{{1, 2, 3}};

  auto val_b = val_a.as_point();

  CHECK(val_b.values_.size() == result_dimension);
  CHECK(val_b.values_[result_dimension - 1] == 1);

  std::string expected_value{typeid(Point<int, 4>).name()};
  CHECK(expected_value == typeid(val_b).name());
}

template <typename RatT, size_t kDimension>
Point<RatT, kDimension + 1> Point<RatT, kDimension>::as_vector()
{
  return {*this, 0};
}
TEST_CASE("Testing Point<>::as_vector()")
{
  static const size_t dimension        = 3;
  static const size_t result_dimension = dimension + 1;

  Point<int, dimension> val_a{{1, 2, 3}};

  auto val_b = val_a.as_vector();

  CHECK(val_b.values_.size() == result_dimension);
  CHECK(val_b.values_[result_dimension - 1] == 0);

  std::string expected_value{typeid(Point<int, 4>).name()};
  CHECK(expected_value == typeid(val_b).name());
}

//│ ▼1 │ Related Operators
//└────┴───────────────────

template <typename RatT_l, typename RatT_r, std::size_t kDimension>
bool operator==(const Point<RatT_l, kDimension>& l_op,
    const Point<RatT_r, kDimension>& r_op)
{
  return l_op.values_ == r_op.values_;
}
TEST_CASE("Testing Point<> == Point<>")
{
  typedef Point3D<int> IPoint3D;

  IPoint3D arbitrary_a{{1, 5, 24}};
  IPoint3D arbitrary_b{{1, 5, 24}};

  // To see if it even compiles, I guess.
  CHECK(arbitrary_a == arbitrary_b);
}

template <typename RatT_l, typename RatT_r, std::size_t kDimension>
auto operator+(const Point<RatT_l, kDimension>& l_op,
    const Point<RatT_r, kDimension>& r_op)
{
  using std::declval;
  Point<decltype(declval<RatT_l>() + declval<RatT_r>()), kDimension> ret;

  for (std::size_t i = 0; i < kDimension; ++i) {
    ret.values_[i] = l_op.values_[i] + r_op.values_[i];
  }
  return ret;
}
TEST_CASE("Testing Point<> + Point<>")
{
  typedef Point<int, 3> IPoint3D;

  IPoint3D origin{{0, 0, 0}};

  IPoint3D a{origin};
  IPoint3D b{{1, 2, 3}};
  IPoint3D c{{10, 20, 30}};
  IPoint3D d{{4, 5, 6}};

  for (const auto& val : {a, b, c, d}) {
    CHECK(val == val + origin);
  }

  IPoint3D a_b{{1, 2, 3}};
  CHECK(a_b == a + b);

  IPoint3D b_b{{2, 4, 6}};
  CHECK(b_b == b + b);

  IPoint3D b_c{{11, 22, 33}};
  CHECK(b_c == b + c);

  IPoint3D b_d{{5, 7, 9}};
  CHECK(b_d == b + d);
}

template <typename RatT_l, typename RatT_r, std::size_t kDimension>
auto operator*(const Point<RatT_l, kDimension>& l_op, const RatT_r& r_op)
{
  using std::declval;
  Point<decltype(declval<RatT_l>() * r_op), kDimension> ret;

  for (std::size_t i = 0; i < kDimension; ++i) {
    ret.values_[i] = l_op.values_[i] * r_op;
  }
  return ret;
}
TEST_CASE("Testing Point<> * RatT")
{
  typedef Point3D<int> IPoint3D;

  IPoint3D a{{3, 5, 7}};
  static const int factor{2};
  IPoint3D expected{{6, 10, 14}};

  CHECK(expected == a * factor);
}

template <typename RatT_l, typename RatT_r, std::size_t kDimension>
auto operator*(RatT_l l_op, const Point<RatT_r, kDimension>& r_op)
{
  // commutative
  return r_op * l_op;
}
TEST_CASE("Testing RatT * Point<>")
{
  typedef Point3D<int> IPoint3D;

  static const int factor{2};
  IPoint3D a{{3, 5, 7}};
  IPoint3D expected{{6, 10, 14}};

  CHECK(expected == factor * a);
}

template <typename RatT_l, typename RatT_r, std::size_t kDimension>
auto dot(const Point<RatT_l, kDimension>& l_op,
    const Point<RatT_r, kDimension>& r_op)
{
  using std::declval;
  // clang-format off
  decltype(declval<RatT_l>() * declval<RatT_r>()
           +
           declval<RatT_l>() * declval<RatT_r>()) sum{0};
  // clang-format on
  for (std::size_t i = 0; i < kDimension; ++i) {
    sum += l_op.values_[i] * r_op.values_[i];
  }

  return sum;
}
TEST_CASE("Testing dot(Point<>, Point<>)")
{
  typedef Point<int, 2> IPoint2D;

  IPoint2D i{{1, 0}};
  IPoint2D j{{0, 1}};

  auto i_j = dot(i, j);
  CHECK(0 == i_j);

  auto orthogonal = dot(3 * i + 4 * j, -4 * i + 3 * j);
  CHECK(0 == orthogonal);

  auto something_different = dot(3 * i, 2 * i);
  CHECK(6 == something_different);
}

template <typename RatT_l, typename RatT_r>
auto cross(const Point<RatT_l, 3>& l_op, const Point<RatT_r, 3>& r_op)
{
  using std::declval;
  // clang-format off
  Point<decltype(declval<RatT_l>() * declval<RatT_r>()
                 -
                 declval<RatT_l>() * declval<RatT_r>()), 3> ret{};
  // clang-format on

  const auto& l = l_op.values_;
  const auto& r = r_op.values_;

  ret.values_[0] = l[1] * r[2] - l[2] * r[1];
  ret.values_[1] = l[2] * r[0] - l[0] * r[2];
  ret.values_[2] = l[0] * r[1] - l[1] * r[0];

  return ret;
}
TEST_CASE("Testing cross(Point<>, Point<>, bool right_handed = true)")
{
  typedef Point<int, 3> IPoint3D;

  static const IPoint3D i{{1, 0, 0}};
  static const IPoint3D j{{0, 1, 0}};
  static const IPoint3D k{{0, 0, 1}};

  CHECK(i == cross(j, k));
  CHECK(j == cross(k, i));
  CHECK(k == cross(i, j));

  // Non-commutative
  CHECK(-1 * i == cross(k, j));
  CHECK(-1 * j == cross(i, k));
  CHECK(-1 * k == cross(j, i));
}

//┌────┬───────────────────
//│ ▲1 │ Related Operators

} // namespace rational_geometry

#endif // _RATIONAL_GEOMETRY_POINT_HPP_INCLUDED_

// vim:set fdm=marker fmr=▼,▲ cms=\ //%s et ts=2 sw=2 sts=2:

