#ifdef __main__

#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>

#include "engine/engine.hpp"
#include "graphics/window.h"

int main()
{
  const double radius = 0.02;

  graphics::window my_window;
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(-3.0, 3.0);

  engine my_engine(1);

  molecule N = molecule(
      {{{0., 0.}, 1., radius},
       {{0., 2 * radius}, 1., radius},
       {{0., 4 * radius}, 1., radius},
       {{0., 6 * radius}, 1., radius},
       {{2 * radius, 4 * radius}, 1., radius},
       {{4 * radius, 2 * radius}, 1., radius},
       {{6 * radius, 0.}, 1., radius},
       {{6 * radius, 2 * radius}, 1., radius},
       {{6 * radius, 4 * radius}, 1., radius},
       {{6 * radius, 6 * radius}, 1., radius}},
      {0.25, 0.5},
      {distribution(generator), distribution(generator)},
      0.,
      distribution(generator));

  molecule O = molecule(
      {{{0 * radius, 0 * radius}, 1., radius},
       {{1 * radius, 2 * radius}, 1., radius},
       {{1 * radius, 4 * radius}, 1., radius},
       {{0 * radius, 6 * radius}, 1., radius},
       {{-1 * radius, 2 * radius}, 1., radius},
       {{-1 * radius, 4 * radius}, 1., radius}},
      {0.45, 0.5},
      {distribution(generator), distribution(generator)},
      0.,
      distribution(generator));

  molecule C = molecule(
      {{{0., 0.}, 1., radius},
       {{0., 2 * radius}, 1., radius},
       {{0., 4 * radius}, 1., radius},
       {{2 * radius, 0.}, 1., radius},
       {{4 * radius, 0.}, 1., radius},
       {{2 * radius, 4 * radius}, 1., radius},
       {{4 * radius, 4 * radius}, 1., radius}},
      {0.6, 0.5},
      {distribution(generator), distribution(generator)},
      0.,
      distribution(generator));

  molecule S = molecule(
      {{{0., 0.}, 1., radius},
       {{2 * radius, 0.}, 1., radius},
       {{4 * radius, 1 * radius}, 1., radius},
       {{2 * radius, 2 * radius}, 1., radius},
       {{0 * radius, 3 * radius}, 1., radius},
       {{2 * radius, 4 * radius}, 1., radius},
       {{4 * radius, 4 * radius}, 1., radius}},
      {0.8, 0.5},
      {distribution(generator), distribution(generator)},
      0.,
      distribution(generator));

  my_engine.add(N);
  my_engine.add(O);
  my_engine.add(C);
  my_engine.add(S);

  for (double x = 0; x < 1; x += radius * 2)
  {
    bumper xbumper = bumper({0., x}, radius);
    bumper ybumper = bumper({x, 0}, radius);
    my_engine.add(xbumper);
    my_engine.add(ybumper);
  }

  my_window.draw(my_engine);
  std :: cout << "WELCOME TO NOCS!!!\nPlease enjoy this despicable low-quality title!" << std :: endl;
  std :: cout << "Waiting for a click..." << std :: endl;
  my_window.wait_click();

  for (double time = 0.; time < 500; time += 0.00002)
  {
    my_engine.run(time);
    my_window.draw(my_engine);
  }
  my_window.close_window();
  return 0;
}

#endif
