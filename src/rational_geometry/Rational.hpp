/// \file    Rational.hpp
/// \author  Tim Holt
///
/// A faster rational number type and its related functions.
///
/// \todo  Make it possible to include the file multiple times with differing
///        flags.
///
/// \todo  Test all permutations of user-settable flags (requires multiple
///        inclusion ability).
///
/// \todo  Consider removing absolutely all overhead when not checking for
///        overflow (this would require the worst kind of code duplication)
///
/// This code is under the MIT license, please see LICENSE.txt for more
/// information

#ifndef _RATIONAL_GEOMETRY_RATIONAL_HPP_INCLUDED_
#define _RATIONAL_GEOMETRY_RATIONAL_HPP_INCLUDED_

// Includes
//----------

#include <cmath>
#include <iostream>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>

#ifndef RATIONAL_GEOMETRY_DONT_THROW_ON_INEXACT_OPERATION
#include "unrepresentable_operation_error.hpp"

#include <sstream>
#include <string>
#endif

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
/// the rest of the rational number library.</b> Any sufficiently sane rational
/// number type (e.g. boost::rational) may be used in the rest of this library.
///
/// Theoretically, this class is as fast as its underlying integer type in
/// multiplication/division (by an integer) and same-instantiated-type
/// addition. It is also faster than, say, boost::rational in nearly every
/// operation. (Factuality of this statement is subject to actual benchmarking,
/// not undertaken. It should be noted that boost/rational admits it is not
/// actually meant to be speedy, though.)
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
/// time--a composite number for kDenominator sufficient to render <i>all</i>
/// the rational values you wish to represent within your project (or at least
/// one that covers the domain of accuracy you desire). To this end, facility
/// is provided for throwing exceptions (of type
/// rational_geometry::unrepresentable_operation_error) when rounding errors
/// occur. These exceptions have a field (.remaining_divisor_) representing the
/// (ideally) small number needed to multiply by the value you have chosen for
/// kDenominator to prevent the exact same instigating operation from throwing
/// after a recompile & re-run. Because this rounding error detection has
/// overhead, its code is toggleable using the
/// RATIONAL_GEOMETRY_DONT_THROW_ON_INEXACT_OPERATION precompiler flag. Simply
/// define it before including rational_geometry/Rational.hpp.
///
/// The library user might alternatively choose to use the
/// RATIONAL_GEOMETRY_DONT_THROW_ON_INEXACT_OPERATION flag to simply disregard
/// innacuracies beyond their chosen fixed denominator.
///
/// The unrepresentable_operation_error class has a method
/// (.accumulate_fix_factor(IntType&)) which automatically modifies the
/// variable passed to it to the accumulated least common multiple of all such
/// exceptions upon which the accumulate_fix_factor() method is called. The int
/// passed to it should be the same type as the signed int type upon which you
/// have instantiated the Rational class template, and must be initialized to a
/// value of 1 before use. This accumulation will probably have to be performed
/// in the catch block of try-catch statements in your unit tests. Write an
/// assertion that the int is still equal to 1 after all accumulations to
/// ensure that your chosen value for kDimension is sufficient. If the
/// assertion fails, the variable's value is exactly that number you need to
/// multiply kDimension by for all your tests to fall within your chosen domain
/// of accuracy.
///
/// For reasons of speed, this library can be set to perform operations which
/// run a higher risk of overflowing the underlying integer type, via the
/// RATIONAL_GEOMETRY_SKIP_OVERFLOW_PROTECTIONS flag. <b>Using this flag may
/// cause overflows in places that will be unexpected if the library user does
/// not have a working knowledge of how the Rational class works internally</b>
/// For a wide variety of use cases, this is not a problem: integer
/// multiplication, addition, and values much smaller than the
/// std::numeric_limits::max<>() for the underlying integer type are
/// particularly safe. The library user may skirt closer to the limits of the
/// class if they are careful (i.e., write lots of tests that cover your actual
/// usage, particularly tests that check that operations that should fail
/// actually do!). If you're really worried, use a large integer type (or don't
/// set the flag in the first place). The library user may turn off overflow
/// protection if they are instantiating Rational upon some manner of infinite
/// precision integer type (these are typically called something like BigInt).
/// Be aware of the pros & cons of your chosen integer type.
///
/// \note  In its pre-alpha state (and probably well beyond that), this library
///        will remain un-benchmarked. Its primary use, for some time, will be
///        as a rational number type that adds no new external dependencies to
///        the rational_geometry library.
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
  explicit Rational(int value);
  Rational(SignedIntT numerator, SignedIntT denominator);
  explicit Rational(long double value);
  explicit Rational(double value);
  explicit Rational(float value);

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

// Helper Functions
//------------------

template <typename IntT>
struct PartialDivisionResult
{
  IntT partial_result_;
  IntT remaining_divisor_;
};

template <typename IntT>
PartialDivisionResult<IntT> partial_division(IntT top_int, IntT bottom_int)
{
#ifndef RATIONAL_GEOMETRY_SKIP_OVERFLOW_PROTECTIONS
  PartialDivisionResult<IntT> ret;

  auto common_factor     = gcd(top_int, bottom_int);
  ret.partial_result_    = top_int / common_factor;
  ret.remaining_divisor_ = bottom_int / common_factor;

  return ret;
#else
  return {top_int / bottom_int, 1};
#endif
}

template <typename IntT>
PartialDivisionResult<IntT> partial_division(
    const std::vector<IntT>& top_ints, IntT bottom_int)
{
#ifndef RATIONAL_GEOMETRY_SKIP_OVERFLOW_PROTECTIONS
  using namespace std;
  PartialDivisionResult<IntT> init{1, bottom_int};

  return std::accumulate(cbegin(top_ints), cend(top_ints), init,
      [](PartialDivisionResult<IntT> so_far, IntT current_numerator) {
        auto current_result =
            partial_division(current_numerator, so_far.remaining_divisor_);
        current_result.partial_result_ *= so_far.partial_result_;
        return current_result;
      });
#else
  return {std::accumulate(cbegin(top_ints), cend(top_ints), 1, std::multiplies)
              / bottom_int,
      1};
#endif
}

// Class Template Definitions
//----------------------------
//   Constructors
//  --------------

template <typename SignedIntT, SignedIntT kDenominator>
Rational<SignedIntT, kDenominator>::Rational() : numerator_{0}
{
}

template <typename SignedIntT, SignedIntT kDenominator>
Rational<SignedIntT, kDenominator>::Rational(int value)
    : numerator_{value * kDenominator}
{
}

template <typename SignedIntT, SignedIntT kDenominator>
Rational<SignedIntT, kDenominator>::Rational(
    SignedIntT numerator, SignedIntT denominator)
    : numerator_{denominator == kDenominator ?
                     numerator :
                     partial_division({numerator, kDenominator}, denominator)
                         .partial_result_}
{
#ifndef RATIONAL_GEOMETRY_DONT_THROW_ON_INEXACT_OPERATION
  if (denominator != kDenominator && (numerator * kDenominator) % denominator) {
    std::stringstream what_error;
    what_error << "Inexact construction of a Rational<"
               << typeid(SignedIntT).name() << ", " << kDenominator << ">";
    throw unrepresentable_operation_error<SignedIntT>(
        what_error.str(), numerator * kDenominator, denominator);
  }
#endif
}

/// Creates nearest representable Rational to the given value.
///
/// \note  While this has more use than many possible float-to-rational
///        conversions, a float of sufficiently high magnitude will not have
///        enough resolution for this method to be accurate.
///
template <typename SignedIntT, SignedIntT kDenominator>
Rational<SignedIntT, kDenominator>::Rational(long double value)
    : numerator_{static_cast<SignedIntT>(std::round(value * kDenominator))}
{
}

template <typename SignedIntT, SignedIntT kDenominator>
Rational<SignedIntT, kDenominator>::Rational(double value)
    : Rational(static_cast<long double>(value))
{
}

template <typename SignedIntT, SignedIntT kDenominator>
Rational<SignedIntT, kDenominator>::Rational(float value)
    : Rational(static_cast<long double>(value))
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
  SignedIntT ret{-numerator_};
  return *reinterpret_cast<Rational<SignedIntT, kDenominator>*>(&ret);
}

// Related Operators
//-------------------
//   Comparison
//  ------------
//     Equality
//    ----------

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

//     Inequality
//    ------------

template <typename SignedIntT_l, typename IntT_r, SignedIntT_l kDenominator>
auto operator!=(Rational<SignedIntT_l, kDenominator> l_op, IntT_r r_op) ->
    typename std::enable_if<std::is_integral<IntT_r>::value, bool>::type
{
  return !(l_op == r_op);
}

template <typename IntT_l, typename SignedIntT_r, SignedIntT_r kDenominator>
auto operator!=(IntT_l l_op, Rational<SignedIntT_r, kDenominator> r_op) ->
    typename std::enable_if<std::is_integral<IntT_l>::value, bool>::type
{
  return !(l_op == r_op);
}

template <typename SignedIntT, SignedIntT kDenominator>
bool operator!=(Rational<SignedIntT, kDenominator> l_op,
    Rational<SignedIntT, kDenominator> r_op)
{
  return !(l_op == r_op);
}

//     Less Than
//    -----------

/// Determine if a rational is less than an int
///
/// Floating type comparison is precluded because of the special case where
/// both operands are equal (either actually or conceptually so). The result in
/// this case would imply accuracy where there is none.
///
/// Converting either operand to the type of the other allows the library user
/// to be explicit about what they really want.
///
template <typename SignedIntT_l, typename IntT_r, SignedIntT_l kDenominator>
auto operator<(Rational<SignedIntT_l, kDenominator> l_op, IntT_r r_op) ->
    typename std::enable_if<std::is_integral<IntT_r>::value, bool>::type
{
#ifndef RATIONAL_GEOMETRY_SKIP_OVERFLOW_PROTECTIONS
  if (l_op.numerator() / kDenominator < r_op) return true;
  if (l_op.numerator() / kDenominator > r_op) return false;
  auto ret = partial_division({r_op, kDenominator}, l_op.numerator());
  return ret.remaining_divisor_ < ret.partial_result_;
#else
  return l_op.numerator() < r_op * kDenominator;
#endif
}

/// Determine if an int is less than a rational
///
/// Floating type comparison is precluded because of the special case where
/// both operands are equal (either actually or conceptually so). The result in
/// this case would imply accuracy where there is none.
///
/// Converting either operand to the type of the other allows the library user
/// to be explicit about what they really want.
///
template <typename IntT_l, typename SignedIntT_r, SignedIntT_r kDenominator>
auto operator<(IntT_l l_op, Rational<SignedIntT_r, kDenominator> r_op) ->
    typename std::enable_if<std::is_integral<IntT_l>::value, bool>::type
{
#ifndef RATIONAL_GEOMETRY_SKIP_OVERFLOW_PROTECTIONS
  if (l_op < r_op.numerator() / kDenominator) return true;
  if (l_op > r_op.numerator() / kDenominator) return false;
  auto ret = partial_division({l_op, kDenominator}, r_op.numerator());
  return ret.partial_result_ < ret.remaining_divisor_;
#else
  return l_op * kDenominator < r_op.numerator();
#endif
}

/// Determine if a rational is less than another rational of the same type.
///
template <typename SignedIntT, SignedIntT kDenominator>
bool operator<(Rational<SignedIntT, kDenominator> l_op,
    Rational<SignedIntT, kDenominator> r_op)
{
  return l_op.numerator() < r_op.numerator();
}

//     Greater Than
//    --------------

template <typename SignedIntT_l, typename IntT_r, SignedIntT_l kDenominator>
auto operator>(Rational<SignedIntT_l, kDenominator> l_op, IntT_r r_op) ->
    typename std::enable_if<std::is_integral<IntT_r>::value, bool>::type
{
  return r_op < l_op;
}

template <typename IntT_l, typename SignedIntT_r, SignedIntT_r kDenominator>
auto operator>(IntT_l l_op, Rational<SignedIntT_r, kDenominator> r_op) ->
    typename std::enable_if<std::is_integral<IntT_l>::value, bool>::type
{
  return r_op < l_op;
}

template <typename SignedIntT, SignedIntT kDenominator>
bool operator>(Rational<SignedIntT, kDenominator> l_op,
    Rational<SignedIntT, kDenominator> r_op)
{
  return r_op < l_op;
}

//     Less Than or Equal To
//    -----------------------

template <typename SignedIntT_l, typename IntT_r, SignedIntT_l kDenominator>
auto operator<=(Rational<SignedIntT_l, kDenominator> l_op, IntT_r r_op) ->
    typename std::enable_if<std::is_integral<IntT_r>::value, bool>::type
{
  return !(r_op < l_op);
}

template <typename IntT_l, typename SignedIntT_r, SignedIntT_r kDenominator>
auto operator<=(IntT_l l_op, Rational<SignedIntT_r, kDenominator> r_op) ->
    typename std::enable_if<std::is_integral<IntT_l>::value, bool>::type
{
  return !(r_op < l_op);
}

template <typename SignedIntT, SignedIntT kDenominator>
bool operator<=(Rational<SignedIntT, kDenominator> l_op,
    Rational<SignedIntT, kDenominator> r_op)
{
  return !(r_op < l_op);
}

//     Greater Than or Equal To
//    --------------------------

template <typename SignedIntT_l, typename IntT_r, SignedIntT_l kDenominator>
auto operator>=(Rational<SignedIntT_l, kDenominator> l_op, IntT_r r_op) ->
    typename std::enable_if<std::is_integral<IntT_r>::value, bool>::type
{
  return !(l_op < r_op);
}

template <typename IntT_l, typename SignedIntT_r, SignedIntT_r kDenominator>
auto operator>=(IntT_l l_op, Rational<SignedIntT_r, kDenominator> r_op) ->
    typename std::enable_if<std::is_integral<IntT_l>::value, bool>::type
{
  return !(l_op < r_op);
}

template <typename SignedIntT, SignedIntT kDenominator>
bool operator>=(Rational<SignedIntT, kDenominator> l_op,
    Rational<SignedIntT, kDenominator> r_op)
{
  return !(l_op < r_op);
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

template <typename SignedIntT, SignedIntT kDenominator>
Rational<SignedIntT, kDenominator> operator*(
    Rational<SignedIntT, kDenominator> l_op,
    Rational<SignedIntT, kDenominator> r_op)
{
  auto result =
      partial_division({l_op.numerator(), r_op.numerator()}, kDenominator);
#ifndef RATIONAL_GEOMETRY_DONT_THROW_ON_INEXACT_OPERATION
  if (result.remaining_divisor_ != 1) {
    std::stringstream what_error;
    // clang-format off
    what_error << "Inexact operation in ("
               << typeid(l_op).name() << " " << l_op
               << " * "
               << typeid(r_op).name() << " " << r_op << "):  "
               << result.partial_result_ << "/" << result.remaining_divisor_
               << " -> " << typeid(SignedIntT).name();
    // clang-format on
    throw unrepresentable_operation_error<SignedIntT>{
        what_error.str(), result.partial_result_, result.remaining_divisor_};
  }
#endif
  SignedIntT ret{result.partial_result_};
  return *reinterpret_cast<Rational<SignedIntT, kDenominator>*>(&ret);
}

//     Division
//    ----------

template <typename SignedIntT_l, typename IntT_r, SignedIntT_l kDenominator>
auto operator/(Rational<SignedIntT_l, kDenominator> l_op, IntT_r r_op) ->
    typename std::enable_if<std::is_integral<IntT_r>::value,
        Rational<SignedIntT_l, kDenominator>>::type
{
#ifndef RATIONAL_GEOMETRY_DONT_THROW_ON_INEXACT_OPERATION
  auto top_int    = l_op.numerator();
  auto bottom_int = r_op;
  if (top_int % bottom_int) {
    std::stringstream what_error;
    // clang-format off
    what_error << "Inexact operation in ("
               << typeid(l_op).name() << " " << l_op
               << " / "
               << typeid(r_op).name() << " " << r_op << "):  "
               << top_int << "/" << bottom_int
               << " -> " << typeid(SignedIntT_l).name();
    // clang-format on
    throw unrepresentable_operation_error<SignedIntT_l>{
        what_error.str(), top_int, bottom_int};
  }
#endif
  SignedIntT_l ret{l_op.numerator() / r_op};
  return *reinterpret_cast<Rational<SignedIntT_l, kDenominator>*>(&ret);
}

template <typename IntT_l, typename SignedIntT_r, SignedIntT_r kDenominator>
auto operator/(IntT_l l_op, Rational<SignedIntT_r, kDenominator> r_op) ->
    typename std::enable_if<std::is_integral<IntT_l>::value,
        Rational<SignedIntT_r, kDenominator>>::type
{
  auto result =
      partial_division({l_op, kDenominator, kDenominator}, r_op.numerator());
#ifndef RATIONAL_GEOMETRY_DONT_THROW_ON_INEXACT_OPERATION
  if (result.remaining_divisor_ != 1) {
    std::stringstream what_error;
    // clang-format off
    what_error << "Inexact operation in ("
               << typeid(l_op).name() << " " << l_op
               << " / "
               << typeid(r_op).name() << " " << r_op << "):  "
               << result.partial_result_ << "/" << result.remaining_divisor_
               << " -> " << typeid(SignedIntT_r).name();
    // clang-format on
    throw unrepresentable_operation_error<SignedIntT_r>{
        what_error.str(), result.partial_result_, result.remaining_divisor_};
  }
#endif
  SignedIntT_r ret{result.partial_result_};
  return *reinterpret_cast<Rational<SignedIntT_r, kDenominator>*>(&ret);
}

template <typename SignedIntT, SignedIntT kDenominator>
Rational<SignedIntT, kDenominator> operator/(
    Rational<SignedIntT, kDenominator> l_op,
    Rational<SignedIntT, kDenominator> r_op)
{
  auto result =
      partial_division({l_op.numerator(), kDenominator}, r_op.numerator());
#ifndef RATIONAL_GEOMETRY_DONT_THROW_ON_INEXACT_OPERATION
  if (result.remaining_divisor_ != 1) {
    std::stringstream what_error;
    // clang-format off
    what_error << "Inexact operation in ("
               << typeid(l_op).name() << " " << l_op
               << " / "
               << typeid(r_op).name() << " " << r_op << "):  "
               << result.partial_result_ << "/" << result.remaining_divisor_
               << " -> " << typeid(SignedIntT).name();
    // clang-format on
    throw unrepresentable_operation_error<SignedIntT>{
        what_error.str(), result.partial_result_, result.remaining_divisor_};
  }
#endif
  SignedIntT ret{result.partial_result_};
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

//   ostream Output
//  ----------------

template <typename SignedIntT, SignedIntT kDenominator>
std::ostream& operator<<(
    std::ostream& the_stream, const Rational<SignedIntT, kDenominator>& value)
{
  the_stream << value.numerator() << '/' << kDenominator;
  return the_stream;
}

// Related Functions
//-------------------

template <typename SignedIntT, SignedIntT kDenominator>
std::pair<SignedIntT, SignedIntT> simplify(
    Rational<SignedIntT, kDenominator> value)
{
  auto ret = partial_division(value.numerator(), value.denominator());
  return *reinterpret_cast<std::pair<SignedIntT, SignedIntT>*>(&ret);
}

//-------------------
// Related Functions

} // namespace rational_geometry

#endif // _RATIONAL_GEOMETRY_RATIONAL_HPP_INCLUDED_

// vim:set et ts=2 sw=2 sts=2:

