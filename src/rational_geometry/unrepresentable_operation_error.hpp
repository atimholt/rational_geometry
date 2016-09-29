/// \file     unrepresentable_operation_error.hpp
/// \author   Tim Holt
///
/// \brief  An error indicating that a Rational number has been used outside of
///         its domain of full accuracy.
///
/// This code is under the MIT license, please see LICENSE.txt for more
/// information

#ifndef _RATIONAL_GEOMETRY_POINT_HPP_INCLUDED_
#define _RATIONAL_GEOMETRY_POINT_HPP_INCLUDED_

// Includes
//----------

#include "common_factor.hpp"

#include <stdexcept>

//----------
// Includes

namespace rational_geometry {

// Class Template Declaration
//----------------------------

/// \brief  An error indicating that a Rational number has been used outside of
///         its domain of full accuracy.
///
/// This exception may be deactivated by defining
/// RATIONAL_GEOMETRY_DONT_THROW_ON_INEXACT_OPERATION before including the
/// "rational_geometry/Rational.hpp" header, where it is used.
///
template <typename IntT>
class unrepresentable_operation_error : public std::domain_error
{
  // STATIC ASSERTIONS
  static_assert(std::is_integral<IntT>::value, "IntT must be integer type");

  // INTERNAL STATE
  IntT minimum_fix_factor_;

 public:
  // CONSTRUCTORS
  unrepresentable_operation_error(const std::string& what_arg,
      IntT operation_numerator,
      IntT operation_divisor);
  unrepresentable_operation_error(
      const char* what_arg, IntT operation_numerator, IntT operation_divisor);

  // ACCESSORS
  IntT get_minimum_fix_factor() const;

  // OTHER METHODS
  IntT& accumulate_fix_factor(IntT& running_accumulation) const;
};

// Class Template Definitions
//----------------------------
//   Constructors
//  --------------

template <typename IntT>
unrepresentable_operation_error<typename IntT>::unrepresentable_operation_error(
    const std::string& what_arg,
    IntT operation_numerator,
    IntT operation_divisor)
    : std::domain_error(what_arg)
    , minimum_fix_factor_{
          operation_divisor / gcd(operation_numerator, operation_divisor)}
{
}

template <typename IntT>
unrepresentable_operation_error<typename IntT>::unrepresentable_operation_error(
    const char* what_arg, IntT operation_numerator, IntT operation_divisor)
    : std::domain_error(what_arg)
    , minimum_fix_factor_{
          operation_divisor / gcd(operation_numerator, operation_divisor)}
{
}

//   Accessors
//  -----------

template <typename IntT>
IntT unrepresentable_operation_error<typename IntT>::get_minimum_fix_factor()
    const
{
  return minimum_fix_factor_;
}

//   Other Methods
//  ---------------

template <typename IntT>
IntT& unrepresentable_operation_error<typename IntT>::accumulate_fix_factor(
    IntT& running_accumulation) const
{
  if (running_accumulation <= 0) {
    running_accumulation = 1;
  }
  running_accumulation = lcm(running_accumulation, get_minimum_fix_factor());
  return running_accumulation;
}

//---------
//

} // namespace rational_geometry

#endif // _RATIONAL_GEOMETRY_POINT_HPP_INCLUDED_

// vim:set et ts=2 sw=2 sts=2:

