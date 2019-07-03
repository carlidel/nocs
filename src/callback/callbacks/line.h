// Forward declarations

#ifndef __nobb__callback__callbacks__callbackforward
#define __nobb__callback__callbacks__callbackforward

template <typename, typename = void> class callback;

#endif

#if !defined(__forward__) && !defined(__nobb__callback__callbacks__line__h)
#define __nobb__callback__callbacks__line__h

// Forward includes

#define __forward__
#include "event/events/line.h"
#undef __forward__

template <> class callback <events :: xline, void>
{
public:

  // Methods

  virtual void trigger(const events :: xline &) = 0;
};

template <typename lambda> class callback <events :: xline, lambda> : public callback <events :: xline, void>
{
  // Members

  lambda _callback;

public:

  // Constructors

  callback(const lambda &);

  // Methods

  void trigger(const events :: xline &);
};

#endif
