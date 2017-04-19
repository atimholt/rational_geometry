/// \file     Direction.hpp
/// \author   Tim Holt
///
/// A rationally-coordinated direction class.
///
/// This code is under the MIT license, please see LICENSE.txt for more
/// information

#ifndef _RATIONAL_GEOMETRY_DIRECTION_HPP_INCLUDED_
#define _RATIONAL_GEOMETRY_DIRECTION_HPP_INCLUDED_

// Includes
//----------

#include "common_factor.hpp"

#include <array>
#include <cassert>
#include <numeric>
#include <type_traits>

//----------
// Includes

namespace rational_geometry {

// Class Template Declaration
//----------------------------

/// A direction useful for geometric systems using only rational coordinates.
///
/// This fulfills much the same usage as unit-length vectors in other geometry
/// libraries, except that unit-length vectors are the exception, not the rule,
/// in a geometry consisting of only rational coordinates. As such, absolutely
/// no guarantees are made about the 'length' of the internal representation of
/// this class.
///
template <typename SignedIntT, std::size_t kDimension = 3>
class Direction : public std::array<SignedIntT, kDimension>
{
 protected:
  // STATIC ASSERTIONS
  static_assert(
      std::is_integral<SignedIntT>::value, "SignedIntT must be integer type");

  static_assert(
      std::is_signed<SignedIntT>::value, "SignedIntT must be a signed type");

  // HELPER FUNCTIONS
  void normalize();

 public:
  // CONSTRUCTORS
  Direction();
  Direction(const std::initializer_list<SignedIntT>& values);
  explicit Direction(const std::array<SignedIntT, kDimension>& values);

  Direction(
      const std::initializer_list<std::pair<SignedIntT, SignedIntT>>& values);

  // ACCESSORS
  size_t first_present_dimension() const;
};

// Class Template Definitions
//----------------------------
//   Constructors
//  --------------

/// Creates a Point with kDimension dimensions, with all values at 0.
///
template <typename SignedIntT, size_t kDimension>
Direction<SignedIntT, kDimension>::Direction()
    : std::array<SignedIntT, kDimension>()
{
}

/// Creates a Direction with kDimension dimensions, fills with the
/// initializer_list
///
template <typename SignedIntT, size_t kDimension>
Direction<SignedIntT, kDimension>::Direction(
    const std::initializer_list<SignedIntT>& values)
    : Direction<SignedIntT, kDimension>()
{
  std::copy(std::cbegin(values),
      std::cbegin(values) + std::min(values.size(), kDimension), begin());
  normalize();
}

template <typename SignedIntT, size_t kDimension>
Direction<SignedIntT, kDimension>::Direction(
    const std::array<SignedIntT, kDimension>& values)
    : std::array<SignedIntT, kDimension>(values)
{
  normalize();
}

template <typename SignedIntT, size_t kDimension>
Direction<SignedIntT, kDimension>::Direction(
    const std::initializer_list<std::pair<SignedIntT, SignedIntT>>& values)
{
  using std::get;

  auto the_lcm = std::accumulate(std::cbegin(values), std::cend(values), 1,
      [&](SignedIntT running_total,
          std::pair<SignedIntT, SignedIntT> rational_coord) {
        return lcm(running_total, get<1>(rational_coord));
      });

  auto i = 0;
  for (auto const& coord : values) {
    (*this)[i] = get<0>(coord) * (the_lcm / get<1>(coord));
    ++i;
  }

  normalize();
}

//   Helper Functions
//  ------------------

/// Make the internal representation consistent
///
/// \todo  consider moving gcd of n integers to common_factor.hpp
///
template <typename SignedIntT, std::size_t kDimension>
void Direction<SignedIntT, kDimension>::normalize()
{
  if (kDimension == 0) return;
  if (kDimension == 1) {
    if ((*this)[0] > 0)
      (*this)[0] = 1;
    else if ((*this)[0] < 0)
      (*this)[0] = -1;
    // left alone if == 0 !
    return;
  }

  // general case
  SignedIntT the_gcd =
      std::accumulate(std::begin(*this), std::end(*this), (*this)[0],
          [&](SignedIntT l_op, SignedIntT r_op) { return gcd(l_op, r_op); });
  if (the_gcd == 0) return;
  for (auto& val : *this) {
    val /= the_gcd;
  }
}

//   Accessors
//  -----------

/// Finds the index of the first non-zero component of a direction.
///
/// \note  returns 1 past the end for a null Direction.
///
template <typename SignedIntT, std::size_t kDimension>
size_t Direction<SignedIntT, kDimension>::first_present_dimension() const
{
  return std::find_if(std::begin(*this), std::end(*this),
             [&](SignedIntT component) { return component != 0; })
         - std::begin(*this);
}

//----------------------------
// Class Template Definitions

} // namespace rational_geometry

#endif // _RATIONAL_GEOMETRY_DIRECTION_HPP_INCLUDED_

// vim:set et ts=2 sw=2 sts=2:

