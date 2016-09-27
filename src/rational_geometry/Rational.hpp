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

#include <type_traits>

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
/// multiplication/division (by an integer) and same-instantiated-type
/// addition. It is also faster than, say, boost::rational in nearly every
/// operation.
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
      "kDenominator template argument of rational_geometry::Rational<> class "
      "must be positive.");

  static_assert(
      std::is_integral<SignedIntT>::value, "SignedIntT must be integer type");

  static_assert(
      std::is_signed<SignedIntT>::value, "SignedIntT must be a signed type");

  // INTERNAL STATE
  SignedIntT numerator_;

 public:
  // CONSTRUCTORS
  Rational();
  explicit Rational(SignedIntT value);
  Rational(SignedIntT numerator, SignedIntT denominator);

  // ACCESSORS
  SignedIntT numerator() const;
  SignedIntT denominator() const;

  long double as_long_double() const;

  // OPERATORS
  Rational& operator++();
  Rational operator++(int);

  Rational& operator--();
  Rational operator--(int);

  Rational operator-() const;
};

// Class Template Definitions
//----------------------------
//   Constructors
//  --------------

template <typename SignedIntT, SignedIntT kDenominator>
Rational<SignedIntT, kDenominator>::Rational() : numerator_{0}
{
}

template <typename SignedIntT, SignedIntT kDenominator>
Rational<SignedIntT, kDenominator>::Rational(SignedIntT value)
    : numerator_{value * kDenominator}
{
}

/// \todo  consider simplifying input to reduce risk of overflow
///
template <typename SignedIntT, SignedIntT kDenominator>
Rational<SignedIntT, kDenominator>::Rational(
    SignedIntT numerator, SignedIntT denominator)
    : numerator_{denominator == kDenominator ?
                     numerator :
                     (numerator * kDenominator) / denominator}
{
}

//   Accessors
//  -----------

template <typename SignedIntT, SignedIntT kDenominator>
SignedIntT Rational<SignedIntT, kDenominator>::numerator() const
{
  return numerator_;
}

template <typename SignedIntT, SignedIntT kDenominator>
SignedIntT Rational<SignedIntT, kDenominator>::denominator() const
{
  return kDenominator;
}


/// Get the long double equivalent of the rational number.
///
/// This is used instead of an operator long double() because such a function
/// implies that conversion to a long double brings no loss of functionality.
/// However, a long double is not adequate in every place a rational number is
/// needed for the same reasons this rational number class exists in the first
/// place.
///
template <typename SignedIntT, SignedIntT kDenominator>
long double Rational<SignedIntT, kDenominator>::as_long_double() const
{
  return static_cast<long double>(numerator()) / denominator();
}

//   Operators
//  -----------

template <typename SignedIntT, SignedIntT kDenominator>
Rational<SignedIntT, kDenominator>& Rational<SignedIntT, kDenominator>::
operator++()
{
  numerator_ += kDenominator;
  return *this;
}

template <typename SignedIntT, SignedIntT kDenominator>
Rational<SignedIntT, kDenominator> Rational<SignedIntT, kDenominator>::
operator++(int)
{
  auto ret = *this;
  ++(*this);
  return ret;
}


template <typename SignedIntT, SignedIntT kDenominator>
Rational<SignedIntT, kDenominator>& Rational<SignedIntT, kDenominator>::
operator--()
{
  numerator_ -= kDenominator;
  return *this;
}

template <typename SignedIntT, SignedIntT kDenominator>
Rational<SignedIntT, kDenominator> Rational<SignedIntT, kDenominator>::
operator--(int)
{
  auto ret = *this;
  --(*this);
  return ret;
}


template <typename SignedIntT, SignedIntT kDenominator>
Rational<SignedIntT, kDenominator> Rational<SignedIntT, kDenominator>::
operator-() const
{
  return {-numerator(), kDenominator};
}

// Related Operators
//-------------------
//   Comparison
//  ------------

template <typename SignedIntT_l, typename IntT_r, SignedIntT_l kDenominator>
auto operator==(Rational<SignedIntT_l, kDenominator> l_op, IntT_r r_op) ->
    typename std::enable_if<std::is_integral<IntT_r>::value, bool>::type
{
  return l_op.numerator() == r_op * kDenominator;
}

template <typename IntT_l, typename SignedIntT_r, SignedIntT_r kDenominator>
auto operator==(IntT_l l_op, Rational<SignedIntT_r, kDenominator> r_op) ->
    typename std::enable_if<std::is_integral<IntT_l>::value, bool>::type
{
  return l_op * kDenominator == r_op.numerator();
}

template <typename SignedIntT, SignedIntT kDenominator>
bool operator==(Rational<SignedIntT, kDenominator> l_op,
    Rational<SignedIntT, kDenominator> r_op)
{
  return l_op.numerator() == r_op.numerator();
}

//   Arithmetic
//  ------------
//     Multiplication
//    ----------------

template <typename SignedIntT_l, typename IntT_r, SignedIntT_l kDenominator>
auto operator*(Rational<SignedIntT_l, kDenominator> l_op, IntT_r r_op) ->
    typename std::enable_if<std::is_integral<IntT_r>::value,
        Rational<SignedIntT_l, kDenominator>>::type
{
  SignedIntT_l ret{l_op.numerator() * r_op};
  return *reinterpret_cast<Rational<SignedIntT_l, kDenominator>*>(&ret);
}

template <typename IntT_l, typename SignedIntT_r, SignedIntT_r kDenominator>
auto operator*(IntT_l l_op, Rational<SignedIntT_r, kDenominator> r_op) ->
    typename std::enable_if<std::is_integral<IntT_l>::value,
        Rational<SignedIntT_r, kDenominator>>::type
{
  SignedIntT_r ret{l_op * r_op.numerator()};
  return *reinterpret_cast<Rational<SignedIntT_r, kDenominator>*>(&ret);
}

/// \todo  add compile-time option for throwing an exception outside of this
///        this operator's accuracy.
///
template <typename SignedIntT, SignedIntT kDenominator>
Rational<SignedIntT, kDenominator> operator*(
    Rational<SignedIntT, kDenominator> l_op,
    Rational<SignedIntT, kDenominator> r_op)
{
  SignedIntT ret{l_op.numerator() * r_op.numerator()};
  ret /= kDenominator;
  return *reinterpret_cast<Rational<SignedIntT, kDenominator>*>(&ret);
}

//     Division
//    ----------

/// \todo  add compile-time option for throwing an exception outside of this
///        this operator's accuracy.
///
template <typename SignedIntT_l, typename IntT_r, SignedIntT_l kDenominator>
auto operator/(Rational<SignedIntT_l, kDenominator> l_op, IntT_r r_op) ->
    typename std::enable_if<std::is_integral<IntT_r>::value,
        Rational<SignedIntT_l, kDenominator>>::type
{
  SignedIntT_l ret{l_op.numerator() / r_op};
  return *reinterpret_cast<Rational<SignedIntT_l, kDenominator>*>(&ret);
}

/// \todo  add compile-time option for throwing an exception outside of this
///        this operator's accuracy.
///
template <typename IntT_l, typename SignedIntT_r, SignedIntT_r kDenominator>
auto operator/(IntT_l l_op, Rational<SignedIntT_r, kDenominator> r_op) ->
    typename std::enable_if<std::is_integral<IntT_l>::value,
        Rational<SignedIntT_r, kDenominator>>::type
{
  SignedIntT_r ret{(l_op * kDenominator * kDenominator) / r_op.numerator()};
  return *reinterpret_cast<Rational<SignedIntT_r, kDenominator>*>(&ret);
}

/// \todo  add compile-time option for throwing an exception outside of this
///        this operator's accuracy.
///
template <typename SignedIntT, SignedIntT kDenominator>
Rational<SignedIntT, kDenominator> operator/(
    Rational<SignedIntT, kDenominator> l_op,
    Rational<SignedIntT, kDenominator> r_op)
{
  SignedIntT ret{(l_op.numerator() * kDenominator) / r_op.numerator()};
  return *reinterpret_cast<Rational<SignedIntT, kDenominator>*>(&ret);
}

//     Addition
//    ----------

template <typename SignedIntT_l, typename IntT_r, SignedIntT_l kDenominator>
auto operator+(Rational<SignedIntT_l, kDenominator> l_op, IntT_r r_op) ->
    typename std::enable_if<std::is_integral<IntT_r>::value,
        Rational<SignedIntT_l, kDenominator>>::type
{
  SignedIntT_l ret{l_op.numerator() + r_op * kDenominator};
  return *reinterpret_cast<Rational<SignedIntT_l, kDenominator>*>(&ret);
}

template <typename IntT_l, typename SignedIntT_r, SignedIntT_r kDenominator>
auto operator+(IntT_l l_op, Rational<SignedIntT_r, kDenominator> r_op) ->
    typename std::enable_if<std::is_integral<IntT_l>::value,
        Rational<SignedIntT_r, kDenominator>>::type
{
  SignedIntT_r ret{l_op * kDenominator + r_op.numerator()};
  return *reinterpret_cast<Rational<SignedIntT_r, kDenominator>*>(&ret);
}

template <typename SignedIntT, SignedIntT kDenominator>
Rational<SignedIntT, kDenominator> operator+(
    Rational<SignedIntT, kDenominator> l_op,
    Rational<SignedIntT, kDenominator> r_op)
{
  SignedIntT ret{l_op.numerator() + r_op.numerator()};
  return *reinterpret_cast<Rational<SignedIntT, kDenominator>*>(&ret);
}

//     Subtraction
//    -------------

template <typename SignedIntT_l, typename IntT_r, SignedIntT_l kDenominator>
auto operator-(Rational<SignedIntT_l, kDenominator> l_op, IntT_r r_op) ->
    typename std::enable_if<std::is_integral<IntT_r>::value,
        Rational<SignedIntT_l, kDenominator>>::type
{
  SignedIntT_l ret{l_op.numerator() - r_op * kDenominator};
  return *reinterpret_cast<Rational<SignedIntT_l, kDenominator>*>(&ret);
}

template <typename IntT_l, typename SignedIntT_r, SignedIntT_r kDenominator>
auto operator-(IntT_l l_op, Rational<SignedIntT_r, kDenominator> r_op) ->
    typename std::enable_if<std::is_integral<IntT_l>::value,
        Rational<SignedIntT_r, kDenominator>>::type
{
  SignedIntT_r ret{l_op * kDenominator - r_op.numerator()};
  return *reinterpret_cast<Rational<SignedIntT_r, kDenominator>*>(&ret);
}

template <typename SignedIntT, SignedIntT kDenominator>
Rational<SignedIntT, kDenominator> operator-(
    Rational<SignedIntT, kDenominator> l_op,
    Rational<SignedIntT, kDenominator> r_op)
{
  SignedIntT ret{l_op.numerator() - r_op.numerator()};
  return *reinterpret_cast<Rational<SignedIntT, kDenominator>*>(&ret);
}

//-------------------
// Related Operators

} // namespace rational_geometry

#endif // _RATIONAL_GEOMETRY_RATIONAL_HPP_INCLUDED_

// vim:set et ts=2 sw=2 sts=2:

