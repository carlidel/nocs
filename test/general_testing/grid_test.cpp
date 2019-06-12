#include "catch.hpp"

// Libraries

#include <math.h>

// Includes

#include "engine/engine.hpp"
#include "graphics/window.h"

// Tests

TEST_CASE("Grid works correctly", "[grid]")
{
    SECTION("Inserting molecules")
    {
        engine my_engine(4);

        molecule my_molecule(
            {{{0.0, 0.0}, 1., 0.01}},
            {0.4, 0.2},
            {0., 0.},
            0., 0.);
        size_t id = my_engine.add(my_molecule);

        my_engine.each<molecule>([](const molecule & current_molecule)
        {
            REQUIRE(current_molecule.mark.x() == 1);
            REQUIRE(current_molecule.mark.y() == 0);
        });
    }

    SECTION("Inserting bumpers")
    {
        engine my_engine(4);
        bumper my_bumper({0.4, 0.2}, 0.03);
        my_engine.add(my_bumper);

        my_engine.each<bumper>([](const bumper & current_bumper)
        {
            REQUIRE(current_bumper.mark.x() == 1);
            REQUIRE(current_bumper.mark.y() == 0);
        });
    }

    SECTION("Molecule properly navigates the grid")
    {
        engine my_engine(5);
        molecule my_molecule(
            {{{0.0, 0.0}, 1., 0.01}},
            {0.1, 0.1},
            {0.2, 0.2},
            0., 0.);
        size_t id = my_engine.add(my_molecule);

        for (int i = 0; i < 5; i++)
        {
            my_engine.each<molecule>([&](const molecule &current_molecule) {
                REQUIRE(current_molecule.mark.x() == i);
                REQUIRE(current_molecule.mark.y() == i);
            });

            my_engine.run(1.0 + i);
        }
    }

    SECTION("Removal from the grid works")
    {
        engine my_engine(4);

        molecule my_molecule1(
            {{{0.0, 0.0}, 1., 0.01}},
            {0.2, 0.2},
            {0., 0.},
            0., 0.);
        molecule my_molecule2(
            {{{0.0, 0.0}, 1., 0.01}},
            {0.4, 0.4},
            {0., 0.},
            0., 0.);
        molecule my_molecule3(
            {{{0.0, 0.0}, 1., 0.01}},
            {0.7, 0.7},
            {0., 0.},
            0., 0.);
        molecule my_molecule4(
            {{{0.0, 0.0}, 1., 0.01}},
            {0.9, 0.9},
            {0., 0.},
            0., 0.);
        size_t id1 = my_engine.add(my_molecule1);
        size_t id2 = my_engine.add(my_molecule2);
        size_t id3 = my_engine.add(my_molecule3);
        size_t id4 = my_engine.add(my_molecule4);

        int count = 0;
        my_engine.each<molecule>([&](const molecule &current_molecule) {
            count += 1;
        });
        REQUIRE(count == 4);

        my_engine.remove(id1);
        my_engine.remove(id3);

        count = 0;
        my_engine.each<molecule>([&](const molecule &current_molecule) {
            count += 1;
        });
        REQUIRE(count == 2);
    }

    SECTION("Event tree built properly")
    {
        molecule mol1(
            {{{{0.0, 0.0}, 1., 0.01}}},
            {0.2, 0.2},
            {1.5, 1.5});
        molecule mol2(
            {{{{0.0, 0.0}, 1., 0.01}}},
            {0.2, 0.3},
            {1.6, 1.5});
        molecule mol3(
            {{{{0.0, 0.0}, 1., 0.01}}},
            {0.3, 0.2},
            {1.6, 1.5});
        molecule mol4(
            {{{{0.0, 0.0}, 1., 0.01}}},
            {0.3, 0.3},
            {1., 1.});

        molecule mol5(
            {{{{0.0, 0.0}, 1., 0.01}}},
            {0.7, 0.7},
            {-1., -1.});
        molecule mol6(
            {{{{0.0, 0.0}, 1., 0.01}}},
            {0.7, 0.8},
            {-1.5, -1.6});
        molecule mol7(
            {{{{0.0, 0.0}, 1., 0.01}}},
            {0.8, 0.7},
            {-1.6, -1.5});
        molecule mol8(
            {{{{0.0, 0.0}, 1., 0.01}}},
            {0.8, 0.8},
            {-1.5, -1.5});

        // no grid
        engine eng_no_grid(1);

        eng_no_grid.add(mol1);
        eng_no_grid.add(mol2);
        eng_no_grid.add(mol3);
        eng_no_grid.add(mol4);
        eng_no_grid.add(mol5);
        eng_no_grid.add(mol6);
        eng_no_grid.add(mol7);
        eng_no_grid.add(mol8);

        REQUIRE(eng_no_grid.event_heap_size() == 25);

        // with grid
        engine eng_grid(6);

        eng_grid.add(mol1);
        eng_grid.add(mol2);
        eng_grid.add(mol3);
        eng_grid.add(mol4);
        eng_grid.add(mol5);
        eng_grid.add(mol6);
        eng_grid.add(mol7);
        eng_grid.add(mol8);
        
        REQUIRE(eng_grid.event_heap_size() == 11);
    }
}