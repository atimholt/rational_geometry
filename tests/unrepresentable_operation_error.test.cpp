
#include "../src/rational_geometry/unrepresentable_operation_error.hpp"

#include "doctest.h"

#include <string>
#include <typeinfo>

namespace rational_geometry {


TEST_CASE("Testing unrepresentable_operation_error.hpp")
{
  auto a = unrepresentable_operation_error<int>("This error is a test", 12, 8);

  CHECK(a.get_inadequate_denominator() == 12);
  CHECK(a.get_minimum_fix_factor() == 2);
}


} // namespace rational_geometry

// vim:set et ts=2 sw=2 sts=2:

