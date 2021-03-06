#include "window.h"

namespace graphics
{
  // Global definitions
  
  bool __enter__ = false;
  bool __click__ = false;
  __unused std :: mutex mtx; // mutex for critical section
  __unused const char * __default_title = const_cast <char *> ("nocs");
  __unused constexpr int __default_width = 750;
  __unused constexpr int __default_height = 750;
  __unused constexpr int __triangle_amount = 40;
  __unused constexpr double __twice_pi = 2.0f * M_PI;
  __unused std :: vector <sphere> sphere_buffer;
  __unused std :: vector <line> line_buffer;
  __unused std :: atomic <bool> request_drawing(false);
  __unused std :: atomic <bool> request_closing(false);

  // vector

  // Constructors

  vector :: vector()
  {
    this->x = 0.;
    this->y = 0.;
  }

  vector :: vector(double x, double y)
  {
    this->x = x;
    this->y = y;
  }

  // color

  // Constructors

  color :: color()
  {
  }

  color :: color(unsigned char red, unsigned char green, unsigned char blue)
  {
    this->red = red;
    this->green = green;
    this->blue = blue;
  }

  // sphere

  sphere :: sphere(vector center, double radius, color c): center(center), radius(radius), c(c)
  {}

  // line

  line :: line(vector beg, vector end, color c): beg(beg), end(end), c(c)
  {}

  // window

  // Constructors

  window :: window()
  {
#ifdef __graphics__
    if (!__started)
    {
      request_closing = false;
      window :: __th = std :: thread(window :: start);
    }
#endif
  }

  // Static methods

  void window :: draw(__unused const engine &engine)
  {
#ifdef __graphics__
    mtx.lock();
    // Reset buffers
    line_buffer.clear();
    sphere_buffer.clear();
    // Fill line_buffer 
    window :: grid(engine);
    engine.each<xline>([&](const xline &xline) {
      window :: list_lines(xline);
    });
    // Fill sphere_buffer
    engine.each<molecule>([&](const molecule &molecule) {
      window :: list_sphere(molecule);
    });
    engine.each<bumper>([&](const bumper &bumper) {
      window :: list_sphere(bumper);
    });
    // Set shared switch on
    request_drawing = true;
    mtx.unlock();
#endif
  }

  void window :: draw(__unused const engine &engine, __unused const uint8_t &tag)
  {
#ifdef __graphics__
    mtx.lock();
    // Reset buffers
    line_buffer.clear();
    sphere_buffer.clear();
    // Fill line_buffer
    window :: grid(engine);
    engine.each<xline>([&](const xline &xline) {
      window :: list_lines(xline);
    });
    // Fill sphere_buffer
    engine.each<molecule>(tag, [&](const molecule &molecule) {
      window :: list_sphere(molecule);
    });
    // Set shared switch on
    request_drawing = true;
    mtx.unlock();
#endif
  }

  void window :: wait_click()
  {
#ifdef __graphics__
    window :: thread_wait_click();
#endif
  }

  void window :: close_window()
  {
#ifdef __graphics__
    request_closing = true;
    __th.join();
#endif
  }

  // Static private methods

  void window :: start()
  {
#ifdef __graphics__
    // Basic setup for startup
    int myargc = 1;
    char *myargv[1];
    myargv[0] = strdup("nocs");
    glutInit(&myargc, myargv);
    // Fundamental options
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(600, 600);
    glutCreateWindow(__default_title);
    // White window
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Set proper projection method
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.f, 1.f, 0.f, 1.f); // xmin, xmax, ymin, ymax
    // Set Renderer funcion and timer
    glutDisplayFunc(window :: render);
    glutTimerFunc(0, window :: timer, 0);
    // Elements for wait_click()
    glutMouseFunc([](int button, __unused int state, __unused int x, __unused int y) {
      switch (button)
      {
      case GLUT_LEFT_BUTTON:
        __click__ = true;
        break;
      default:
        __click__ = false;
        break;
      }
    });
    // Set boolean on true and enter in MainLoop
    __started = true;
    glutMainLoop();
    // We exit the MainLoop. Execute closing.
    if (glutGetWindow())
      glutDestroyWindow(glutGetWindow());
    __started = false;
#endif
  }

  void window :: render()
  {
#ifdef __graphics__
    if (request_drawing)
    {
      mtx.lock();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clean the screen and the depth buffer
      glLoadIdentity(); // Reset the projection matrix

      for (auto it = line_buffer.begin(); it != line_buffer.end(); it++)
      {
        draw_line(*it);
      }
      for (auto it = sphere_buffer.begin(); it != sphere_buffer.end(); it++)
      {
        draw_circle(*it);
      }

      glutSwapBuffers();

      request_drawing = false;
      mtx.unlock();
    }

    if (request_closing)
    {
      glutLeaveMainLoop();
    }

#endif
  }

  void window :: timer(__unused int extra)
  {
#ifdef __graphics__
    glutPostRedisplay();
    glutTimerFunc(1, window :: timer, 0);
#endif
  }

  void window :: thread_wait_click()
  {
#ifdef __graphics__
    __click__ = false;
    while (true)
    {
      if (__click__)
        break;
      std :: this_thread :: sleep_for(std :: chrono :: milliseconds(50));
    }
#endif
  }

  void window :: draw_line(__unused line l)
  {
#ifdef __graphics__
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.f, 1.f, 0.f, 1.f); // xmin, xmax, ymin, ymax

    glBegin(GL_LINE_STRIP);
    glColor3d(l.c.red, l.c.green, l.c.blue);
    glVertex3d(GLdouble(l.beg.x), GLdouble(l.beg.y), GLdouble(0.));
    glVertex3d(GLdouble(l.end.x), GLdouble(l.end.y), GLdouble(0.));
    glEnd();
#endif
  }

  void window :: draw_circle(__unused sphere c)
  {
#ifdef __graphics__
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.f, 1.f, 0.f, 1.f); // xmin, xmax, ymin, ymax
    

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(c.center.x, c.center.y); // center of circle
    for (int i = 0; i <= __triangle_amount; i++)
    {
      glColor3f(c.c.red, c.c.green, c.c.blue);
      glVertex2f(
          c.center.x + (c.radius * std :: cos(i * __twice_pi / __triangle_amount)),
          c.center.y + (c.radius * std :: sin(i * __twice_pi / __triangle_amount)));
    }
    glEnd();
#endif
  }

  // Static members

  bool window :: __started = false;
  int window :: __id = 0;
  std :: thread window :: __th = std :: thread();

  void window :: list_sphere(const molecule &molecule)
  {
    for (size_t i = 0; i < molecule.size(); i++)
    {
      vec position = molecule.position() + molecule[i].position() % molecule.orientation();

      for (double dx : {1., 0., -1.})
        for (double dy : {1., 0., -1.})
          sphere_buffer.push_back(sphere({dx + position.x, dy + position.y}, molecule[i].radius()));
    }
  }

  void window :: list_sphere(const bumper &bumper)
  {
    for (double dx : {1., 0., -1.})
      for (double dy : {1., 0., -1.})
        sphere_buffer.push_back(sphere({dx + bumper.position().x, dy + bumper.position().y}, bumper.radius(), {1, 0, 0}));
  }

  void window :: list_lines(const xline &xline)
  {
    line_buffer.push_back(line({xline.xposition(), 0.}, {xline.xposition(), 1.}, {0, 0, 0}));
  }

  void window :: grid(const engine &engine)
  {
    double step = 1. / engine.fineness();
    for (size_t i = 1; i < engine.fineness(); i++)
    {
      line_buffer.push_back(line({step * i, 0.}, {step * i, 1.}));
      line_buffer.push_back(line({0., step * i}, {1., step * i}));
    }
  }
}
