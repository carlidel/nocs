#include "line.hpp"
#include "engine/engine.h"

namespace events
{
    // Constructors

  xline :: xline (:: molecule & molecule, const int & fold, :: xline & xline)
  {
    // Working variables
    double beg, end, delta;
    // Simple case first (one atom in molecule)
    if(molecule.size() == 1)
    {
      vec xa = molecule.position() + vec(fold);
      double xl = xline.xposition();

      vec v = molecule.velocity();

      delta = xa.x - xl;
      if (std :: signbit(delta) != std :: signbit(v.x))
      {
        double abs_delta = abs(delta) - molecule.radius();
        this->_happens = true;
        this->_time = molecule.time() + abs(abs_delta / v.x);
        this->_molecule.atom = 0;
        this->_molecule.molecule = &molecule;
        this->_molecule.version = molecule.version();
        this->_xline = &xline;
        this->_fold = fold;

        return;
      }
      else
      {
        this->_happens = false;
        return;
      }
    }
    else 
    // Then difficult case
    {
      vec xa = molecule.position() + vec(fold);
      double xl = xline.xposition();

      vec v = molecule.velocity();

      delta = xa.x - xl;
      
      if(delta <= molecule.radius())
      {
        // They are already close
        beg = molecule.time();
        if(std :: signbit(delta) != std :: signbit(v.x))
        {
          // They are getting closer and closer...
          // ...until the cdm itself will touch the line!
          end = beg + abs(delta / v.x);
        }
        else
        {
          // They are getting far away, until the surrounding circle
          // does not touch xline anymore
          double circle_delta = molecule.radius() - delta;
          end = beg + abs(circle_delta / v.x);
        }
      }
      else if(std :: signbit(delta) != std :: signbit(v.x))
      {
        // They are not already close
        // But they are getting closer and closer
        beg = molecule.time() + abs((delta - molecule.radius()) / v.x);
        end = molecule.time() + abs(delta / v.x);
      }
      else
      {
        this->_happens = false;
        return;
      }
    }
    // Now let's deal with the difficult case!

    double step = 0.5 * M_PI / fabs(2. * molecule.angular_velocity()); // TODO: find if this cropping can be used also for this kind of event.

    // Look for collisions between atoms and xline

    this->_time = std :: numeric_limits <double> :: infinity();

    for(double binbeg = beg; binbeg < end; binbeg += step)
    {
      double binend = std :: min(binbeg + step, end);

      for(size_t i = 0; i < molecule.size(); i++)
      {
        double ctime = collision(molecule, i, xline, binbeg, binend, fold);

        if(!std :: isnan(ctime) && ctime < this->_time)
        {
          this->_time = ctime;
          this->_molecule.atom = i;
        }
      }

      if(this->_time < std :: numeric_limits <double> :: infinity())
      {
        this->_happens = true;
        this->_molecule.molecule = &molecule;
        this->_molecule.version = molecule.version();
        this->_xline = &xline;
        this->_fold = fold;

        return;
      }
    }

    this->_happens = false;
    
  }

  // Geters

  const :: molecule & xline :: molecule() const
  {
    return *(this->_molecule.molecule);
  }

  // Methods

  bool xline :: current()
  {
    return static_cast<int32_t>(this->_molecule.version) == this->_molecule.molecule->version();
  }

  bool xline :: resolve()
  {
    // Check version
    if(!current())
      return false;
    // Integrate to collision

    this->_molecule.molecule->integrate(this->_time);

    // Update version

    (*(this->_molecule.molecule))++;

    // Collision resolution

    // Simple case (1 atom molecule)
    if(this->_molecule.molecule->size() == 1 && this->_xline->x_only())
    {
      bool sign = std :: signbit(this->_molecule.molecule->velocity().x);
        if (this->_xline->temperature() != -1)
      {  
        if (this->_xline->randomness())
        {
          this->_molecule.molecule->velocity_manual_change(vec(this->_xline->random_extraction() * (sign ? 1 : -1),
           this->_molecule.molecule->velocity().y));
        }
        else
        {
          if (this->_xline->multiplicative())
          {
            // NOW HERE WE ARE DEALING WITH THIS "TEMPERATURE" WHICH IS ACTUALLY A DIRTY ELASTICITY CONSTANT.
            this->_molecule.molecule->velocity_manual_change(vec(this->_molecule.molecule->velocity().x * this->_xline->temperature() * (-1), this->_molecule.molecule->velocity().y));
          }
          else
          {
            this->_molecule.molecule->velocity_manual_change(vec(this->_xline->temperature() * (sign ? 1 : -1), this->_molecule.molecule->velocity().y));
          }
        }
      }
      return true;
    }
    else // Difficult case
    {
      vec a = position(*(this->_molecule.molecule), this->_molecule.atom, this->_fold);
      vec b = vec(this->_xline->xposition(), a.y);

      vec n = (a - b).normalize(); // Versor of the impulse from xline to molecule

      // Everything from here should be just as equal!
      double m = this->_molecule.molecule->mass();
      double i = this->_molecule.molecule->inertia_moment();
      this->av = this->_molecule.molecule->angular_velocity();
      this->l = i * this->av;
      this->v = this->_molecule.molecule->velocity();
      this->p = this->v * m;

      this->r = (*(this->_molecule.molecule))[this->_molecule.atom].position() % this->_molecule.molecule->orientation() + (*(this->_molecule.molecule))[this->_molecule.atom].radius() * (-n);

      this->module = (2 * v * n + 2 * av * (r ^ n)) / -(1 / m + ((r ^ n) * (r ^ n)) / i); // TODO: n-ple check this equation when developing tests, but it should be right.

      // Update molecule velocity and angular_velocity

      this->_molecule.molecule->impulse(r, module * n);  
    }
    
    // TODO: develop a meaningful system to define a xline's temperature and the thermical exchange in a collision.
    if (this->_xline->temperature() != -1)
    {
      if (this->_xline->randomness())
      {
        this->_molecule.molecule->scale_energy(this->_xline->random_extraction());
      }
      else
      {
        if (this->_xline->multiplicative())
        {
          // NOW HERE WE ARE DEALING WITH THIS "TEMPERATURE" WHICH IS ACTUALLY A DIRTY ELASTICITY CONSTANT.
          this->_molecule.molecule->scale_energy(
            this->_molecule.molecule->energy() * this->_xline->temperature());
        }
        else
        {
          this->_molecule.molecule->scale_energy(this->_xline->temperature());
        }
      }
    }
    return true;
  }

  void xline :: each(engine * engine, void (engine :: * callback)(:: molecule &, const size_t &))
  {
    (engine->*callback)(*(this->_molecule.molecule), 0);
  }

  void xline :: callback(dispatcher & dispatcher)
  {
    dispatcher.trigger(*this);
  }

  // Private Methods

  double xline :: collision(const :: molecule & molecule, const size_t & index, const :: xline & xline, const double & beg, const double & end, const int & fold)
  {
    double radiisquared = (molecule[index].radius()) * (molecule[index].radius());

    auto distsquared = [&](const double & time)
    {
      vec pos = position(molecule, index, time, fold); 
      return ~(pos - vec(xline.xposition(), pos.y)) - radiisquared;
    };

    double binmin = gss :: min(distsquared, beg, end);

    if(distsquared(binmin) > 0)
      return NAN;

    double binmax = gss :: max(distsquared, beg, binmin);
    double zero = secant :: compute(distsquared, binmax, binmin);

    return (distsquared(zero + time_epsilon) < distsquared(zero)) ? zero : NAN;
  }
}
