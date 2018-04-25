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

#include "Operations.hpp"
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
class Direction
{
 protected:
  // STATIC ASSERTIONS
  static_assert(
      std::is_integral<SignedIntT>::value, "SignedIntT must be integer type");

  static_assert(
      std::is_signed<SignedIntT>::value, "SignedIntT must be a signed type");

  // INTERNAL STATE
  std::array<SignedIntT, kDimension> dimension_proportions_;

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
  const std::array<SignedIntT, kDimension>& get() const;

  SignedIntT get(size_t index) const;
  SignedIntT get_dimensionality() const;

  size_t first_present_dimension() const;

  // OPERATORS
  bool operator==(Direction<SignedIntT, kDimension> r_op) const;
  bool operator<(Direction<SignedIntT, kDimension> r_op) const;
};

// Class Template Definitions
//----------------------------
//   Constructors
//  --------------

/// Creates a Point with kDimension dimensions, with all values at 0.
///
template <typename SignedIntT, size_t kDimension>
Direction<SignedIntT, kDimension>::Direction() : dimension_proportions_()
{
}

/// \brief  Creates a Direction with kDimension dimensions, fills with the
///         initializer_list
///
template <typename SignedIntT, size_t kDimension>
Direction<SignedIntT, kDimension>::Direction(
    const std::initializer_list<SignedIntT>& values)
    : Direction<SignedIntT, kDimension>()
{
  std::copy(std::cbegin(values),
      std::cbegin(values) + std::min(values.size(), kDimension),
      begin(dimension_proportions_));
  normalize();
}

template <typename SignedIntT, size_t kDimension>
Direction<SignedIntT, kDimension>::Direction(
    const std::array<SignedIntT, kDimension>& values)
    : dimension_proportions_(values)
{
  normalize();
}

/// Creates a Direction from an ordered collection of rational numbers.
///
/// Each dimension's portion will be proportional to the respective rational
/// number input.
///
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
    dimension_proportions_[i] = get<0>(coord) * (the_lcm / get<1>(coord));
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
    if (dimension_proportions_[0] > 0)
      dimension_proportions_[0] = 1;
    else if (dimension_proportions_[0] < 0)
      dimension_proportions_[0] = -1;
    // left alone if == 0 !
    return;
  }

  // general case
  SignedIntT the_gcd = std::accumulate(std::begin(dimension_proportions_),
      std::end(dimension_proportions_), (dimension_proportions_)[0],
      [&](SignedIntT l_op, SignedIntT r_op) { return gcd(l_op, r_op); });
  if (the_gcd == 0) return;
  for (auto& val : dimension_proportions_) {
    val /= the_gcd;
  }
}

//   Accessors
//  -----------

/// Get a const reference to the array of data of the Direction.
///
/// As usual, always be wary of const references changing on you!
///
template <typename SignedIntT, std::size_t kDimension>
const std::array<SignedIntT, kDimension>&
Direction<SignedIntT, kDimension>::get() const
{
  return dimension_proportions_;
}


/// Get the number representing the specified dimension.
///
/// \todo  Consider range checking.
///
template <typename SignedIntT, std::size_t kDimension>
SignedIntT Direction<SignedIntT, kDimension>::get(size_t index) const
{
  return dimension_proportions_[index];
}

/// Get the dimensionality of the direction.
///
template <typename SignedIntT, std::size_t kDimension>
SignedIntT Direction<SignedIntT, kDimension>::get_dimensionality() const
{
  return kDimension;
}


/// Finds the index of the first non-zero component of a direction.
///
/// \note  returns 1 past the end for a null Direction.
///
template <typename SignedIntT, std::size_t kDimension>
size_t Direction<SignedIntT, kDimension>::first_present_dimension() const
{
  return std::find_if(std::begin(dimension_proportions_),
             std::end(dimension_proportions_),
             [&](SignedIntT component) { return component != 0; })
         - std::begin(dimension_proportions_);
}

//   Operators
//  -----------

/// Tests whether two directions are equivalent.
///
template <typename SignedIntT, std::size_t kDimension>
bool Direction<SignedIntT, kDimension>::operator==(
    Direction<SignedIntT, kDimension> r_op) const
{
  // invariant: internal representation is always simplified and unique.
  return dimension_proportions_ == r_op.dimension_proportions_;
}

/// Tests order of two directions
///
/// \note  This is NOT useful for anything except in places requiring ordering
///        of objects, like some standard library containers.
///
template <typename SignedIntT, std::size_t kDimension>
bool Direction<SignedIntT, kDimension>::operator<(
    Direction<SignedIntT, kDimension> r_op) const
{
  return dimension_proportions_ < r_op.dimension_proportions_;
}

// Related Functions
//-------------------

/// Finds one of 2 unique mutually orthogonal directions to the inputs.
///
/// \note  Some edge cases return edge-case results. All results are based on
///        the cross product. This also means this only works in 3 dimensions.
///
/// \sa  https://en.wikipedia.org/wiki/Cross_product
///
template <typename SignedIntT>
Direction<SignedIntT, 3> mutual_orthogonal(Direction<SignedIntT, 3> l_op,
    Direction<SignedIntT, 3> r_op,
    bool opposite_value = false)
{
  auto ret = cross(l_op.get(), r_op.get());
  if (opposite_value) {
    for (auto& i : ret) {
      i *= -1;
    }
  }
  return Direction<SignedIntT, 3>{ret};
}

//-------------------
// Related Functions

} // namespace rational_geometry

#endif // _RATIONAL_GEOMETRY_DIRECTION_HPP_INCLUDED_

// vim:set et ts=2 sw=2 sts=2:

