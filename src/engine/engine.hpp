#ifndef __nobb__engine__engine__hpp
#define __nobb__engine__engine__hpp

#include "engine.h"
#include "molecule/molecule.h"

// Methods

template <typename type, typename lambda, typename std :: enable_if <std :: is_same <type, molecule> :: value> :: type *> void engine :: each(const lambda & callback) const
{
  this->_molecules.each([&](molecule * molecule)
  {
    molecule->integrate(this->_time);
    callback((const class molecule &) (*molecule));
  });
}

template <typename type, typename lambda, typename std :: enable_if <std :: is_same <type, molecule> :: value> :: type *> void engine :: each(const uint8_t & tag, const lambda & callback) const
{
  this->_tags[tag].each([&](molecule * molecule)
  {
    molecule->integrate(this->_time);
    callback((const class molecule &) (*molecule));
  });
}

#endif