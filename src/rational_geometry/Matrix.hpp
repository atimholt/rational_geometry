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

//----------------------------
// Class Template Definitions

} // namespace rational_geometry

#endif // _RATIONAL_GEOMETRY_MATRIX_HPP_INCLUDED_

// vim:set et ts=2 sw=2 sts=2:

