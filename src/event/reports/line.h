// Forward declarations

template <typename> class report;

#if !defined(__forward__) && !defined(__nobb__event__reports__line__h)
#define __nobb__event__reports__line__h

// Libraries

#include <stddef.h>

// Includes

#include "geometry/vec.h"

// Forward includes

#define __forward__
#include "event/events/line.h"
#undef __forward__

template <> class report <events :: xline>
{
  // Public nested classes

public:

  class velocity
  {
    // Members

    const events :: xline & _event;

  public:

    // Constructors

    velocity(const events :: xline &);

    // Getters

    const vec & before() const;
    const vec & after() const;
          vec delta() const;

  } velocity;

  class momentum
  {
    // Members

    const events :: xline & _event;

  public:

    // Constructors

    momentum(const events :: xline &);

    // Getters

    const vec & before() const;
          vec after() const;
          vec delta() const;

  } momentum;

  class angular_velocity
  {
    // Members

    const events :: xline & _event;

  public:

    // Constructors

    angular_velocity(const events :: xline &);

    // Getters

    const double & before() const;
    const double & after() const;
          double delta() const;

  } angular_velocity;

  class angular_momentum
  {
    // Members

    const events :: xline & _event;

  public:

    // Constructors

    angular_momentum(const events :: xline &);

    // Getters

    const double & before () const;
          double after() const;
          double delta() const;

  } angular_momentum;

  class energy
  {
    // Members

    const events :: xline & _event;

  public:

    // Constructors

    energy(const events :: xline &);

    // Getters

    double before() const;
    double after() const;
    double delta() const;

  } energy;

  class xline
  {
    // Members

    const events :: xline & _event;

  public:

    // Constructors

    xline(const events :: xline &);

    // Getters

    const double & xposition() const;
    const double & temperature() const;
    const bool & multiplicative() const;
    const bool & randomness() const;

  } xline;

private:

  // Members

  const events :: xline & _event;

public:

  // Constructors

  report(const events :: xline &);

  // Getters

  const size_t & id() const;
  const size_t & atom() const;
  const vec & position() const;
  const double & orientation() const;
  const double & mass() const;
  const double & time() const;
  const double & module() const;
};

#endif
