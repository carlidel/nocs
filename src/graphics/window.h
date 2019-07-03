#ifndef __window__
#define __window__

// Libraries

#ifdef __graphics__

#define LIGHT

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#ifdef _MSC_VER
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#include <GL/freeglut.h>
#endif // __graphics__

#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <cmath>
#include <cstring>
#include <iostream>

// Includes

#include "molecule/atom.h"
#include "engine/engine.hpp"
#include "molecule/molecule.h"
#include "elements/bumper.h"
#include "elements/line.h"

#ifdef _MSC_VER
  #ifndef __unused
  #define __unused
  #endif
#else
  #ifndef __unused
  #define __unused __attribute__((__unused__))
  #endif
#endif

namespace graphics
{
  struct vector
  {
    // Members

    double x;
    double y;

    // Constructors

    vector();
    vector(double x, double y);
  };

  struct color
  {
    // Members

    unsigned char red;
    unsigned char green;
    unsigned char blue;

    // Constructor

    color();
    color(unsigned char red, unsigned char green, unsigned char blue);
  };

  struct sphere
  {
    // Members

    vector center;
    double radius;
    color c;

    // Constructor

    sphere();
    sphere(vector center, double radius, color c = {0, 0, 1});
  };

  struct line
  {
    // Members

    vector beg;
    vector end;
    color c;

    // Constructor

    line();
    line(vector beg, vector end, color c = {1, 0, 0});
  };

  class window
  {
  protected:

    // Static members

    static bool __started;
    static int __window_count;
    static int __id;
    static std :: thread __th;

  public:

    // Constructors

    window();

    // Static methods

    static void draw(const engine &);
    static void draw(const engine &, const uint8_t &);
    static void wait_click();
    static void close_window();

  private:

    // Static private methods

    static void start();
    static void render();
    static void timer(int);
    static void thread_wait_click();
    static void draw_line(__unused line);
    static void draw_circle(__unused sphere);

    static void list_sphere(const molecule &);
    static void list_sphere(const bumper &);
    static void list_lines(const xline &);
    static void grid(const engine &);
  };

}

#endif
