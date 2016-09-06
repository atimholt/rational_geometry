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
  Matrix(const std::initializer_list<std::array<RatT, kHeight>>& values);

  // GETTERS
  /// \todo  implement
  Point<RatT, kHeight> get_row(size_t which) const;
  /// \todo  implement
  Point<RatT, kWidth> get_column(size_t which) const;

  // SETTERS
  /// \todo  implement
  Matrix& set_row(const Point<RatT, kHeight>& value);
  /// \todo  implement
  Matrix& set_column(const Point<RatT, kWidth>& value);
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

//----------------------------
// Class Template Definitions

} // namespace rational_geometry

#endif // _RATIONAL_GEOMETRY_MATRIX_HPP_INCLUDED_

// vim:set et ts=2 sw=2 sts=2:

