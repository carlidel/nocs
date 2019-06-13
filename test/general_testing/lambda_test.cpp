#include "catch.hpp"

// Includes

#include "engine/engine.hpp"
#include "graphics/window.h"

// Tests

TEST_CASE("Tag system and data gathering works correctly", "[data] [tags] [lambdas]")
{
    SECTION("Event Data gathering works (no tag)")
    {
        engine my_engine(1);
        
        molecule mol1(
            {{{{0.0, 0.0}, 1., 0.05}}},
            {0.20, 0.5},
            {1, 0});
        molecule mol2(
            {{{{0.0, 0.0}, 1., 0.05}}},
            {0.80, 0.5},
            {-1, 0});

        size_t id1 = my_engine.add(mol1);
        size_t id2 = my_engine.add(mol2);

        int caught_count = 0;

        my_engine.on<events::molecule>([&](const report<events::molecule> my_report) {
            caught_count += 1;
        });

        my_engine.run(5.3);

        REQUIRE(caught_count == 13);
    }

    SECTION("Event Data gathering works (single tag)")
    {

        enum tags
        {
            tag1
        };
        engine my_engine(1);

        molecule mol1(
            {{{{0.0, 0.0}, 1., 0.05}}},
            {0.20, 0.5},
            {1, 0});
        molecule mol2(
            {{{{0.0, 0.0}, 1., 0.05}}},
            {0.80, 0.5},
            {-1, 0});

        size_t id1 = my_engine.add(mol1);
        size_t id2 = my_engine.add(mol2);

        molecule mol3(
            {{{{0.0, 0.0}, 1., 0.05}}},
            {0.20, 0.2},
            {1, 0});
        molecule mol4(
            {{{{0.0, 0.0}, 1., 0.05}}},
            {0.80, 0.2},
            {-1, 0});

        size_t id3 = my_engine.add(mol3);
        size_t id4 = my_engine.add(mol4);

        my_engine.tag(id3, tag1);

        int caught_count = 0;
        bool caught1 = false;

        my_engine.on<events::molecule>([&](const report<events::molecule> my_report) {
            caught_count += 1;
        });

        my_engine.on<events::molecule>(tag1, [&](const report<events::molecule> my_report) {
            caught1 = true;
        });

        my_engine.run(0.3);

        REQUIRE(caught_count == 2);
        REQUIRE(caught1);
    }

    SECTION("Event Data gathering works (double tag)")
    {
        enum tags
        {
            tag2,
            tag3
        };
        engine my_engine(1);
        my_engine.elasticity(2.0);

        molecule mol1(
            {{{{0.0, 0.0}, 1., 0.05}}},
            {0.20, 0.5},
            {1, 0});
        molecule mol2(
            {{{{0.0, 0.0}, 1., 0.05}}},
            {0.80, 0.5},
            {-1, 0});

        size_t id1 = my_engine.add(mol1);
        size_t id2 = my_engine.add(mol2);

        molecule mol5(
            {{{{0.0, 0.0}, 1., 0.05}}},
            {0.20, 0.8},
            {1, 0});
        molecule mol6(
            {{{{0.0, 0.0}, 1., 0.05}}},
            {0.80, 0.8},
            {-1, 0});

        size_t id5 = my_engine.add(mol5);
        size_t id6 = my_engine.add(mol6);

        my_engine.tag(id5, tag2);
        my_engine.tag(id6, tag3);

        int caught_count = 0;
        bool caught2 = false;

        my_engine.on<events::molecule>([&](const report<events::molecule> my_report) {
            caught_count += 1;
        });

        my_engine.on<events::molecule>(tag2, tag3, [&](const report<events::molecule> my_report) {
            caught2 = true;
        });

        my_engine.run(0.3);

        REQUIRE(caught_count == 2);
        REQUIRE(caught2);
    }

    SECTION("Event Data gathering works (bumper case)")
    {
        engine my_engine(1);

        molecule mol1(
            {{{{0.0, 0.0}, 1., 0.05}}},
            {0.20, 0.5},
            {1, 0});

        bumper bum({0.80, 0.5}, 0.05);

        size_t id = my_engine.add(mol1);
        my_engine.add(bum);

        int caught_count = 0;

        my_engine.on<events::bumper>([&](const report<events::bumper> my_report) {
            caught_count += 1;
        });

        my_engine.run(0.8);

        REQUIRE(caught_count == 1);
    }

    SECTION("Event Data gathering works (bumper case tag)")
    {
        enum tags{tag1};
        engine my_engine(1);

        molecule mol1(
            {{{{0.0, 0.0}, 1., 0.05}}},
            {0.20, 0.5},
            {1, 0});

        molecule mol2(
            {{{{0.0, 0.0}, 1., 0.05}}},
            {0.20, 0.8},
            {1, 0});

        bumper bum({0.80, 0.5}, 0.05);
        bumper bum2({0.80, 0.8}, 0.05);

        my_engine.add(mol1);
        my_engine.add(bum);
        size_t id = my_engine.add(mol2);
        my_engine.add(bum2);

        my_engine.tag(id, tag1);

        int caught_count = 0;
        bool caught = false;

        my_engine.on<events::bumper>([&](const report<events::bumper> my_report) {
            caught_count += 1;
        });
        my_engine.on<events::bumper>(tag1, [&](const report<events::bumper> my_report) {
            caught = true;
        });

        my_engine.run(0.8);

        REQUIRE(caught_count == 2);
        REQUIRE(caught);
    }

    SECTION("Each method works (no tag)")
    {
    }

    SECTION("Each method works (with tag)")
    {

    }
}