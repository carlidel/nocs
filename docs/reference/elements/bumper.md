## Class `bumper`

### Overview

Class `bumper` is an object-oriented representation of a spherical immovable obstacle in a 2-dimensional space.

In order to have different kind of collision physics, it is possible to create bumpers with different "temperatures" or methods of collision.

### Interface

#### Constructors

  * `bumper()`

    builds an uninitialized bumper.

  * `bumper(const vec & position, const double & radius, const double & temperature = -1, const bool & multiplicative = false, const bool & randomness = false, std :: default_random_engine * random_engine = NULL)`

    builds a bumper with the given values and flags. More specifically:
    * For a standard bumper with standard elastic collision, you have to provide only `position` and `radius`;
    * For a bumper that resets the energy of every colliding molecule at a given value of `temperature`, you have to provide only `position`, `radius` and `temperature`.
    * For a bumper that has a different elastic coefficient, you have to provide `position`, `radius`, `temperature` and set `multiplicative` to `true`. This will make the `temperature` value the elastic coefficient of the bumper. (Further development of NOCS may improve the naming standard)
    * For a bumper that resets the energy of every colliding value at a randomly extracted value from an exponential distribution with `temperature` as average value, you have to provide `position`, `radius`, `temperature`, set `multiplicative` to `false`, set `randomness` to `true` and provide a pointer to an `std::default_random_engine`. This way, you have the possibility to fully control the eventual random element in the simulation.

#### Public members

  * `grid :: mark`

    position of the molecule inside the engine grid.

#### Getters

  * `const vec & position() const`
  * `const double & radius() const`
  * `const double & temperature() const`
  * `const bool & multiplicative() const`
    
    Returns if the bumper is multiplicative.

  * `const bool & randomness() const`
    
    Returns if the bumper has random behavior.

#### Methods

  * `double random_extraction()`

    If the bumper is random, performs a random extraction from its exponential distribution.
