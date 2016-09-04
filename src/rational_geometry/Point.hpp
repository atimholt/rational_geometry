// Point.hpp

#ifndef _RATIONAL_GEOMETRY_POINT_HPP_INCLUDED_
#define _RATIONAL_GEOMETRY_POINT_HPP_INCLUDED_

//│ ▼1 │ Includes
//└────┴──────────

#include <array>
#include <cstdarg>
#include <cstddef>
#include <initializer_list>

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
  Point(const std::initializer_list<RatT>& values);
  Point(const std::array<RatT, kDimension>& values);
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
Point<RatT, kDimension>::Point(const std::initializer_list<RatT>& values)
    : values_{} // 0 initialize
{
  using namespace std;
  auto start = begin(values);
  auto stop  = start + min(values.size(), values_.size());
  copy(start, stop, begin(values_));
}

template <typename RatT, size_t kDimension>
Point<RatT, kDimension>::Point(const std::array<RatT, kDimension>& values)
    : values_{values}
{
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

//  │ ▼2 │ Accessors
//  └────┴───────────

template <typename RatT, size_t kDimension>
Point<RatT, kDimension + 1> Point<RatT, kDimension>::as_point()
{
  return {*this, 1};
}

template <typename RatT, size_t kDimension>
Point<RatT, kDimension + 1> Point<RatT, kDimension>::as_vector()
{
  return {*this, 0};
}

//│ ▼1 │ Related Operators
//└────┴───────────────────

template <typename RatT_l, typename RatT_r, std::size_t kDimension>
bool operator==(const Point<RatT_l, kDimension>& l_op,
    const Point<RatT_r, kDimension>& r_op)
{
  return l_op.values_ == r_op.values_;
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

template <typename RatT_l, typename RatT_r, std::size_t kDimension>
auto operator*(RatT_l l_op, const Point<RatT_r, kDimension>& r_op)
{
  // commutative
  return r_op * l_op;
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

//┌────┬───────────────────
//│ ▲1 │ Related Operators

} // namespace rational_geometry

#endif // _RATIONAL_GEOMETRY_POINT_HPP_INCLUDED_

// vim:set fdm=marker fmr=▼,▲ cms=\ //%s et ts=2 sw=2 sts=2:

