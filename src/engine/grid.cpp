#include "grid.hpp"
#include "molecule/molecule.h"
#include "elements/bumper.h"
#include "elements/line.h"

void my_in_assert(bool element)
{
  if (!element)
  {
    std::cout << "GRID ASSERT VIOLATED!" << std::endl;
    exit(EXIT_FAILURE);
  }
}

// Nested classes

// mark

// Getters

size_t grid :: mark :: x() const
{
  return this->_x;
}

size_t grid :: mark :: y() const
{
  return this->_y;
}

// grid

// Constructors

grid :: grid(const size_t & fineness) : _fineness(fineness)
{
  this->_molecules = new set <molecule *> * [this->_fineness];

  for(size_t i = 0; i < this->_fineness; i++)
    this->_molecules[i] = new set <molecule *> [this->_fineness];

  this->_bumpers = new set <bumper *> * [this->_fineness];

  for(size_t i = 0; i < this->_fineness; i++)
    this->_bumpers[i] = new set <bumper *> [this->_fineness];

  this->_xlines = new set <xline *> * [this->_fineness];

  for(size_t i = 0; i < this->_fineness; i++)
    this->_xlines[i] = new set <xline *> [this->_fineness];
}

// Destructor

grid :: ~grid()
{
  for(size_t i = 0; i < this->_fineness; i++)
  {
    delete [] this->_molecules[i];
    delete [] this->_bumpers[i];
    delete [] this->_xlines[i];
  }
  delete [] this->_molecules;
  delete [] this->_bumpers;
  delete [] this->_xlines;
}

// Getters

const size_t & grid :: fineness() const
{
  return this->_fineness;
}

// Methods

void grid :: add(molecule & molecule)
{
  this->add(molecule, (size_t) (molecule.position().x * this->_fineness), (size_t) (molecule.position().y * this->_fineness));
}

void grid :: add(bumper & bumper)
{
  this->add(bumper, (size_t) (bumper.position().x * this->_fineness), (size_t) (bumper.position().y * this->_fineness));
}

void grid :: add(xline & xline)
{
  // xlines by default occupy the grid in the y-zero-sector.
  // from this, everything is consequent in the refresh() method in the engine.
  this->add(xline, (size_t) (xline.xposition() * this->_fineness), (size_t) 0);
}

void grid :: remove(molecule & molecule)
{
  this->_molecules[molecule.mark._x][molecule.mark._y].remove(&molecule);
}

void grid :: update(molecule & molecule, const vec :: fold & fold)
{
  //std::cout << "before: " << molecule.mark.x() << " " << molecule.mark.y() << std::endl;
  bool flag;

  this->remove(molecule);

  vec :: fold teleport = vec :: direct;

  do
  {
    flag = true;
    if (molecule.position().x < 0)
    {
      teleport = vec :: right;
      molecule.teleport(teleport);
      teleport = vec :: direct;
    } else if (molecule.position().x >= 1)
    {
      teleport = vec :: left;
      molecule.teleport(teleport);
      teleport = vec :: direct;
    }
    else
      flag = false;
  } while(flag);

  do
  {
    flag = true;
    if (molecule.position().y < 0)
    {
      teleport = vec :: up;
      molecule.teleport(teleport);
      teleport = vec ::direct;
    } else if (molecule.position().y >= 1)
    {
      teleport = vec :: down;
      molecule.teleport(teleport);
      teleport = vec ::direct;
    }
    else
      flag = false;
  } while(flag);

  if(!((size_t)trunc(molecule.position().x * this->_fineness) < this->_fineness && (size_t)trunc(molecule.position().y * this->_fineness) < this->_fineness))
  {
    std::cout << molecule.position().x << " " << molecule.position().y << std::endl;
    exit(0);
  }

  this->add(molecule, trunc(molecule.position().x * this->_fineness), trunc(molecule.position().y * this->_fineness));

  //std::cout << "after: " << molecule.mark.x() << " " << molecule.mark.y() << std::endl;
}

// Private Methods

void grid :: add(molecule & molecule, const size_t & x, const size_t & y)
{
  molecule.mark._x = x;
  molecule.mark._y = y;

  this->_molecules[molecule.mark._x][molecule.mark._y].add(&molecule);
}

void grid :: add(bumper & bumper, const size_t & x, const size_t & y)
{
  bumper.mark._x = x;
  bumper.mark._y = y;

  this->_bumpers[bumper.mark._x][bumper.mark._y].add(&bumper);
}

void grid :: add(xline & xline, const size_t & x, const size_t & y)
{
  xline.mark._x = x;
  xline.mark._y = y;

  this->_xlines[xline.mark._x][xline.mark._y].add(&xline);
}