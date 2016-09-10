/// \file     Point.hpp
/// \author   Tim Holt
///
/// A Point class and its related functions. The class is templatized so that
/// any rational type may be used for the point coordinates.
///
/// \todo  Make as many functions constexpr as possible.
///
/// This code is under the MIT license, please see LICENSE.txt for more
/// information

#ifndef _RATIONAL_GEOMETRY_POINT_HPP_INCLUDED_
#define _RATIONAL_GEOMETRY_POINT_HPP_INCLUDED_

// Includes
//----------

#include <array>
#include <initializer_list>
#include <ostream>
#include <typeinfo>
#include <utility>

//----------
// Includes

namespace rational_geometry {

// Class Template Declaration
//----------------------------

/// A geometric point in n-space with rational number coordinates.
///
/// This class template should actually work with any type that has infinite
/// precision, or with any type in uses where the type is not used outside of
/// its full accuracy. One suggestion might be to throw an exception on a
/// inaccurate operation from your RatT type.
///
/// \todo  Get cbegin() & cend() to work
///
template <typename RatT, std::size_t kDimension = 3>
class Point
{
 protected:
  // CONSTRUCTORS
  Point(const Point<RatT, kDimension - 1>& smaller_point, RatT last);

 public:
  // INTERNAL STATE
  std::array<RatT, kDimension> values_;

  // CONSTRUCTORS
  Point();
  Point(const std::initializer_list<RatT>& values);
  Point(const std::array<RatT, kDimension>& values);

  // ACCESSORS
  Point<RatT, kDimension + 1> as_point() const;
  Point<RatT, kDimension + 1> as_vector() const;
  Point<RatT, kDimension - 1> as_simpler() const;

  RatT& operator[](size_t index);
  RatT& at(size_t index);

  typename std::array<RatT, kDimension>::iterator begin();
  typename std::array<RatT, kDimension>::iterator end();

  // FRIENDS
  friend Point<RatT, kDimension - 1>;
  friend Point<RatT, kDimension + 1>;
};

// Convenience typedefs
//----------------------

template <typename RatT>
using Point3D = Point<RatT, 3>;

template <typename RatT>
using Point2D = Point<RatT, 2>;

// Class Template Definitions
//----------------------------
//   Constructors
//  --------------
//     Public
//    --------

/// Creates a Point with kDimension dimensions, with all values at 0.
///
template <typename RatT, size_t kDimension>
Point<RatT, kDimension>::Point() : values_{}
{
}

/// Creates a Point with kDimension dimensions, fills with the initializel_list
///
template <typename RatT, size_t kDimension>
Point<RatT, kDimension>::Point(const std::initializer_list<RatT>& values)
    : values_{} // 0 initialize
{
  using namespace std;
  auto start = std::begin(values);
  auto stop  = start + min(values.size(), values_.size());
  copy(start, stop, std::begin(values_));
}

template <typename RatT, size_t kDimension>
Point<RatT, kDimension>::Point(const std::array<RatT, kDimension>& values)
    : values_{values}
{
}

//     Protected
//    -----------

/// \brief  Utility constructor for representing a point's equivalent in a
///         higher dimension.
///
template <typename RatT, size_t kDimension>
Point<RatT, kDimension>::Point(
    const Point<RatT, kDimension - 1>& smaller_point, RatT last)
{
  std::copy(std::begin(smaller_point.values_), std::end(smaller_point.values_),
      std::begin(values_));
  *std::rbegin(values_) = last;
}

//   Accessors
//  -----------

/// \brief  Get a version of the point as a point that can be translated by a
///         translation matrix.
///
/// For when the vector (Point) actually conceptually represents a location in
/// space, so that a matrix of 1 higher dimension can be used to translate its
/// location, and not just scale/rotate/skew it.
///
template <typename RatT, size_t kDimension>
Point<RatT, kDimension + 1> Point<RatT, kDimension>::as_point() const
{
  return {*this, 1};
}

/// \brief  Get a version of the point as a point that can<i>not</i> be
///         translated by a translation matrix.
///
/// For when the vector (Point) represents a vector that <i>isn't</i> pretending
/// to be a location, but actually just represents a direction and magnitude.
///
template <typename RatT, size_t kDimension>
Point<RatT, kDimension + 1> Point<RatT, kDimension>::as_vector() const
{
  return {*this, 0};
}

/// Converts a higher-dimensional point back to its real value.
///
/// This is meant to convert back to the real type in use from a value
/// originally obtained through the as_point() or as_vector() methods.
///
/// \return  The Point without its last element.
///
template <typename RatT, size_t kDimension>
Point<RatT, kDimension - 1> Point<RatT, kDimension>::as_simpler() const
{
  Point<RatT, kDimension - 1> ret;

  using namespace std;
  copy(std::cbegin(values_), std::cend(values_) - 1, std::begin(ret.values_));

  return ret;
}


template <typename RatT, size_t kDimension>
RatT& Point<RatT, kDimension>::operator[](size_t index)
{
  return values_[index];
}

template <typename RatT, size_t kDimension>
RatT& Point<RatT, kDimension>::at(size_t index)
{
  return values_.at(index);
}


template <typename RatT, size_t kDimension>
typename std::array<RatT, kDimension>::iterator Point<RatT, kDimension>::begin()
{
  return values_.begin();
}

template <typename RatT, size_t kDimension>
typename std::array<RatT, kDimension>::iterator Point<RatT, kDimension>::end()
{
  return values_.end();
}

// Related Operators
//-------------------
//   Comparison Operators
//  ----------------------

/// Test equality of two points.
///
/// They must actually be exactly equal in all fields. This is one of the
/// advantages of using rational numbers for our math: with your typical
/// floating-point geometric libraries, you usually have to test
/// <i>approximate</i> equality due to rounding errors.
///
template <typename RatT_l, typename RatT_r, std::size_t kDimension>
bool operator==(const Point<RatT_l, kDimension>& l_op,
    const Point<RatT_r, kDimension>& r_op)
{
  // note: std::array comparison with its built-in operator= requires the types
  // contained to be the same. It's fine with me if they're different if they
  // really do compare equal. Hence this over-complex reimplimentation.
  using namespace std;
  return equal(cbegin(l_op.values_), cend(l_op.values_), cbegin(r_op.values_));
}

/// Test for inequality
///
template <typename RatT_l, typename RatT_r, std::size_t kDimension>
bool operator!=(const Point<RatT_l, kDimension>& l_op,
    const Point<RatT_r, kDimension>& r_op)
{
  return !(l_op == r_op);
}

/// Test whether a point is less than another, lexicographically.
///
/// This is not just useful for the stl, it is also a reliable way of
/// determining what order a group of colinear points is in. That specifically.
/// Don't use it for any other kind of point comparison.
///
template <typename RatT_l, typename RatT_r, std::size_t kDimension>
bool operator<(const Point<RatT_l, kDimension>& l_op,
    const Point<RatT_r, kDimension>& r_op)
{
  // see note in operator==. Find it by searching "over-complex".
  using namespace std;
  return lexicographical_compare(cbegin(l_op.values_), cend(l_op.values_),
      cbegin(r_op.values_), cend(r_op.values_));
}

template <typename RatT_l, typename RatT_r, std::size_t kDimension>
bool operator<=(const Point<RatT_l, kDimension>& l_op,
    const Point<RatT_r, kDimension>& r_op)
{
  return !(r_op < l_op);
}

template <typename RatT_l, typename RatT_r, std::size_t kDimension>
bool operator>(const Point<RatT_l, kDimension>& l_op,
    const Point<RatT_r, kDimension>& r_op)
{
  return r_op < l_op;
}

template <typename RatT_l, typename RatT_r, std::size_t kDimension>
bool operator>=(const Point<RatT_l, kDimension>& l_op,
    const Point<RatT_r, kDimension>& r_op)
{
  return !(l_op < r_op);
}

//   Other Operators
//  -----------------

/// Add two vectors
///
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

/// Scale a vector by a scalar.
///
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

/// Scale a vector by a scalar.
///
template <typename RatT_l, typename RatT_r, std::size_t kDimension>
auto operator*(RatT_l l_op, const Point<RatT_r, kDimension>& r_op)
{
  // commutative
  return r_op * l_op;
}

// Related Functions
//-------------------

/// Find the dot product (<i>scalar</i> product) between two vectors.
///
/// \return  a value equivalent to the magnitude of the two vectors multiplied
///          together, multiplied by the cosine of the angle between them.
///
/// \todo  Make it usable for any iterable container type of known size.
///        That is, give it a decltype-defaulted template parameter for return
///        type, and templatize away Point<>.
///
/// \sa  https://en.wikipedia.org/wiki/Dot_product
///
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

/// Find the cross product (<i>vector</i> product) between two vectors.
///
/// \return  a vector which (when not magnitude 0) is perpendicular to both
///          inputs and equal to their magnitudes multiplied together,
///          multiplied by the sine of the angle between them. Of the two
///          possible directions (when not magnitude 0), the actual value is
///          given by the right-hand rule.
///
/// \todo  Make it usable for any iterable container type of known size.
///        That is, give it a decltype-defaulted template parameter for return
///        type, and templatize away Point<>.
///
/// \sa  https://en.wikipedia.org/wiki/Cross_product
///
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

template <typename RatT, size_t kDimension>
std::ostream& operator<<(
    std::ostream& the_stream, const Point<RatT, kDimension> the_point)
{
  the_stream << typeid(the_point).name();
  the_stream << ":(";
  for (const auto& coordinate : the_point.values_) {
    the_stream << coordinate << ", ";
  }
  the_stream << ")";
  return the_stream;
}

//-------------------
// Related Functions

} // namespace rational_geometry

#endif // _RATIONAL_GEOMETRY_POINT_HPP_INCLUDED_

// vim:set et ts=2 sw=2 sts=2:

