#include "atom.h"

// atom

// Constructors

atom :: atom(const vec & position, const double & mass, const double & radius) : _position(position), _mass(mass), _radius(radius)
{
}

// Private constructors

atom :: atom()
{
}

// Getters

const vec & atom :: position() const
{
	return this->_position;
}

const double & atom :: radius() const
{
	return this->_radius;
}

const double & atom :: mass() const
{
	return this->_mass;
}

// Setters

atom & atom :: position(const vec & position)
{
	this->_position = position;
    return *this;
}