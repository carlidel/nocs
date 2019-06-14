#include "catch.hpp"

// Libraries

#include <unistd.h>

// Includes

#include "engine/engine.hpp"
#include "graphics/window.h"

// Tests

TEST_CASE("Graphic tools works correctly", "[graphics]")
{
    SECTION("Window opening and closing")
    {
        engine my_engine(1);
        graphics::window my_window;
        my_window.draw(my_engine);
#ifdef __graphics__
        std::cout << "You should see nothing, just a white window!" << std::endl;
#else
        std::cout << "Since there is no graphic support... you will see nothing!" << std::endl;
#endif
#ifdef __graphics__
        usleep(5e6);
#endif
        my_window.close_window();
    }

    SECTION("Engine drawing (no tag)")
    {
        engine my_engine(4);
        graphics::window my_window;
        molecule mol1(
            {{{{0.0, 0.0}, 1., 0.07}}},
            {0.20, 0.3},
            {1, 0});

        molecule mol2(
            {{{{0.0, 0.0}, 1., 0.07}}},
            {0.40, 0.3},
            {1, 0});

        bumper bum1({0.5, 0.8}, 0.07);
        
        my_engine.add(mol1);
        my_engine.add(mol2);
        my_engine.add(bum1);

        my_window.draw(my_engine);
#ifdef __graphics__
        usleep(1e5);
        std::cout << "You should see a 4x4 grid, one red ball and 2 moving blue balls!" << std::endl;
#else
        std::cout << "Since there is no graphic support... you will see nothing!" << std::endl;
#endif
        for (double i = 0.01; i < 1; i += 0.01)
        {
            my_engine.run(i);
            my_window.draw(my_engine);
#ifdef __graphics__
            usleep(1e5);
#endif
        }
        my_window.close_window();
    }
    SECTION("Engine drawing (with tag)")
    {
        enum tags{my_tag};
        engine my_engine(4);
        graphics::window my_window;

        molecule mol1(
            {{{{0.0, 0.0}, 1., 0.07}}},
            {0.20, 0.3},
            {1, 0});

        molecule mol2(
            {{{{0.0, 0.0}, 1., 0.07}}},
            {0.40, 0.3},
            {1, 0});

        bumper bum1({0.5, 0.8}, 0.7);

        my_engine.add(mol1);
        size_t id = my_engine.add(mol2);
        my_engine.add(bum1);
        my_engine.tag(id, my_tag);

        my_window.draw(my_engine, my_tag);
#ifdef __graphics__
        usleep(1e5);
        std::cout << "You should see a 4x4 grid, and only one moving blue ball!" << std::endl;
#else
        std::cout << "Since there is no graphic support... you will see nothing!" << std::endl;
#endif
        for (double i = 0.01; i < 1; i += 0.01)
        {
            my_engine.run(i);
            my_window.draw(my_engine, my_tag);
#ifdef __graphics__
            usleep(1e5);
#endif
        }
        my_window.close_window();
    }
}