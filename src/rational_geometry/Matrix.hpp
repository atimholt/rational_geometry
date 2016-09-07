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
/// \todo  Implement operators and relevant functions.
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
template <typename RatT, size_t kWidth = 4, size_t kHeight = kWidth>
class Matrix
{
 public:
  // INTERNAL STATE

  /// Conceptually, this is an array of <i>columns</i>.
  std::array<std::array<RatT, kHeight>, kWidth> values_;

  // CONSTRUCTORS
  Matrix();
  Matrix(std::initializer_list<std::initializer_list<RatT>> values);

  // GETTERS
  Point<RatT, kHeight> get_row(size_t which) const;
  Point<RatT, kWidth> get_column(size_t which) const;

  // SETTERS
  Matrix& set_row(size_t which, const Point<RatT, kHeight>& values);
  Matrix& set_column(size_t which, const Point<RatT, kWidth>& values);
};

// Class Template Definitions
//----------------------------
//   Constructors
//  --------------

/// Initialize a matrix to an identity matrix.
///
template <typename RatT, size_t kWidth, size_t kHeight>
Matrix<RatT, kWidth, kHeight>::Matrix()
{
  for (int column = 0; column < kWidth; ++column) {
    for (int row = 0; row < kHeight; ++row) {
      values_[column][row] = (column == row) ? 1 : 0;
    }
  }
}

template <typename RatT, size_t kWidth, size_t kHeight>
Matrix<RatT, kWidth, kHeight>::Matrix(
    std::initializer_list<std::initializer_list<RatT>> values)
{
  auto row_count = 0;
  // Can't use std::copy, the values are inverted from each-other.
  for (const auto& row : values) {
    auto column_count = 0;
    for (const auto& entry : row) {
      values_[column_count][row_count] = entry;
      ++column_count;
    }
    ++row_count;
  }
}

//   Getters
//  ---------

template <typename RatT, size_t kWidth, size_t kHeight>
Point<RatT, kHeight> Matrix<RatT, kWidth, kHeight>::get_row(size_t which) const
{
  Point<RatT, kHeight> ret;
  for (int i = 0; i < kHeight; ++i) {
    ret.values_[i] = values_[i][which];
  }
  return ret;
}

template <typename RatT, size_t kWidth, size_t kHeight>
Point<RatT, kWidth> Matrix<RatT, kWidth, kHeight>::get_column(
    size_t which) const
{
  Point<RatT, kWidth> ret;

  using namespace std;
  copy(cbegin(values_[which]), cend(values_[which]), begin(ret.values_));

  return ret;
}

//   Setters
//  ---------

/// \todo  Implement bounds checking.
///
template <typename RatT, size_t kWidth, size_t kHeight>
Matrix<RatT, kWidth, kHeight>& Matrix<RatT, kWidth, kHeight>::set_row(
    size_t which, const Point<RatT, kHeight>& values)
{
  for (int i = 0; i < kHeight; ++i) {
    values_[i][which] = values.values_[i];
  }

  return *this;
}

/// \todo  Implement bounds checking.
///
template <typename RatT, size_t kWidth, size_t kHeight>
Matrix<RatT, kWidth, kHeight>& Matrix<RatT, kWidth, kHeight>::set_column(
    size_t which, const Point<RatT, kWidth>& values)
{
  using namespace std;
  copy(cbegin(values.values_), cend(values.values_), begin(values_[which]));

  return *this;
}

// Related Operators
//-------------------

template <typename RatT, size_t kWidth, size_t kHeight>
bool operator==(const Matrix<RatT, kWidth, kHeight>& l_op,
    const Matrix<RatT, kWidth, kHeight>& r_op)
{
  using namespace std;

  auto l_column = cbegin(l_op.values_);
  auto r_column = cbegin(r_op.values_);

  for (; l_column != cend(l_op.values_); ++l_column, ++r_column) {
    if (!equal(cbegin(*l_column), cend(*l_column), cbegin(*r_column))) {
      return false;
    }
  }
  return true;
}

template <typename RatT, size_t kWidth, size_t kHeight>
bool operator!=(const Matrix<RatT, kWidth, kHeight>& l_op,
    const Matrix<RatT, kWidth, kHeight>& r_op)
{
  return !(l_op == r_op);
}

/// Determine if the left Matrix is lexicographically lesser.
///
/// This serves no practical purpose other than use in the stl.
///
template <typename RatT, size_t kWidth, size_t kHeight>
bool operator<(const Matrix<RatT, kWidth, kHeight>& l_op,
    const Matrix<RatT, kWidth, kHeight>& r_op)
{
  using namespace std;

  auto l_column = cbegin(l_op.values_);
  auto r_column = cbegin(r_op.values_);

  for (; l_column != cend(l_op.values_); ++l_column, ++r_column) {
    auto l_entry = cbegin(*l_column);
    auto r_entry = cbegin(*r_column);

    for (; l_entry != cend(*l_column); ++l_entry, ++r_entry) {
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

// Related Functions
//-------------------

template <typename RatT, size_t kWidth, size_t kHeight>
std::ostream& operator<<(
    std::ostream& the_stream, const Matrix<RatT, kWidth, kHeight>& the_matrix)
{
  the_stream << typeid(the_matrix).name();
  the_stream << ":\n[\n";
  for (const auto& column : the_matrix.values_) {
    for (const auto& entry : column) {
      the_stream << " " << entry << ", ";
    }
    the_stream << "\n";
  }
  the_stream << "]";
  return the_stream;
}

//-------------------
// Related Functions

} // namespace rational_geometry

#endif // _RATIONAL_GEOMETRY_MATRIX_HPP_INCLUDED_

// vim:set et ts=2 sw=2 sts=2:

