#ifndef __nobb__event__events__line__hpp
#define __nobb__event__events__line__hpp

#include "line.h"
#include "math/gss.hpp"
#include "math/secant.hpp"

namespace events
{
  // Static private methods

  inline vec xline :: position(const :: molecule & molecule, const size_t & index, const int & fold)
  {
    return molecule.position() + vec(fold) + (molecule[index].position() % molecule.orientation());
  }

  inline vec xline :: position(const :: molecule & molecule, const size_t & index, const double & time, const int & fold)
  {
    double dt = time - molecule.time();
    return molecule.position() + vec(fold) + molecule.velocity() * dt + molecule[index].position() % (molecule.orientation() + molecule.angular_velocity() * dt);
  }
}

#endif
