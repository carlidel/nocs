#include "molecule.h"

// molecule

// Constructors

molecule :: molecule()
{
}

molecule :: molecule(const std :: vector<atom> & atoms, const vec & position, const vec & velocity, const double & orientation, const double & angular_velocity) :  _position(position), _velocity(velocity), _orientation(orientation), _angular_velocity(angular_velocity), _mass(0), _radius(0), _inertia_moment(0), _time(0), _version(0)
{
	this->_size = atoms.size();
	this->_atoms = new atom[this->_size];

  vec center_mass(0, 0);
	for(size_t i = 0; i < this->_size; i++)
	{
		this->_atoms[i] = atoms[i];
		this->_mass += this->_atoms[i].mass();

    center_mass += this->_atoms[i].mass() * this->_atoms[i].position();
	}

	center_mass /= this->_mass;

	for(size_t i = 0; i < this->_size; i++)
		this->_atoms[i].position(this->_atoms[i].position() - center_mass);

	for(size_t i = 0; i < this->_size; i++)
    {
      this->_radius = std :: max(this->_radius, !this->_atoms[i].position() + this->_atoms[i].radius());
      this->_inertia_moment += this->_atoms[i].mass() * (.5 * pow(this->_atoms[i].radius(), 2) + ~this->_atoms[i].position()); // Steiner's theorem
    }
}

molecule :: molecule(const molecule & m) : _size(m.size()), _atoms(new atom[m.size()]), _position(m.position()), _velocity(m.velocity()), _orientation(m.orientation()), _angular_velocity(m.angular_velocity()), _mass(m.mass()), _radius(m.radius()), _inertia_moment(m.inertia_moment()), mark(m.mark), tag(m.tag)
{
	for(size_t i = 0; i < this->_size; i++)
		this->_atoms[i] = m[i];
}

molecule :: ~molecule()
{
	delete[] this->_atoms;
}

// Getters

const size_t & molecule :: size() const
{
	return this->_size;
}

const vec & molecule :: position() const
{
	return this->_position;
}

const vec & molecule :: velocity() const
{
	return this->_velocity;
}

const double & molecule :: orientation() const
{
	return this->_orientation;
}

const double & molecule :: angular_velocity() const
{
	return this->_angular_velocity;
}

const double & molecule :: radius() const
{
	return this->_radius;
}

const double & molecule :: mass() const
{
	return this->_mass;
}

const double & molecule :: inertia_moment() const
{
	return this->_inertia_moment;
}

const double & molecule :: time() const
{
	return this->_time;
}

const int32_t & molecule :: version() const
{
	return this->_version;
}

double molecule :: energy() const
{
  return 0.5 * ( (this->_mass * (~this->_velocity)) + (this->_inertia_moment * this->_angular_velocity * this->_angular_velocity) );
}

// Methods

void molecule :: set_time(const double & time)
{
	this->_time = time;
}

void molecule :: integrate(const double & time)
{
	if(this->_time < time)
  {
    this->_position += this->_velocity * (time - this->_time);
    this->_orientation += fmod(this->_angular_velocity * (time - this->_time), 2. * M_PI);

    this->_time = time;
  }
}

void molecule :: impulse(const vec & position, const vec & impulse)
{
  this->_velocity = (this->_mass * this->_velocity + impulse) / this->_mass;
  this->_angular_velocity = (this->_inertia_moment * this->_angular_velocity + (position ^ (impulse))) / this->_inertia_moment;
}

void molecule :: teleport(const vec :: fold & fold)
{
  this->_position += vec(fold);
}

void molecule :: scale_energy(const double & target)
{
  double ratio = sqrt(target / this->energy());

  this->_velocity *= ratio;
  this->_angular_velocity *= ratio;
}

void molecule :: velocity_manual_change(const vec & target)
{
	assert(this->size() == 1 && "No, I will not allow this!");
	this->_velocity = target;
}

void molecule :: disable()
{
  this->_version = -1;
}

// Public Operators

const atom & molecule :: operator [] (const size_t & n) const
{
	return this->_atoms[n];
}

molecule & molecule :: operator ++ ()
{
  this->_version = std :: max(this->_version + 1, 0);
  return *this;
}

molecule molecule :: operator ++ (int)
{
  molecule temp = *this;
  ++*this;
  return temp;
}