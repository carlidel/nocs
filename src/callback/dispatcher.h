// Forward declarations

class dispatcher;

#if !defined(__forward__) && !defined(__nobb__callback__dispatcher__h)
#define __nobb__callback__dispatcher__h

// Libraries

#include <stdint.h>
#include <tuple>

// Includes

#include "callbacks/molecule.hpp"
#include "callbacks/bumper.hpp"
#include "callbacks/line.hpp"
#include "data/hashtable.hpp"
#include "data/set.hpp"

class dispatcher
{
  // Nested enums

  enum type {all, stag, dtag};

  // Members

  struct
  {
    hashtable <size_t, type> types;
    hashtable <size_t, callback <events :: molecule> *> all;

    struct
    {
      hashtable <size_t, std :: tuple <callback <events :: molecule> *, uint8_t>> handles;
      set <callback <events :: molecule> *> map[255];
    } stag;

    struct
    {
      hashtable <size_t, std :: tuple <callback <events :: molecule> *, uint8_t, uint8_t>> handles;
      set <callback <events :: molecule> *> map[255][255];
    } dtag;
  } _molecule;

  struct
  {
    hashtable <size_t, type> types;
    hashtable <size_t, callback <events :: bumper> *> all;

    struct
    {
      hashtable <size_t, std :: tuple <callback <events :: bumper> *, uint8_t>> handles;
      set <callback <events :: bumper> *> map[255];
    } stag;
  } _bumper;

  struct
  {
    hashtable <size_t, type> types;
    hashtable <size_t, callback <events :: xline> *> all;

    struct
    {
      hashtable <size_t, std :: tuple <callback <events :: xline> *, uint8_t>> handles;
      set <callback <events :: xline> *> map[255];
    } stag;
  } _xline;

public:

  // Methods

  size_t add(callback <events :: molecule> *);
  size_t add(callback <events :: molecule> *, const uint8_t &);
  size_t add(callback <events :: molecule> *, const uint8_t &, const uint8_t &);

  size_t add(callback <events :: bumper> *);
  size_t add(callback <events :: bumper> *, const uint8_t &);

  size_t add(callback <events :: xline> *);
  size_t add(callback <events :: xline> *, const uint8_t &);

  void trigger(const events :: molecule &);
  void trigger(const events :: bumper &);
  void trigger(const events :: xline &);

  template <typename etype> void remove(const size_t &);

private:

  // Private static members

  static size_t autoincrement;
};

#endif
