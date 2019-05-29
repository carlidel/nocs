#ifdef __main__

#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include <cmath>

#include "engine/engine.hpp"
#include "graphics/window.h"

int main()
{
  enum tags {fatty, ninja};

  graphics::window my_window;
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(-3.0, 3.0);

  engine my_engine(6);

  for(double x = 0.1; x <= 0.9; x += 0.1)
    for(double y = 0.1; y <= 0.4; y += 0.1)
    {
      molecule my_molecule
      (
        {
          {{0., 0.}, 1., 0.005},
          {{0., 0.02}, 10., 0.015}
        },
        {x, y},
        {0, 0},
        0.,
        M_PI / 4.
      );

      size_t my_molecule_id = my_engine.add(my_molecule);
      my_engine.tag(my_molecule_id, fatty);
    }

  for(double x = 0.1; x <= 0.9; x += 0.1)
    for(double y = 0.6; y <= 0.9; y += 0.1)
    {
      molecule my_other_molecule(
          {{{0., 0.}, 0.01, 0.02}},
          {x, y},
          {distribution(generator), distribution(generator)},
          0.,
          0.);

      size_t my_other_molecule_id = my_engine.add(my_other_molecule);
      my_engine.tag(my_other_molecule_id, ninja);
    }

  bumper my_bumper({0.5, 0.5}, 0.03);
  my_engine.add(my_bumper);

  my_engine.elasticity(fatty, fatty, 0.8);

  my_window.draw(my_engine);
  std :: cout << "Waiting for a click..." << std :: endl;
  my_window.wait_click();

  my_engine.on <events :: molecule> (fatty, ninja, [&](const report <events :: molecule> my_report)
  {
    std :: cout << "There has been a collision between " << my_report.alpha.id() << " and " << my_report.beta.id() << std :: endl;
    std :: cout << "Delta energy for alpha: " << my_report.alpha.energy.delta() << std :: endl;
  });

  for(double time = 0.; time < 500.0; time += 0.001)
  {
    my_engine.run(time);

    double fatty_total_energy = 0.;

    my_engine.each <molecule> (fatty, [&](const molecule & current_molecule) 
    {
      fatty_total_energy += current_molecule.energy();
    });

    double ninja_total_energy = 0.;

    if (!fmod(time, 1.0))
    {
      my_engine.each <molecule> (ninja, [&](const molecule & current_molecule)
      {
        ninja_total_energy += current_molecule.energy();
      });
      std ::cout << "Heavy total energy: " << std ::setw(10) << fatty_total_energy << std ::endl
                 << "Light total energy: " << std ::setw(10) << ninja_total_energy << std ::endl;
    }
    my_engine.reset.energy.tag(ninja, 1.);

    my_window.draw(my_engine);
  }
  my_window.close_window();
  return 0;
}

#endif
