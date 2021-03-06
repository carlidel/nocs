#include "engine.hpp"
#include "event/events/molecule.h"
#include "event/events/bumper.h"
#include "event/events/line.h"
#include "event/events/grid.h"

// tag

// Constructors

engine :: tag :: tag() : _id(autoincrement++), _references(0)
{
  memset(this->_tags, '\0', tags);
}

// Destructor

engine :: ~engine()
{
  delete [] this->_tags;
}

// Getters

const size_t & engine :: tag :: id() const
{
  return this->_id;
}

size_t engine :: tag :: size() const
{
  for(size_t i = 0; i < tags; i++)
    if(!(this->_tags[i]))
      return i;

  return tags;
}

const size_t & engine :: tag :: references() const
{
  return this->_references;
}

// Private methods

void engine :: tag :: add(const uint8_t & tag)
{
  for(size_t i = 0; i < tags; i++)
    if(!(this->_tags[i]))
    {
      this->_tags[i] = tag + 1;
      return;
    }
}

void engine :: tag :: remove(const uint8_t & tag)
{
  bool shift = false;

  for(size_t i = 0; i < tags; i++)
  {
    if(this->_tags[i] == tag + 1)
    {
      this->_tags[i] = 0;
      shift = true;
    }

    if(shift && i < tags - 1)
      this->_tags[i] = this->_tags[i + 1];
  }
}

// Operators

uint8_t engine :: tag :: operator [] (const size_t & index) const
{
  return this->_tags[index] - 1;
}

// Private operators

void engine :: tag :: operator ++ ()
{
  this->_references++;
}

void engine :: tag :: operator ++ (int)
{
  ++(*this);
}

void engine :: tag :: operator -- ()
{
  this->_references--;
}

void engine :: tag :: operator -- (int)
{
  --(*this);
}

// Private static members

size_t engine :: tag :: autoincrement = 1;

// engine

// Constructors

engine :: engine(const size_t & fineness) : _grid(fineness), _tags(new hashtable <size_t, molecule *> [256]), _time(0), reset(*this)
{
  this->_elasticity.all = 1.;

  for(size_t i = 0; i < 255; i++)
  {
    this->_elasticity.stag[i] = -1;

    for(size_t j = 0; j < 255; j++)
      this->_elasticity.dtag[i][j] = -1;
  }
}

// Getters

const size_t & engine :: fineness() const
{
  return this->_grid.fineness();
}

const size_t & engine :: event_heap_size() const
{
  return this->_events.size();
}

// Setters

void engine :: elasticity(const double & elasticity)
{
  assert(elasticity > 0);
  this->_elasticity.all = elasticity;

  this->_molecules.each([&](molecule * molecule)
  {
    this->refresh(*molecule);
  });
}

void engine :: elasticity(const uint8_t & tag, const double & elasticity)
{
  assert(elasticity > 0);
  this->_elasticity.stag[tag] = elasticity;

  this->_molecules.each([&](molecule * molecule)
  {
    this->refresh(*molecule);
  });
}

void engine :: elasticity(const uint8_t & alpha, const uint8_t & beta, const double & elasticity)
{
  assert(elasticity > 0);
  this->_elasticity.dtag[alpha][beta] = elasticity;
  this->_elasticity.dtag[beta][alpha] = elasticity;

  this->_molecules.each([&](molecule * molecule)
  {
    this->refresh(*molecule);
  });
}

// Methods

size_t engine :: add(const molecule & molecule)
{
  class molecule * entry = new class molecule(molecule);
  
  entry->set_time(this->_time);
  
  this->_molecules.add(entry->tag.id(), entry);

  this->_grid.add(*entry);
  this->refresh(*entry);

  return entry->tag.id();
}

void engine :: add(const bumper & bumper)
{
  class bumper * entry = new class bumper(bumper);
  this->_bumpers.add(entry);

  this->_grid.add(*entry);

  for(ssize_t dx = -1; dx <= 1; dx++)
    for(ssize_t dy = -1; dy <= 1; dy++)
    {
      ssize_t x = (bumper.mark.x() + this->_grid.fineness() + dx) % this->_grid.fineness();
      ssize_t y = (bumper.mark.y() + this->_grid.fineness() + dy) % this->_grid.fineness();

      this->_grid.each <class molecule> (x, y, [&](class molecule & molecule)
      {
        this->refresh(molecule);
      });
    }
}

void engine :: add(const xline & xline)
{
  class xline * entry = new class xline(xline);
  this->_xlines.add(entry);

  this->_grid.add(*entry);

  for(ssize_t dx = -1; dx <= 1; dx++)
    for(size_t y = 0; y < this->_grid.fineness(); y++) // xlines stay in y = 0 sectors
    {
      ssize_t x = (xline.mark.x() + this->_grid.fineness() + dx) % this->_grid.fineness();

      this->_grid.each <class molecule> (x, y, [&](class molecule & molecule)
      {
        this->refresh(molecule);
      });
    }
}


void engine :: remove(const size_t & id)
{
  molecule * entry = this->_molecules[id];
  this->_grid.remove(*entry);
  entry->disable();

  this->_molecules.remove(id);
  this->_garbage.add(entry);
}

void engine :: tag(const size_t & id, const uint8_t & tag)
{
  molecule * entry = this->_molecules[id];
  entry->tag.add(tag);
  this->_tags[tag].add(id, entry);
}

void engine :: untag(const size_t & id, const uint8_t & tag)
{
  molecule * entry = this->_molecules[id];
  entry->tag.remove(tag);
  this->_tags[tag].remove(id);
}

void engine :: run(const double & time)
{
  begin = std::chrono::steady_clock::now();
  end = std::chrono::steady_clock::now();
  mid = std::chrono::steady_clock::now();
  double ETA;
  double starting_time = this->_time;
  unsigned int mins, hours, secs;

  while(this->_events.size() && ((const event *) (this->_events.peek()))->time() <= time)
  {
    if (std::chrono::duration_cast<std::chrono::seconds>(end - mid).count() > 10)
    {
      mid = std::chrono::steady_clock::now();
      ETA = std::chrono::duration_cast<std::chrono::seconds>(mid - begin).count() * (time - ((const event *)(this->_events.peek()))->time()) / (((const event *)(this->_events.peek()))->time() - starting_time);
      secs = fmod(ETA, 60);
      mins = int(ETA / 60) % 60;
      hours = mins / 60;
      std::cout << "(" << starting_time << " -> " << ((const event *)(this->_events.peek()))->time() << " -> " << time << ") "
                << "ETA: " << hours << "h" << mins << "m" << secs << "s" << std::endl;
    }

    event * event = this->_events.pop();
    event->each(this, &engine :: decref);

    if(event->resolve())
    {
      event->each(this, &engine :: refresh);
      event->callback(this->_dispatcher);
    }

    delete event;
    end = std::chrono::steady_clock::now();
  }

  if(time > this->_time)
    this->_time = time;

  this->_molecules.each([&](molecule *molecule) {
    molecule->integrate(this->_time);
    this->check_position(*molecule);
  });

  this->collect();
}

// Private methods

double engine :: elasticity(const molecule & alpha, const molecule & beta)
{
  if(alpha.tag.size() == 1 && beta.tag.size() == 1 && this->_elasticity.dtag[alpha.tag[0]][beta.tag[0]] > 0)
    return this->_elasticity.dtag[alpha.tag[0]][beta.tag[0]];
  else if(alpha.tag.size() == 1 && beta.tag.size() == 0 && this->_elasticity.stag[alpha.tag[0]] > 0)
    return this->_elasticity.stag[alpha.tag[0]];
  else if(alpha.tag.size() == 0 && beta.tag.size() == 1 && this->_elasticity.stag[beta.tag[0]] > 0)
    return this->_elasticity.stag[beta.tag[0]];
  else
    return this->_elasticity.all;
}

void engine :: check_position(molecule & molecule)
{
  // Is the particle in the correct location? If not, fix it!
  double step = 1. / this->_grid.fineness();
  if (!(molecule.position().x >= step * molecule.mark.x() && molecule.position().y >= step * molecule.mark.y() && molecule.position().x <= step * (molecule.mark.x() + 1) && molecule.position().y <= step * (molecule.mark.y() + 1)))
  {
    this->_grid.update(molecule, vec ::direct);
  }
}

void engine :: refresh(molecule & molecule, const size_t & skip)
{
  check_position(molecule);
  // Grid event

  events :: grid * event = new events :: grid(molecule, this->_grid);

  if (isnan(event->time()) && event->happens())
  {
    std::cout << "MOLECULE NAN!!" << std::endl;
    exit(0);
  }

  if(event->happens())
  {
    event->each(this, &engine :: incref);
    this->_events.push(event :: wrapper(event));
  }
  else
    delete event;

  for(ssize_t dx = -1; dx <= 1; dx++)
  {
    ssize_t x = molecule.mark.x() + dx;

    int fold = vec :: direct;

    if(x < 0)
      fold |= vec :: right;
    else if(x >= static_cast<ssize_t>(this->_grid.fineness()))
      fold |= vec :: left;

    x = (x + this->_grid.fineness()) % this->_grid.fineness();

    // Bumper event

    this->_grid.each <xline> (x, 0, [&](xline & xline)
    {
      events :: xline * event = new events :: xline(molecule, fold, xline);

      if (isnan(event->time()) && event->happens())
      {
        std::cout << "XLINE NAN!" << std::endl;
        exit(0);
      }
      if(event->happens())
      {
        event->each(this, &engine :: incref);
        this->_events.push(event);
      }
      else
        delete event;
    });
  }

  for(ssize_t dx = -1; dx <= 1; dx++)
    for(ssize_t dy = -1; dy <= 1; dy++)
    {
      ssize_t x = molecule.mark.x() + dx;
      ssize_t y = molecule.mark.y() + dy;

      int fold = vec :: direct;

      if(x < 0)
        fold |= vec :: right;
      else if(x >= static_cast<ssize_t>(this->_grid.fineness()))
        fold |= vec :: left;

      if(y < 0)
        fold |= vec :: up;
      else if(y >= static_cast<ssize_t>(this->_grid.fineness()))
        fold |= vec :: down;

      x = (x + this->_grid.fineness()) % this->_grid.fineness();
      y = (y + this->_grid.fineness()) % this->_grid.fineness();

      // Molecule event

      this->_grid.each <class molecule> (x, y, [&](class molecule & beta)
      {
        if(beta.tag.id() == molecule.tag.id() || beta.tag.id() == skip)
          return;

        events :: molecule * event = new events :: molecule(molecule, fold, beta, this->elasticity(molecule, beta));

        if (isnan(event->time()) && event->happens())
        {
          std::cout << "MOLECULE NAN!" << std::endl;
          exit(0);
        }
        if(event->happens())
        {
          event->each(this, &engine :: incref);
          this->_events.push(event);
        }
        else
          delete event;
      });

      // Bumper event

      this->_grid.each <bumper> (x, y, [&](bumper & bumper)
      {
        events :: bumper * event = new events :: bumper(molecule, fold, bumper);

        if(event->happens())
        {
          event->each(this, &engine :: incref);
          this->_events.push(event);
        }
        else
          delete event;
      });
    }
}

void engine :: incref(molecule & molecule, const size_t &)
{
  molecule.tag++;
}

void engine :: decref(molecule & molecule, const size_t &)
{
  molecule.tag--;
}

void engine :: collect()
{
  set <molecule *> old;

  this->_garbage.each([&](molecule * entry)
  {
    if(!(entry->tag.references()))
      old.add(entry);
  });

  old.each([&](molecule * entry)
  {
    this->_garbage.remove(entry);
    delete entry;
  });
}
