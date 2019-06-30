#ifndef __nobb__callback__callbacks__line__hpp
#define __nobb__callback__callbacks__line__hpp

#include "line.h"
#include "event/reports/line.h"

// Constructors

template <typename lambda> callback <events :: xline, lambda> :: callback(const lambda & callback) : _callback(callback)
{
}

// Methods

template <typename lambda> void callback <events :: xline, lambda> :: trigger(const events :: xline & event)
{
  this->_callback(report <events :: xline> (event));
}

#endif
