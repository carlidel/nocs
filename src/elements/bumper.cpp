#include "bumper.h"

// Constructors

bumper :: bumper()
{
}

bumper :: bumper(const vec & position, const double & radius, const double & temperature, const bool & multiplicative, const bool & randomness, std :: default_random_engine * random_engine) : _position(position), _radius(radius), _temperature(temperature), _multiplicative(multiplicative), _randomness(randomness), _random_engine(random_engine)
{
  assert(~(this->_randomness && this->_multiplicative) && "A bumper can't be random and multiplicative at the same time!");
  assert((this->_temperature == -1.0 || this->_temperature >= 0) && "Temperature not valid!");
  assert(~(this->_randomness && this->_temperature != -1.0) && "Temperature not valid for random bumper!");
  assert(~(this->_multiplicative && this->_temperature != -1.0) && "Temperature not valid for multiplicative bumper!");
  assert(~(this->_randomness && this->_random_engine == NULL) && "pointer to std::default_random_engine unspecified for random bumper");

  if (this->_temperature != -1.0 && this->_randomness)
  {
    this->_exp_distribution = std :: exponential_distribution <double> (this->_temperature);
  }
}

// Getters

const vec & bumper :: position() const
{
  return this->_position;
}

const double & bumper :: radius() const
{
  return this->_radius;
}

const double & bumper :: temperature() const
{
  return this->_temperature;
}

const bool & bumper :: multiplicative() const
{
  return this->_multiplicative;
}

const bool & bumper :: randomness() const
{
  return this->_randomness;
}

// Public Methods

double bumper :: random_extraction()
{
  assert(this->_randomness && "Bumper is NOT random!");
  return this->_exp_distribution(*(this->_random_engine));
}
