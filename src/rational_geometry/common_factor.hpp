/// \file     common_factor.hpp
/// \author   Tim Holt
///
/// Tools for dealing with the factors of integers.
///
/// As much as I hate re-implementing something already so frequently
/// implemented, this is meant to be a header-only library with no external
/// dependencies. Beyond that, even boost's common_factor.hpp does not make use
/// of modern c++'s constexpr keyword, separating its gcd & lcm functions into
/// separate runtime and compile time headers.
///
/// This code is under the MIT license, please see LICENSE.txt for more
/// information

#ifndef _RATIONAL_GEOMETRY_COMMON_FACTOR_HPP_INCLUDED_
#define _RATIONAL_GEOMETRY_COMMON_FACTOR_HPP_INCLUDED_

// Includes
//----------

#include <type_traits>

//----------
// Includes

namespace rational_geometry {

// Functions
//-----------

/// A constexpr abs().
///
/// I'm sure the standards committee has a really good reason for its abs() not
/// to be constexpr. \s
///
template <typename T>
constexpr auto abs(T value) ->
    typename std::enable_if<std::is_integral<T>::value, T>::type
{
  return value >= 0 ? value : -value;
}

/// \brief  Find the greatest common divisor between two integral numbers using
///         Euclid's algorithm.
///
template <typename T>
constexpr auto gcd(T a, T b) ->
    typename std::enable_if<std::is_integral<T>::value, T>::type
{
  return rational_geometry::abs(0 == b ? a : gcd<T>(b, a % b));
}

//-----------
// Functions

} // namespace rational_geometry

#endif // _RATIONAL_GEOMETRY_COMMON_FACTOR_HPP_INCLUDED_

// vim:set et ts=2 sw=2 sts=2:

