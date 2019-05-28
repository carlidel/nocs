## Class `bumper` (event/events/bumper.h)

### Overview

Class `bumper` represents the collision of a molecule with a bumper as an object-oriented event.

### Interface

#### Constructor

  * `bumper(:: molecule & molecule, const int & fold, :: bumper & bumper)`

    builds a collision event with the given elements and verifies whether the collision will happen or not. (fold indicates the standard translation to be considered for `molecule`, following the standard given in **vec.md**)

#### Getters

  * `bool happens() const`

    returns whether the event will happen or not.

  * `double time() const`

    returns when the event will happen.

  * `const :: molecule & molecule() const`

    returns the molecule involved in the collision.

#### Public Methods

  * `virtual bool current()`

    returns whether the involved elements are in the correct version or not.

  * `virtual vool resolve()`

    executes the event with its dynamical consequences.

  * `virtual void each(engine * engine, void (engine :: *callback)(molecule &, const size_t &))`

    given the working engine and a method of the engine that takes as argument a `molecule &` and a `const size_t &`, executes the given method to each molecule involved in the event. (In this case, one molecule is involved)

  * `virtual void callback(dispatcher & dispatcher)`

    given the engine's main dispatcher, executes the dispatcher's trigger for the event, which will execute the corrispondent requested reports, if any.

### Private elements and methods

#### Settings

`time_epsilon` sets the epsilon sensitivity to be used in certain parts of the computation.

#### Private methods

* `double collision(const :: molecule & alpha, const size_t & index_alpha, const :: molecule & beta, const size_t & index_beta, const double & beg, const double & end, const int & fold)`

  Given two molecules, indexes of the two atoms under analysis, the time frame under inspection and the eventual translational fold to keep under consideration, checks if the two molecules will collide and returns the collision time if the answer is positive. Returns NaN if it's negative.

#### Static inline methods

* `vec position(const :: molecule & molecule, const size_t & index, const int & fold)`
  
  Inline method for quickly obtain the atom of the given index coordinates in the engine's reference system at its current time.

* `vec position(const :: molecule & molecule, const size_t & index, const double & time, const int & fold)`

  Inline method for quickly obtain the atom of the given index coordinates in the engine's reference system at a given time.