#ifndef __lockpick__
#define __lockpick__

// Libraries

#ifdef __graphics__

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#include <GL/freeglut.h>
#endif // __graphics__

#include <cmath>
#include <cstring>

namespace lockpick
{

  struct vector
  {
    // Members

    double x;
    double y;

    // Constructors

    vector();
    vector(double x, double y);

    // Destructor

    ~vector() = default;
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


    // Destructor

    ~color() = default;
  };

  class window
  {
    // Members

    int _id;

  protected:

    struct _glcircle
    {
      // Members

      unsigned int subdivs;
      double * buf;

      // Constructors

      _glcircle();
      _glcircle(unsigned int subdivs);

      // Destructor

      ~_glcircle();

      // Operators

      _glcircle & operator=(_glcircle g);

    };

    class _circle
    {
      // Members

      unsigned int _size;

    protected:

      // Static members

      double _x;
      double _y;
      color _c;

    public:

      double _scale;

      // Constructors

      _circle();
      _circle(vector center, double scale, color c, unsigned int size);
      _circle(unsigned int size);

      // Destructor

      ~_circle() = default;

      // Methods

      void draw(vector center, double scale, color c);
    };

    // Static members

    static bool __started;

    static char * __default_title;
    static int __default_width;
    static int __default_height;

    static int __window_count;

    _circle __ball;
    _glcircle __glball;

  public:

    // Constructors

    window(const char * title = nullptr, int width = __default_width, int height = __default_height, int position_x = 0, int position_y = 0, color background = color(255, 255, 255), int frame_width_percentage = 95, int frame_height_percentage = 95);

    // Destructor

    ~window();

    // Methods

    void line(vector from, vector to);
    void circle(vector center, double radius, color c = {1, 0, 0});


    // Static methods

    static void set_default_title(const char * title);
    static void set_default_size(int width, int height);
    static void wait_enter();
    static void wait_click();
    static void flush();
    static void clear();

  private:

    // Static private methods

    static void start();
  };

}

#endif
