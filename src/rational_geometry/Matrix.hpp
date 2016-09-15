/// \file     Matrix.hpp
/// \author   Tim Holt
///
/// A Matrix class and its related functions.
///
/// The class is templatized so that any rational type may be used for the
/// matrix elements.
///
/// \todo  Make as many functions constexpr as possible.
///
/// \todo  Implement relevant functions.
///
/// \todo  change code style to put return type on its own line in definitions.
///
/// This code is under the MIT license, please see LICENSE.txt for more
/// information

#ifndef _RATIONAL_GEOMETRY_MATRIX_HPP_INCLUDED_
#define _RATIONAL_GEOMETRY_MATRIX_HPP_INCLUDED_

// Includes
//----------

#include "Point.hpp"

#include <array>
#include <initializer_list>
#include <ostream>
#include <typeinfo>

//----------
// Includes

namespace rational_geometry {

// Class Template Declaration
//----------------------------

/// \brief  An arbitrarily-sized matrix with rational members, useful for
///         affine transforms.
///
/// Considering the members are rational, it's not possible to perform
/// arbitrary rotation. The envisioned purpose is mainly rotations that are
/// multiples of Tau/4 radians, reflections, scalings, and translations.
///
/// However, any kind of rational transform may be performed if the library
/// user wishes.
///
/// \note  tau = 2*pi. That is, tau radians = 1 full circle.
///
/// \todo  remove methods returning or taking Point<> once dot() and cross()
///        are more fully templatized.
///
template <typename RatT, size_t kHeight = 4, size_t kWidth = kHeight>
class Matrix
{
 public:
  // INTERNAL STATE

  /// Conceptually, this is an array of <i>rows</i>.
  std::array<std::array<RatT, kWidth>, kHeight> values_;

  // CONSTRUCTORS
  Matrix();
  Matrix(std::initializer_list<std::initializer_list<RatT>> values);

  // GETTERS
  Point<RatT, kWidth> get_row(size_t which) const;
  Point<RatT, kHeight> get_column(size_t which) const;

  // SETTERS
  Matrix& set_row(size_t which, const Point<RatT, kWidth>& values);
  Matrix& set_column(size_t which, const Point<RatT, kHeight>& values);
};

// Class Template Definitions
//----------------------------
//   Constructors
//  --------------

/// Initialize a matrix to an identity matrix.
///
template <typename RatT, size_t kHeight, size_t kWidth>
Matrix<RatT, kHeight, kWidth>::Matrix()
{
  for (int row = 0; row < kHeight; ++row) {
    for (int column = 0; column < kWidth; ++column) {
      values_[row][column] = (row == column) ? 1 : 0;
    }
  }
}

template <typename RatT, size_t kHeight, size_t kWidth>
Matrix<RatT, kHeight, kWidth>::Matrix(
    std::initializer_list<std::initializer_list<RatT>> values)
{
  using namespace std;

  auto in_row = cbegin(values);
  for (auto& row : values_) {
    copy(cbegin(*in_row), cend(*in_row), begin(row));
    ++in_row;
  }
}

//   Getters
//  ---------

template <typename RatT, size_t kHeight, size_t kWidth>
Point<RatT, kWidth> Matrix<RatT, kHeight, kWidth>::get_row(size_t which) const
{
  Point<RatT, kWidth> ret;

  using namespace std;
  copy(cbegin(values_[which]), cend(values_[which]), begin(ret));

  return ret;
}

template <typename RatT, size_t kHeight, size_t kWidth>
Point<RatT, kHeight> Matrix<RatT, kHeight, kWidth>::get_column(
    size_t which) const
{
  Point<RatT, kHeight> ret;
  for (int i = 0; i < kHeight; ++i) {
    ret[i] = values_[i][which];
  }
  return ret;
}

//   Setters
//  ---------

/// \todo  Implement bounds checking.
///
template <typename RatT, size_t kHeight, size_t kWidth>
Matrix<RatT, kHeight, kWidth>& Matrix<RatT, kHeight, kWidth>::set_row(
    size_t which, const Point<RatT, kWidth>& values)
{
  using namespace std;
  copy(cbegin(values), cend(values), begin(values_[which]));

  return *this;
}

/// \todo  Implement bounds checking.
///
template <typename RatT, size_t kHeight, size_t kWidth>
Matrix<RatT, kHeight, kWidth>& Matrix<RatT, kHeight, kWidth>::set_column(
    size_t which, const Point<RatT, kHeight>& values)
{
  for (int i = 0; i < kHeight; ++i) {
    values_[i][which] = values[i];
  }

  return *this;
}

// Related Operators
//-------------------
//   Comparison Operators
//  ----------------------

template <typename RatT_l, typename RatT_r, size_t kHeight, size_t kWidth>
bool operator==(const Matrix<RatT_l, kHeight, kWidth>& l_op,
    const Matrix<RatT_r, kHeight, kWidth>& r_op)
{
  using namespace std;

  auto l_row = cbegin(l_op.values_);
  auto r_row = cbegin(r_op.values_);

  for (; l_row != cend(l_op.values_); ++l_row, ++r_row) {
    if (!equal(cbegin(*l_row), cend(*l_row), cbegin(*r_row))) {
      return false;
    }
  }
  return true;
}

template <typename RatT_l, typename RatT_r, size_t kHeight, size_t kWidth>
bool operator!=(const Matrix<RatT_l, kHeight, kWidth>& l_op,
    const Matrix<RatT_r, kHeight, kWidth>& r_op)
{
  return !(l_op == r_op);
}

/// Determine if the left Matrix is lexicographically lesser.
///
/// This serves no practical purpose other than use in the stl.
///
template <typename RatT_l, typename RatT_r, size_t kHeight, size_t kWidth>
bool operator<(const Matrix<RatT_l, kHeight, kWidth>& l_op,
    const Matrix<RatT_r, kHeight, kWidth>& r_op)
{
  using namespace std;

  auto l_row = cbegin(l_op.values_);
  auto r_row = cbegin(r_op.values_);

  for (; l_row != cend(l_op.values_); ++l_row, ++r_row) {
    auto l_entry = cbegin(*l_row);
    auto r_entry = cbegin(*r_row);

    for (; l_entry != cend(*l_row); ++l_entry, ++r_entry) {
      if (*l_entry < *r_entry) {
        return true;
      }
      else if (*r_entry < *l_entry) {
        return false;
      }
      // Continue to test, this entry is equal.
    }
  }
  return false;
}

//   Other Operators
//  -----------------

/// Multiply two Matrices
///
/// \todo  Eventually use references to members instead of creating Points. This
///        is a harder problem than at first blush.
///
template <typename RatT_l,
    typename RatT_r,
    size_t kl_Height,
    size_t kCommon_Dimension,
    size_t kr_Width>
auto operator*(const Matrix<RatT_l, kl_Height, kCommon_Dimension>& l_op,
    const Matrix<RatT_r, kCommon_Dimension, kr_Width>& r_op)
{
  using std::declval;
  // clang-format off
  typedef decltype(
      dot(
        declval<Point<RatT_l, kCommon_Dimension>>(),
        declval<Point<RatT_r, kCommon_Dimension>>()
      )) RetBaseT;
  // clang-format on
  Matrix<RetBaseT, kl_Height, kr_Width> ret;

  // copying the values is slower than ideal, but we're just trying to get a
  // working algorithm to begin with.
  for (int i = 0; i < kl_Height; ++i) {
    for (int j = 0; j < kr_Width; ++j) {
      ret.values_[i][j] = dot(l_op.get_row(i), r_op.get_column(j));
    }
  }

  return ret;
}

/// Multiply a Point by a Matrix.
///
template <typename RatT_l, typename RatT_r, size_t kHeight, size_t kWidth>
auto operator*(const Matrix<RatT_l, kHeight, kWidth>& l_op,
    const Point<RatT_r, kWidth>& r_op)
{
  // Width of l_op is height of r_op_matrix.
  Matrix<RatT_r, kWidth, 1> r_op_matrix;
  r_op_matrix.set_column(0, r_op);

  auto ret = l_op * r_op_matrix;
  return ret.get_column(0);
}

// Related Functions
//-------------------

template <typename RatT, size_t kHeight, size_t kWidth>
std::ostream& operator<<(
    std::ostream& the_stream, const Matrix<RatT, kHeight, kWidth>& the_matrix)
{
  the_stream << typeid(the_matrix).name();
  the_stream << ":\n[\n";
  for (const auto& row : the_matrix.values_) {
    for (const auto& entry : row) {
      the_stream << " " << entry << ", ";
    }
    the_stream << "\n";
  }
  the_stream << "]";
  return the_stream;
}

template <typename RatT, size_t kDimensions>
auto make_translation(Point<RatT, kDimensions> new_origin)
{
  Matrix<RatT, kDimensions + 1, kDimensions + 1> ret{};

  // 1st argument not off by one.
  return ret.set_column(kDimensions, new_origin.as_point());
}

// \todo  Implement make_rotation(Point, Point, Point);
// \todo  Implement make_scale(RatT, RatT, RatT);
// \todo  Implement make_scale(RatT);

//-------------------
// Related Functions

} // namespace rational_geometry

#endif // _RATIONAL_GEOMETRY_MATRIX_HPP_INCLUDED_

// vim:set et ts=2 sw=2 sts=2:

