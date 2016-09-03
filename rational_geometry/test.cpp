// test.cpp

// This flag exists so users can also use doctest without having to run my tests
// again.
#define RATIONAL_GEOMETRY_THIS_IS_THE_TEST_BINARY

// To test templates, they must be in a translation unit *somewhere*.
#include "Point.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

