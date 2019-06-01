# Getting started with NOCS

## The elements of NOCS

In NOCS you are provided with a **1x1 empty space** you can fill up freely with **molecules**, composed by rigid spheres of arbitrary mass and radius, and **bumpers**, immovable spheres which can either be completely elastic in the collisions or instead execute different kinds of processing when colliding with a molecule. In order to speed up the simulation and avoid useless computations, you can set up a **grid** for subdividing your space.

### Space and grid

The space you work in NOCS is a 1x1 toroidal square (i.e. Snake-like space), in which every inserted molecule is tracked down in an event-based approach. Meaning, you don't have a continuous step-by-step integration equal for every molecule, but instead, you have an event system that tries to predicts which molecules will collide with which elements, and then only integrates the elements that will actually collide.

That implies that NOCS has to check every possible couple of elements and verify if they will collide or not, and then build a heap of events and then compute, one by one, the effects of every event and then recalculate many possible successive events.

Since that implies a computing time of $O(n^2)$, you are also provided with the possibility to split your region with a grid of arbitrary fineness. With that grid set, NOCS will only check for events between elements in the same sector or in neighborhood sectors, thus reducing the computation of events distant in time.

### Molecules

In NOCS you can build molecules of any shape using rigid spheres of arbitrary mass and radius, defined as **atoms**. You are provided then with many tools for tagging different categories of molecules and setting, for example, different elastic coefficients.

The constructor function of a molecule makes use of some syntactic sugar provided by the C++ language. More specifically, it makes use of implicit constructors for having a clean construction syntax.

### Bumpers

Another element you have in NOCS are immovable spheres, defined as bumpers, which can be used as obstacles or combined to make walls inside the simulation. You can have many type of bumpers:

1. basic bumpers that have a completely elastic collision with every interacting molecule;
2. bumpers that have a specific elastic coefficient for collisions;
3. bumpers that have a specific temperature and resets the energy of colliding molecules to a specific constant value;
4. bumpers that have a specific temperature and resets the energy of colliding molecules to a value randomly extracted from an exponential distribution.

## Basic commands

### Libraries to be included

```cpp
{% raw %}
#include "engine/engine.hpp"  /* This library contains the main core of the
                                 simulation and all the components necessary */

#include "graphics/window.h"  /* This library contains the tools for creating
                                 graphical representations of the simulation
                                 and it's therefore optional for the actual
                                 execution of a simulation */
{% endraw %}
```

### Declaring the fundamental thing

```cpp
{% raw %}
int main()
{
  engine my_engine(6); /* This declares an engine with a 6x6 grid subdivision.
                          my_engine will then be the framework for the entire
                          simulation */
}
{% endraw %}
```

#### How to deal with the grid

The engine's space is square of unit size.

Since it would be not ideal to compute the possible collision between two molecules far far away (there could be something like 10'000 more molecules between them), it is good to split out the space into smaller region, in order to analyze only the possible future interaction between molecules in the same zone or in adjacent zones.

Therefore, if you have elements that are at least smaller than a circle of diameter 0.1, you can set a proper grid that will speed up the computational time. (in this case the fineness limit of the grid is `1 / (0.1 * 2) = 5`)

### Building your first molecules and giving them a nice tag

In order to keep the syntax of the code as light as possible, we take advantage on implicit constructors for the various variables required by the main constructor methods.

For building an **atom**, you have to use the following constructor:

```cpp
atom(const vec & position, const double & radius, const double & mass)
```

Where `vec` is our object-oriented implementation of a 2D vector in a standard Euclidean space (if you want to learn more, please check the source code reference in the *geometry* folder).

By using implicit constructors, if you want, for example, make an atom in (0.0, 0.0) with radius 0.05 and mass 1.0, you can directly write:

```c++
atom({0.0, 0.0}, 0.05, 1.0)
```

But you will never need to build a single atom, since you can make directly an entire **molecule** by using the constructor

```cpp
molecule(const std :: vector<atom> & atoms, const vec & position = vec(0, 0), const vec & = velocity vec(0, 0), const double &  orientation = 0, const double angular_velocity & = 0)
```

With the same mentality, you can directly use implicit constructors to pass a nested list of doubles so that they will be interpreted as an `std::vector` of atoms.

If you, for example, want to make a molecule composed by 2 atoms of different size and mass, and define a starting position, orientation, velocity and rotation for it, you can use the following signature:

```cpp
molecule my_molecule
(
    { // This is a vector of atoms via implicit constructor
        { // First atom of the vector
          {0., 0.}, // Position (arbitrary reference system)
          1.,       // Mass
          0.005     // Radius
        },
        { // Second atom of the vector
          {0., 0.02}, // Position (arbitrary reference system)
          10.,        // Mass
          0.015       // Radius
        }
    },
    {0.5, 0.5},   // Position of CDM of molecule (inside simulation space)
    {0, 0},   // Initial velocity of CDM
    0.,       // Initial orientation (in radians)
    M_PI / 4. // Angular velocity (in radians / time unit)
);
```

The following code creates a bunch of identical molecules with a for loop and then inserts them inside the engine, with the method `engine::add`. This method returns an `id` for the element inserted, which you can save and use for many things, like tag assignment.

```cpp
{% raw %}
int main()
{
  engine my_engine(6); // 6x6 zones engine

  enum tags {heavy, light}; // creating some handy arbitrary tags for any possible need

  /*Creating a handful of molecules*/

  for(double x = 0.1; x <= 0.9; x += 0.1)
      for(double y = 0.1; y <= 0.4; y += 0.1)
      {
          molecule my_molecule
          (
              { // Array of atoms
                  // First atom
                  {
                    {0., 0.}, // Position (arbitrary reference system)
                    1.,       // Mass
                    0.005     // Radius
                  },
                  // Second atom
                  {{0., 0.02}, 10., 0.015}
              },
              {x, y},   // Position of center of mass
              {0, 0},   // Velocity of center of mass
              0.,       // Initial orientation (in radians)
              M_PI / 4. // Angular velocity (in radians / time unit)
          );

          size_t my_molecule_id = my_engine.add(my_molecule);
          // Now the molecule is in the engine, ready to work.
          // The function returns the id of the molecule inside the engine.

          my_engine.tag(my_molecule_id, heavy);
          // Adds tag heavy to the molecule.
          // REMARK: you can add tags to molecules only
          // after inserting them into the engine.
      }

      double swap = 1.;

      for(double x = 0.1; x <= 0.9; x += 0.1)
          for(double y = 0.6; y <= 0.9; y += 0.1)
          {
              molecule my_other_molecule // Same thing as before
              (
                  {
                      {{0., 0.}, 0.01, 0.02}
                  },
                  {x, y},
                  {3. * swap, 3. * swap},
                  0.,
                  0.
              );

              swap *= -1; // Serves the purpose to have a null total momentum
                          // (if that's what you want)

              size_t my_other_molecule_id = my_engine.add(my_other_molecule);
              my_engine.tag(my_other_molecule_id, light);
          }
}
{% endraw %}
```

### Building a bumper and placing it inside the engine

You can build a completely elastic bumper by using the simple constructor:

```cpp
bumper(const vec & position, const double & radius)
```

And add it in the engine like in the following example (REMARK: there is no tagging system for bumpers!):

```cpp
{% raw %}
int main()
{
  //[...]

  bumper my_bumper(
    {0.5, 0.5}, // Position
    0.03        // Radius
  );

  my_engine.add(my_bumper);
}
{% endraw %}
```

You can then make instead other kinds of bumpers by using different overloadings of the bumper constructor:

```cpp
bumper(const vec & position, const double & radius, const double & temperature = -1, const bool & multiplicative = false, const bool & randomness = false, std :: default_random_engine * random_engine = NULL)
```

* For a standard bumper with standard elastic collision, you have to provide only `position` and `radius`;
* For a bumper that resets the energy of every colliding molecule at a given value of `temperature`, you have to provide only `position`, `radius` and `temperature`.
* For a bumper that has a different elastic coefficient, you have to provide `position`, `radius`, `temperature` and set `multiplicative` to `true`. This will make the `temperature` value the elastic coefficient of the bumper. (Further development of NOCS may improve the naming standard)
* For a bumper that resets the energy of every colliding value at a randomly extracted value from an exponential distribution with `temperature` as average value, you have to provide `position`, `radius`, `temperature`, set `multiplicative` to `false`, set `randomness` to `true` and provide a pointer to an `std::default_random_engine`. This way, you have the possibility to fully control the eventual random element in the simulation.

### Changing the elasticity of the collision between two molecules

Thanks to the tagging system showed before, you are able to change the elasticity coefficient of specific collision between molecules. The way you can do that is shown in this example code below:

```cpp
{% raw %}
int main()
{
  //[...]

  my_engine.elasticity(heavy, heavy, 0.8); // Now if two molecules with tag
    // heavy collide, the collision will be anelastic with a coefficent of 0.8

  my_engine.elasticity(light, 1.5); // Now any collision involving a molecule
    // with tag light will be superelastic with a coefficent of 1.5

  my_engine.elasticity(1.0); // Now every collision is just normal
                             // and fully elastic
}
{% endraw %}
```

### Executing the simulation

Since this is an even-based simulation, you can (and should) specify just the times in which you are interested to observe and sample the condition of your simulation.

For instance, if you are simulating a Boltzmann Gas with many particles, and you are interested in observing the evolution only after 100, 500 and 1000 time units, you can just indicate these 3 times as target times and leave to NOCS the rest of the work.

You can do that with the `engine::run` method.

A new engine always starts at time 0.

```cpp
{% raw %}
int main()
{
  // Creating the engine and adding everything...
  engine my_engine(6);

  // [...]

  // Run the simulation to the time 100
  my_engine.run(100); // Run UNTIL time 100

  // Do some analysis...

  
  // Run the simulation to the time 500
  // (the engine right now is at time 100)
  my_engine.run(500); // Run UNTIL time 500

  // Do some analysis...

  // Run the simulation to the time 1000
  // (the engine right now is at time 500)
  my_engine.run(1000); // Run UNTIL time 1000

  // Do some analysis...
  // And so on..

}
{% endraw %}
```

If you wish to "enforce" a time-based-like integration, you can for example use a for loop and run the engine at incremental times with very small deltas... however, this will be far from efficient from every point of view (however, it might be extremely useful if you want to have a proper graphical visualization of what is happening).

```cpp
{% raw %}
int main()
{
  // Creating the engine and adding everything...
  engine my_engine(6);

  // [...]


  for(double time = 0.; ; time += 0.01)
    my_engine.run(time); // Run UNTIL time...

}
{% endraw %}
```

**REMARK: remember that this simulation is event-based, therefore you have to specify at what times you want to gather pictures of the simulation space. But remember then that a smaller delta time implies more integrations, which implies more numerical errors.**

### Subscribing to an event

NOCS offers you the possibility of gathering data about every event that happens during the simulation. You can both gather data about every event without any skimming or you can instead use the tagging system shown before so that you can obtain data regarding only molecules with particular tags.

To do so, the NOCS engine provides an internal dispatcher that allows you to create lambda functions capable to wrap the event objects inside a report object, which allows you an easy way to access all the possible information you might want to gather.

If you want to gather only the data about a particular kind of event involving only a particular kind of molecules, you can use this subscription system to build lambda functions that will set off only when the molecules with the given tags are involved in an event.

It sounds difficult, but it's more easy to do than to explain. You can take as reference the following commented code:

```cpp
{% raw %}
//[...]
#include <iostream>

int main()
{
  //[...]

  size_t id_of_subscription;

  id_of_subscription = my_engine.on // I want to subscribe to...
    <events :: molecule>  // ...any collision between two molecules
    ( // So I make a lambda function that takes this kind of report as argument
      [&](const report <events :: molecule> my_report)
      {
        /*and I write now here what I want it to do and print*/

        std :: cout << "There has been a collision between "
          << my_report.alpha.id() << " and "
          << my_report.beta.id() << std :: endl;

        std :: cout << "Delta energy for alpha: "
          << my_report.alpha.energy.delta() << std :: endl;
      }
    ); // and I've also memorized the subscription's id

  my_engine.on // I want to subscribe to...
    <events :: bumper>  // ...any collision between a bumper and a molecule...
    (
      heavy,  // ...that involves a molecule with a tag heavy

      // So I make a lambda function that takes this kind of report as argument
      [&](const report <events :: bumper> my_report)
      {
        /*and I write now here what I want it to do and print*/

        std :: cout << "There has been a collision between "
          << my_report.id() << " and " << a bumper << std :: endl;

        std :: cout << "And the molecule was heavy!" << std :: endl;
      }
    );

  my_engine.on // I want to subscribe to...
    <events :: molecule>  // ...any collision between a molecule and a molecule
    (
      heavy,  // ...that involves a molecule with a tag heavy
      light,  // ...AND a molecule with a tag light
      // So I make a lambda function that takes this kind of report as argument
      [&](const report <events :: molecule> my_report)
      {
        /*and I write now here what I want it to do and print*/

        std :: cout << "There has been a VERY SPECIFIC collision between "
          << my_report.alpha.id() << " and "
          << my_report.beta.id() << std :: endl;

        std :: cout << "Delta energy for alpha: "
          << my_report.alpha.energy.delta() << std :: endl;
      }
    );

  //...time goes on...

  my_engine.unsubscribe(id_of_subscription) // Now I want to unsubscribe from
                      // the first subscription because I don't need it anymore
}
{% endraw %}
```

As you can see, thanks to some other syntactic sugar of the C++ language, you can make use of the report objects to easily access every information about event data. For a complete list of what information you can gather with a report object, please consult:

* [This reference file](./reference/event/reports/molecule.md) for `report <events :: molecule>`
* [This reference file](./reference/event/reports/bumper.md) for `report <events :: bumper>`

### Gathering informations about specific groups of molecules

When the simulation is not running (i.e. when it's not executing an `engine::run` function), you have at your hands a still photo of your system at a specific time.

For gathering data in a practical way, you can use the method `engine::each`, and directly pass a lambda function to the various elements inside your simulation.

You can see a few examples of usage in the code below:

```cpp
{% raw %}
int main()
{
  //[...]

  double total_energy = 0.;

  my_engine.each <molecule>
  (
    [&](const molecule & current_molecule)
  /* Dear engine, for each molecule, please execute this lambda
     that accepts a const reference to the current molecule and captures
     total_energy.
     Each time it is called the lambda adds to total_energy the energy
     of the current molecule.
     Also known as: sum the energies of all the molecules */
    {
        total_energy += current_molecule.energy();
    }
  );

  double total_heavy_energy = 0.;

  my_engine.each <molecule>
  (
    heavy,
    [&](const molecule & current_molecule)
  /* Dear engine, for each molecule with tag heavy, please execute this lambda
     that accepts a const reference to the current molecule and captures
     heavy_total_energy.
     Each time it is called the lambda adds to total_heavy_energy the energy
     of the current molecule.
     Also known as: sum the energies of all the heavy molecules */
    {
        total_heavy_energy += current_molecule.energy();
    }
  );
}
{% endraw %}
```

For a complete reference on the available getters for a molecule object, please check [this reference file](./reference/molecule/molecule.md).

### Resetting the energy of one or more molecules

If you want to alter the energy of a molecule and at the same time maintaining all the proportion in the energy distribution (translational and rotational) you can use these methods shown in the commented code below.

You can make use of the tagging system even for this kind of operation.

```cpp
{% raw %}
int main()
{
  //[...]

  my_engine.reset.energy.id(1, 10.5)
  // set the energy of the molecule with id 1 to 10.5 units of energy

  my_engine.reset.energy.tag(heavy, 0.1)
  // set the energy of all the molecules with tag heavy to 0.1 units of energy

  my_engine.reset.energy.all(1.0)
  // Now every molecule has 1 unit of energy as energy
}
{% endraw %}
```

### And now, some graphics

With the graphical library included (and with the proper compilation flags enabled with Cmake) you can make use of some tools for visualizing the simulation progresses.

The library is based on `Freeglut3` and runs on a completely separated thread.

At this development stage are provided with 4 basic tools:

1. The window initialization command, which is as simple as just declaring a new `window` object:

   ```cpp
   graphics::window my_window
   ```

   Please note the presence of the `graphics` namespace.

2. The `window::draw` method, that takes the engine as argument and just draws all the elements in the engine (you can also specify a tag and get only the elements with that tag drawn):
   ```cpp
   my_window.draw(my_engine); // Draw everything
   my_window.draw(my_engine, my_special_tag); // Draw only tagged molecules
   ```

3. The `window::wait_click()` method, that blocks the entire program execution until a mouse click is detected inside the Freeglut window.

4. The `window::close_window()` method, that closes properly the window, you should use it for exiting the program with elegance and respectfullnes.

```cpp
{% raw %}

int main()
{
  engine my_engine(6);

  //[... adding random things into the engine just like above]

  molecule * a_random_molecule = new molecule (/*stuff*/);
  bumper * a_random_bumper = new bumper(/*stuff*/);

  my_engine.add(a_random_molecule);
  my_engine.add(a_random_bumper);

  window my_window; // Initialize the window

  my_window.wait_click();
  // Blocks the execution until a mouse click is detected inside the window

  my_window.draw(my_engine);
  // I want you window to draw all the elements inside engine

  my_window.wait_click();

  my_window.draw(my_engine, heavy);
  // Now I want you window to draw instead
  // all the elements inside engine with tag heavy

  my_window.wait_click();

  // Now if we start the simulation...
  for(double time = 0.; time <= 10.0; time += 0.01)
  {
    my_engine.run(time); // Run engine UNTIL time...
    my_window.draw(my_engine); // ..and then draw the engine
    if (fmod(time, 1.0) == 0)
      my_window.wait_click(); // If we want to pause after some steps
  }

  my_window.close_window() // Close the window with elegance
  return 0;
}

{% endraw %}
```
