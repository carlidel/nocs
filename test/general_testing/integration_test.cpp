#include "catch.hpp"

// Libraries

#include <type_traits>
#include <limits>
#include <math.h>
#include <iostream>

// Includes

#include "engine/engine.hpp"
#include "graphics/window.h"

// Numeric Tolerance

const double EPSILON = std::numeric_limits<double>::epsilon() * 32;

// Tests

TEST_CASE("Molecule integation works correctly", "[engine] [integration]")
{
    SECTION("molecule-molecule collision (1 atom each)")
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

        my_engine.add(mol1);
        my_engine.add(mol2);

        my_engine.on<events::molecule>([&](const report<events::molecule> my_report) {
            REQUIRE(fabs(my_report.time() - 0.25) < EPSILON);
            REQUIRE(fabs(my_report.alpha.velocity.delta().x - 2.0) < EPSILON);
            REQUIRE(fabs(my_report.beta.velocity.delta().x + 2.0) < EPSILON);
        });

        my_engine.run(0.3);
    }

    SECTION("molecule-bumper collision (1 atom molecule)")
    {
        engine my_engine(1);

        molecule mol1(
            {{{{0.0, 0.0}, 1., 0.05}}},
            {0.20, 0.5},
            {1, 0});

        bumper bum({0.80, 0.5}, 0.05);

        my_engine.add(mol1);
        my_engine.add(bum);

        my_engine.on<events::bumper>([&](const report<events::bumper> my_report){
            REQUIRE(fabs(my_report.time() - 0.5) < EPSILON);
            REQUIRE(fabs(my_report.velocity.delta().x + 2.0) < EPSILON);
        });

        my_engine.run(0.7);
    }
    
    SECTION("molecule-molecule collision (3 atoms each)")
    {
        engine my_engine(1);

        molecule mol1(
            {{{0.0, 0.0}, 1., 0.05},
             {{0.0, 0.1}, 1., 0.05},
             {{0.0, 0.2}, 1., 0.05}},
            {0.20, 0.4},
            {1, 0});
        molecule mol2(
            {{{0.0, 0.0}, 1., 0.05},
             {{0.0, 0.1}, 1., 0.05},
             {{0.0, 0.2}, 1., 0.05}},
            {0.80, 0.6},
            {-1, 0});

        my_engine.add(mol1);
        my_engine.add(mol2);

        my_engine.on<events::molecule>([&](const report<events::molecule> my_report) {
            REQUIRE(fabs(my_report.time() - 0.25) < EPSILON);
            REQUIRE(fabs(my_report.module() + 2.65116279069767) < EPSILON);
            REQUIRE(fabs(my_report.alpha.velocity.delta().x - 0.883720930232558) < EPSILON);
            REQUIRE(fabs(my_report.beta.velocity.delta().x + 0.883720930232558) < EPSILON);
            REQUIRE(fabs(my_report.alpha.angular_velocity.delta() - 11.16279069767441889) < EPSILON);
            REQUIRE(fabs(my_report.beta.angular_velocity.delta() - 11.16279069767441889) < EPSILON);
        });

        my_engine.run(0.3);
    }
    
    SECTION("molecule-bumper collision (3 atoms molecule)")
    {
        engine my_engine(1);

        molecule mol1(
            {{{0.0, 0.0}, 1., 0.05},
             {{0.0, 0.1}, 1., 0.05},
             {{0.0, 0.2}, 1., 0.05}},
            {0.20, 0.4},
            {1, 0});

        bumper bum({0.80, 0.5}, 0.05);

        my_engine.add(mol1);
        my_engine.add(bum);

        my_engine.on<events::bumper>([&](const report<events::bumper> my_report){
            REQUIRE(fabs(my_report.time() - 0.5) < EPSILON);
            REQUIRE(fabs(my_report.module() - 2.65116279069767) < EPSILON);
            REQUIRE(fabs(my_report.velocity.delta().x + 0.883720930232558) < EPSILON);
            REQUIRE(fabs(my_report.angular_velocity.delta() - 11.16279069767441889) < EPSILON);
        });

        my_engine.run(0.55);
    }

    SECTION("Changing elasticity constant works")
    {
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

        size_t subs = my_engine.on<events::molecule>([&](const report<events::molecule> my_report) {
            REQUIRE(fabs(my_report.alpha.velocity.delta().x - 3.0) < EPSILON);
            REQUIRE(fabs(my_report.beta.velocity.delta().x + 3.0) < EPSILON);
        });

        my_engine.run(0.3);

        my_engine.remove(id1);
        my_engine.remove(id2);
        my_engine.unsubscribe<events::molecule>(subs);
        my_engine.elasticity(0.5);

        id1 = my_engine.add(mol1);
        id2 = my_engine.add(mol2);

        subs = my_engine.on<events::molecule>([&](const report<events::molecule> my_report) {
            REQUIRE(fabs(my_report.alpha.velocity.delta().x - 1.5) < EPSILON);
            REQUIRE(fabs(my_report.beta.velocity.delta().x + 1.5) < EPSILON);
        });

        my_engine.run(0.6);
    }

    SECTION("Changing elasticity constant works (with tags)")
    {
        enum tags{tag1, tag2, tag3};
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
        bool caught1 = false;
        bool caught2 = false;

        my_engine.on<events::molecule>([&](const report<events::molecule> my_report) {
            caught_count += 1;
        });

        my_engine.on<events::molecule>(tag1, [&](const report<events::molecule> my_report) {
            caught1 = true;
        });

        my_engine.on<events::molecule>(tag2, tag3, [&](const report<events::molecule> my_report) {
            caught2 = true;
        });

        my_engine.run(0.3);

        REQUIRE(caught_count == 3);
        REQUIRE(caught1);
        REQUIRE(caught2);
    }
}