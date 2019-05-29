#include "catch.hpp"

// Libraries

#include <iostream>
#include <sstream>

// Includes

#include "geometry/vec.h"
#include "molecule/molecule.h"

// Tests

TEST_CASE("Molecule initialize correctly and prints output", "[constructors] [printers]")
{
  SECTION("Initializing and printing")
  {
    std :: vector<atom> a;

    a.push_back(atom({3, 4}, 1, 1));
    a.push_back(atom({-3, 4}, 1, 1));
    a.push_back(atom({0, 2}, 2, 1));

    molecule m(a);

    REQUIRE(m[0].position() == vec(3., 1.));
    REQUIRE(m[1].position() == vec(-3., 1.));
    REQUIRE(m[2].position() == vec(0, -1.));

    REQUIRE(m.inertia_moment() == 24.);
    REQUIRE(m.mass() == 4.);
  }
}
