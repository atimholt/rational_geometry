/// \file    Rational.hpp
/// \author  Tim Holt
///
/// A faster rational number type and its related functions.
///
/// This code is under the MIT license, please see LICENSE.txt for more
/// information

#ifndef _RATIONAL_GEOMETRY_RATIONAL_HPP_INCLUDED_
#define _RATIONAL_GEOMETRY_RATIONAL_HPP_INCLUDED_

// Includes
//----------


//----------
// Includes

namespace rational_geometry {

// Class Template Declaration
//----------------------------

/// A faster rational number type with a narrower use case.
///
/// Basically, it's a fixed-point fractional type with an arbitrary
/// denominator and optional rounding detection.
///
/// <b>Use of this particular rational type is <i>not</i> required for use of
/// the rest of the ZGeo library.</b> Any sufficiently sane rational number
/// type (e.g. boost::rational) may be used in the rest of this library.
///
/// Theoretically, this class is as fast as its underlying integer type in
/// (integer) multiplication and same-instantiated-type addition. It is also
/// faster than, say, boost::rational in nearly every operation.
///
/// This is because the denominator is templatized away from the run-time
/// internal representation. As a consequence, only a single integer value is
/// passed around and, of course, the fraction is never internally rendered
/// into its most simplified form. That is, common factors between the
/// numerator and denominator are never canceled away unless the library user
/// explicitly asks for such a representation. Even then, this requested
/// representation <i>cannot</i> be reflected back into the Rational<> value's
/// internals. A different instantiation of the Rational<> template would be
/// necessary (but this is not the recommended use).
///
/// The creator of this library recommends determining--at or before compile
/// time--a composite number sufficient to render <i>all</i> the rational
/// values you wish to represent within your project. To this end, facility is
/// [planned to be] provided for throwing exceptions when rounding errors
/// occur. These exceptions [will] have a field representing the (ideally)
/// small number needed to multiply the value of kDenominator by to prevent the
/// exact same instigating operation from throwing after a recompile & re-run.
/// Because this rounding error detection has overhead, its code [will be]
/// separate from the intended end-use code. Additionally, rounding is not an
/// error unless you want it to be. Just keep in mind that allowing uses that
/// cause rounding removes most of the meaning/purpose of the == operator.
///
/// For reasons of speed (and ease of implementation, to some degree), [it is
/// currently planned that] this library will perform operations which run a
/// higher risk of overflowing the underlying integer type. That is, performing
/// operations between two numerators before involving denominators alleviates
/// any need to determine greatest common factors, etc.. For a wide variety of
/// use cases, this is not a problem: integer multiplication, addition, and
/// values much smaller than the std::numeric_limits::max<>() for the
/// underlying integer type are particularly safe. The library user may skirt
/// closer to the limits of the class if they are careful (i.e., write lots of
/// tests that cover your actual usage, particularly tests that check that
/// operations that should fail actually do!). If you're really worried, use a
/// large integer type.
///
/// \note  In its pre-alpha state (and probably well beyond that), this library
///        will remain un-benchmarked. Its primary use, for some time, will be
///        as a rational number type that adds no new external dependencies.
///
template <typename SignedIntT, SignedIntT kDenominator>
class Rational
{
  // STATIC ASSERTIONS
  static_assert(kDenominator > 0,
      "kDenominator template argument of Rational<> class must be positive.");

 public:
  // OPERATORS
  operator double();
  Rational& operator++();
  Rational operator++(int);
};

//----------------------------
// Class Template Declaration

} // namespace rational_geometry

#endif // _RATIONAL_GEOMETRY_RATIONAL_HPP_INCLUDED_

// vim:set et ts=2 sw=2 sts=2:

