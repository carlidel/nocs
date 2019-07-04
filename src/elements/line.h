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
    bool _x_only;
    std :: exponential_distribution <double> _exp_distribution;
    std :: uniform_real_distribution <double> _unif_distribution;
    std :: default_random_engine * _random_engine;

public:

    // Constructors

    xline();
    xline(const double &xpos, const double &temperature = -1.0, const bool &randomness = false, const bool &multiplicative = false, const bool &x_only=false, std :: default_random_engine *random_engine = NULL);

    // Public members

    //TODO::Deal with the grid
    grid :: mark mark;

    // Getters

    const double & xposition() const;
    const double & temperature() const;
    const bool & multiplicative() const;
    const bool & randomness() const;
    const bool & x_only() const;

    // Methods
    
    double exp_random_extraction();
    double unif_random_extraction();
};

#endif