// Foward declarations

class molecule;

#if !defined(__forward__) && !defined(__nobb__molecule__molecule__h)
#define __nobb__molecule__molecule__h

// Libraries

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm> // allow min in MSVC
#include <assert.h>
#include <type_traits>

// Includes

#include "geometry/vec.h"
#include "molecule/atom.h"
#include "engine/grid.h"
#include "engine/engine.h"

class molecule
{

public:

  // Members

  size_t _size;
  atom * _atoms;

  vec _position;
  vec _velocity;

  double _orientation;
  double _angular_velocity;

  double _mass;
  double _radius;
  double _inertia_moment;

  double _time;
  int32_t _version;

public:

  // Public members

  grid :: mark mark;
  class engine :: tag tag;

  // Constructors

  molecule();
  molecule(const std :: vector<atom> &, const vec & = vec(0, 0), const vec & = vec(0, 0), const double & = 0, const double & = 0);
  molecule(const molecule &);

  // Destructor

  ~molecule();

  // Getters

  const size_t & size() const;
  const vec & position() const;
  const vec & velocity() const;
  const double & orientation() const;
  const double & angular_velocity() const;
  const double & radius() const;
  const double & mass() const;
  const double & inertia_moment() const;
  const double & time() const;
  const int32_t & version() const;

  double energy() const;

  // Methods

  void set_time(const double &);
  void integrate(const double &);
  void impulse(const vec &, const vec &);
  void teleport(const vec :: fold &);
  void scale_energy(const double &);

  // DESPICABLE METHOD!
  void velocity_manual_change(const vec &);

  void disable();

  // Operators

  const atom & operator [] (const size_t &) const;
  molecule & operator ++ ();
  molecule operator ++ (int);
};

#endif
