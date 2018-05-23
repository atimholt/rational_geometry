# Rational Geometry

A library for manipulating multidimensional geometries that are defined in terms
of rational number coordinates.

Mostly, this library is for taking intersections, unions, and differences
between polytopes in two and three dimensions. While there are math libraries
already capable of doing similar things, they all assume they must deal with
floating point number coordinates, leading to completely different kinds of
calculations under the hood. This library is intended to start with the
assumption that all values calculated with represent exact, infinitely
precise numbers.

No assumption is made (hopefully) about what kind of rational numbers are being
used, but a rational number type is included in the library. While this
rational number type may well be useful in other places, it has a different
set of caveats than someone might be used to if they're used to a programming
language's primitive types (e.g. int, float, etc).

