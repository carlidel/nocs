// Foward declarations

class xline;

#if !defined(__forward__) && !defined (__nobb__elements__xline__h)
#define __nobb__elements__bumper__h

// Libraries

#include <cmath>
#include <assert.h>
#include <random>

// Includes

#include "geometry/vec.h"
#include "engine/grid.h"

class xline
{
    // Members

    double _xposition;
    double _temperature;
    bool _multiplicative;
    bool _randomness;
    std :: exponential_distribution <double> _exp_distribution;
    std :: default_random_engine * _random_engine;

public:

    // Constructors

    xline();
    xline(const double &, const double & = -1.0, const bool & = false, const bool & = false, std :: default_random_engine * = NULL);

    // Public members

    //TODO::Deal with the grid
    grid :: mark mark;

    // Getters

    const double & xposition() const;
    const double & temperature() const;
    const bool & multiplicative() const;
    const bool & randomness() const;

    // Methods
    
    double random_extraction();
};

#endif