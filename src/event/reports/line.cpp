#include "line.h"
#include "event/events/line.h"

// Public nested classes

// velocity

// Constructors

report<events ::xline>::velocity ::velocity(const events ::xline &event) : _event(event)
{
}

// Getters

const vec &report<events ::xline>::velocity ::before() const
{
    return this->_event.v;
}

const vec &report<events ::xline>::velocity ::after() const
{
    return this->_event._molecule.molecule->velocity();
}

vec report<events ::xline>::velocity ::delta() const
{
    return this->after() - this->before();
}

// momentum

// Constructors

report<events ::xline>::momentum ::momentum(const events ::xline &event) : _event(event)
{
}

// Getters

const vec &report<events ::xline>::momentum ::before() const
{
    return this->_event.p;
}

vec report<events ::xline>::momentum ::after() const
{
    return this->_event._molecule.molecule->velocity() * this->_event._molecule.molecule->mass();
}

vec report<events ::xline>::momentum ::delta() const
{
    return this->after() - this->before();
}

// angular_velocity

// Constructors

report<events ::xline>::angular_velocity ::angular_velocity(const events ::xline &event) : _event(event)
{
}

// Getters

const double &report<events ::xline>::angular_velocity ::before() const
{
    return this->_event.av;
}

const double &report<events ::xline>::angular_velocity ::after() const
{
    return this->_event._molecule.molecule->angular_velocity();
}

double report<events ::xline>::angular_velocity ::delta() const
{
    return this->after() - this->before();
}

// angular_momentum

// Constructors

report<events ::xline>::angular_momentum ::angular_momentum(const events ::xline &event) : _event(event)
{
}

// Getters

const double &report<events ::xline>::angular_momentum ::before() const
{
    return this->_event.l;
}

double report<events ::xline>::angular_momentum ::after() const
{
    return this->_event._molecule.molecule->angular_velocity() * this->_event._molecule.molecule->inertia_moment();
}

double report<events ::xline>::angular_momentum ::delta() const
{
    return this->after() - this->before();
}

// energy

// Constructors

report<events ::xline>::energy ::energy(const events ::xline &event) : _event(event)
{
}

// Getters

double report<events ::xline>::energy ::before() const
{
    return 0.5 * (~this->_event.v * this->_event._molecule.molecule->mass() + this->_event.av * this->_event.av * this->_event._molecule.molecule->inertia_moment());
}

double report<events ::xline>::energy ::after() const
{
    return this->_event._molecule.molecule->energy();
}

double report<events ::xline>::energy ::delta() const
{
    return this->after() - this->before();
}

// xline

// Constructors

report<events ::xline>::xline ::xline(const events ::xline &event) : _event(event)
{
}

// Methods

const double &report<events ::xline>::xline ::xposition() const
{
    return this->_event._xline->xposition();
}

const double &report<events ::xline>::xline ::temperature() const
{
    return this->_event._xline->temperature();
}

const bool &report<events ::xline>::xline ::multiplicative() const
{
    return this->_event._xline->multiplicative();
}

const bool &report<events ::xline>::xline ::randomness() const
{
    return this->_event._xline->randomness();
}

// Constructors

report<events ::xline>::report(const events ::xline &event) : velocity(event), momentum(event), angular_velocity(event), angular_momentum(event), energy(event), xline(event), _event(event)
{
}

// Getters

const size_t &report<events ::xline>::id() const
{
    return this->_event._molecule.molecule->tag.id();
}

const size_t &report<events ::xline>::atom() const
{
    return this->_event._molecule.atom;
}

const vec &report<events ::xline>::position() const
{
    return this->_event._molecule.molecule->position();
}

const double &report<events ::xline>::orientation() const
{
    return this->_event._molecule.molecule->orientation();
}

const double &report<events ::xline>::mass() const
{
    return this->_event._molecule.molecule->mass();
}

const double &report<events ::xline>::time() const
{
    return this->_event._time;
}

const double &report<events ::xline>::module() const
{
    return this->_event.module;
}
