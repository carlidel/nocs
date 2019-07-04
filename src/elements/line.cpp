#include "line.h"

// Constructors

xline :: xline()
{
}

xline ::xline(const double &xpos, const double &temperature, const bool &randomness, const bool &multiplicative, const bool &x_only, std ::default_random_engine *random_engine) : _xposition(xpos), _temperature(temperature), _multiplicative(multiplicative), _randomness(randomness), _x_only(x_only), _random_engine(random_engine)
{
    assert(!(this->_randomness && this->_multiplicative) && "A xline can't be random and multiplicative at the same time!");
    assert((this->_temperature == -1.0 || this->_temperature >= 0) && "Temperature not valid!");
    assert(!(this->_randomness && this->_temperature != -1.0) && "Temperature not valid for random xline!");
    assert(!(this->_multiplicative && this->_temperature != -1.0) && "Temperature not valid for multiplicative xline!");
    assert(!(this->_randomness && this->_random_engine == NULL) && "pointer to std::default_random_engine unspecified for random bumper");
    assert((this->_x_only && (this->_randomness || this->_multiplicative)) && "An x_only coordinate x line must be or random or multiplicative");


    if (this->_temperature != -1.0 && this->_randomness)
    {
        this->_exp_distribution = std :: exponential_distribution<double>(this->_temperature);
        this->_unif_distribution = std :: uniform_real_distribution<double> (0.0, 1.0);
    }
}

// Getters

const double & xline :: xposition() const
{
    return this->_xposition;
}

const double & xline :: temperature() const
{
    return this->_temperature;
}

const bool & xline :: multiplicative() const
{
    return this->_multiplicative;
} 

const bool & xline :: randomness() const
{
    return this->_randomness;
}

const bool & xline :: x_only() const
{
    return this->_x_only;
}

// Methods

double xline :: exp_random_extraction()
{
    assert(this->_randomness && "xline is NOT random!");
    return this->_exp_distribution(*(this->_random_engine));
}

double xline :: unif_random_extraction()
{
    assert(this->_randomness && "xline is NOT random!");
    return this->_unif_distribution(*(this->_random_engine));
}
