/// \file     Operations.hpp
/// \author   Tim Holt
///
/// Generic operations for dealing with rational geometric objects.
///
/// This code is under the MIT license, please see LICENSE.txt for more
/// information

#ifndef _RATIONAL_GEOMETRY_OPERATIONS_HPP_INCLUDED_
#define _RATIONAL_GEOMETRY_OPERATIONS_HPP_INCLUDED_

// Includes
//----------

#include <utility>

//----------
// Includes


/// Find the dot product (<i>scalar</i> product) between two vectors.
///
/// \return  a value equivalent to the magnitude of the two vectors multiplied
///          together, multiplied by the cosine of the angle between them.
///
/// \note  This can take two 1D containers of arbitrary (random access) type,
///        for the purposes of making Matrix multiplication really easy to
///        implement (think row and column classes with no inheritance
///        relation).
///
/// \note  Hypothetically, this could be implemented to take any type that is
///        forward iterable, but I don't see the use case, and it makes some
///        things elsewhere harder.
///
/// \sa  https://en.wikipedia.org/wiki/Dot_product
///
template <typename RatT_l,
    typename RatT_r,
    std::size_t kDimension,
    template <typename, size_t> typename TContainer_l,
    template <typename, size_t> typename TContainer_r>
auto dot(const TContainer_l<RatT_l, kDimension>& l_op,
    const TContainer_r<RatT_r, kDimension>& r_op)
{
  using std::declval;
  // clang-format off
  decltype(declval<RatT_l>() * declval<RatT_r>()
           +
           declval<RatT_l>() * declval<RatT_r>()) sum{0};
  // clang-format on
  for (std::size_t i = 0; i < kDimension; ++i) {
    sum += l_op[i] * r_op[i];
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
/// \note  This does not take two arbitrary container types, since the output
///        type needs to be known, and favoring one as the default output type
///        is silly, and allowing templatized output type specification is
///        dubious.
///
/// \sa  https://en.wikipedia.org/wiki/Cross_product
///
template <typename RatT_l,
    typename RatT_r,
    template <typename, size_t> typename TContainer>
auto cross(const TContainer<RatT_l, 3>& l_op, const TContainer<RatT_r, 3>& r_op)
{
  using std::declval;
  // clang-format off
  TContainer<decltype(declval<RatT_l>() * declval<RatT_r>()
                      -
                      declval<RatT_l>() * declval<RatT_r>()), 3> ret{};
  // clang-format on

  ret[0] = l_op[1] * r_op[2] - l_op[2] * r_op[1];
  ret[1] = l_op[2] * r_op[0] - l_op[0] * r_op[2];
  ret[2] = l_op[0] * r_op[1] - l_op[1] * r_op[0];

  return ret;
}

#endif // _RATIONAL_GEOMETRY_OPERATIONS_HPP_INCLUDED_

// vim:set et ts=2 sw=2 sts=2:

