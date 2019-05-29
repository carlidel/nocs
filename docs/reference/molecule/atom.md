## Class `atom`

### Overview

Class `atom` is the base component for class `molecule` and represents a single massive sphere in a 2-dimensional space.

### Interface

#### Constructor

 * `atom(const vec & position, const double & radius, const double & mass)`
    
    builds an atom of given mass and radius at the given position.

#### Getters

 * `const vec & position() const`
 * `const vec & radius() const`
 * `const vec & mass() const`

### Private Methods and Interface

#### Setters

 * `atom & position(const vec &)`
   Given a vector, changes the position of the atom. This is used by the friend class `molecule` during a molecule construction, so that it is possible to properly normalize the distribution of a set of atoms.
