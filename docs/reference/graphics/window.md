## Class `window`

### Overview
Class `window` is a simple graphical interface based on `Freeglut3` which allows the user to visualize an `engine` in a specific window.

The class executes the entire window instance into a separate thread, so that the requested drawing procedures do not interfere with the actual calculations.

Moreover, it offers the method `wait_click` for pausing the entire program until a mouse click is detected inside the window.

The entire class is wrapped into the `namespace graphics`, so that its helper structs do not interfere with the rest of the code.

### Interface

#### Constructor

`window()`

Starts a `GLUT` window in a separate thread.

**REMARK: you can only start one window at the time.**

#### Public methods

  * `window()`

    default construction for the window. It needs to be called in order to have the graphical thread started.

  * `static void draw(const engine & engine)`

    draws all the elements of the simulation.

  * `static void draw(const engine & engine, const unit8_t & tag)`

    draws only the elements of the simulation with the given tag.

  * `static void wait_click()`

    blocks the execution until a mouse click is detected inside the window.

  * `static void close_window()`

    closes the window properly.

### Private Helper Classes and Methods

#### Helper structs

* `struct vector`

  gives a more simplified version of a 2D vector for drawing purposes.

* `struct color`

  gives a simple wrapper for managing colors for `Freeglut`.

* `struct sphere`

  allows the proper formulation and construction for a sphere to be drawn in the graphical context.

* `struct line`

  allows the proper formulation and construction for a line to be drawn in the graphical context.

#### Private static methods

* `void start()`

  starts properly the GLUT window instance inside the graphical thread.

* `void renderer()`

  renderer function called by the `GlutMainLoop()`.

* `void timer(int)`

  standard GLUT timer function in order to have the `renderer` function called by the `GlutMainLoop()`.

* `static void thread_wait_click()`

  Blocks the graphical thread (and therefore the entire program thanks to a mutex) until a mouse click inside the window is detected.

* `void draw_line(__unused line l)`

  Draws a given line with the various `gl` methods.

* `void draw_circle(__unused sphere c)`

  Draws a given sphere with the various `gl` methods.

* `void list_sphere(const molecule & m)`
   
  `void list_sphere(const bumper & b)`

  Given a molecule or a bumper, adds the spheres to be drawn to the corresponding shared `sphere_buffer` for the graphical thread.

* `void grid(const engine & e)`

  Given the NOCS engine, takes the fineness of the grid and fills the shared `line_buffer` with the proper lines in order to properly draw the grid.