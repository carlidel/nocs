# NOCS

## (Not Only Colliding Spheres)

[![docs](https://img.shields.io/readthedocs/:packageName/:version.svg?style=plastic)](https://carlidel.github.io/nocs)
[![GitHub pull-requests](https://img.shields.io/github/issues-pr/matteomonti/nocs.svg?style=plastic)](https://github.com/matteomonti/nocs/pulls)
[![GitHub issues](https://img.shields.io/github/issues/matteomonti/nocs.svg?style=plastic)](https://github.com/matteomonti/nocs/issues)

[![GitHub stars](https://img.shields.io/github/stars/matteomonti/nocs.svg?label=Stars&style=social)](https://github.com/matteomonti/nocs/stargazers)
[![GitHub watchers](https://img.shields.io/github/watchers/matteomonti/nocs.svg?label=Watch&style=social)](https://github.com/matteomonti/nocs/watchers)

Exact 2D gas dynamics framework.

|List of authors||
|:-:|:-:|
| **Author** | [**M. Monti**](https://github.com/matteomonti) |
| **Contributor** | [**C.E. Montanari**](https://github.com/carlidel) |
| **Contributor** | [**N. Curti**](https://github.com/Nico-Curti) |

| **Build Status**              |
|:-----------------------------:|
| **Linux / MacOS** : [![travis](https://travis-ci.com/Nico-Curti/nocs.svg?branch=master)](https://travis-ci.com/Nico-Curti/nocs) |
|**Windows** : in development|

# Table of Contents

- [NOCS](#nocs)
  - [(Not Only Colliding Spheres)](#not-only-colliding-spheres)
- [Table of Contents](#table-of-contents)
- [What is NOCS](#what-is-nocs)
  - [Works about NOCS](#works-about-nocs)
- [Compiling the project](#compiling-the-project)
  - [Dependencies](#dependencies)
  - [Compiling commands](#compiling-commands)
    - [Compile everything from source](#compile-everything-from-source)

# What is NOCS

NOCS is a C++ 2D event-based engine for simulating molecules composed of hard spheres of arbitrary mass and dimension.

It's meant to be used for simulating statistical mechanics scenarios in the fastest and most precise way possible, by computing analytically every collision between molecules and by integrating the motion of the molecules only when strictly necessary.

## Works about NOCS

1. C. E., Montanari (2017) Development of an event-based simulator for analysing excluded volume effects in a Brownian gas. [Bachelor thesis], Università di Bologna, Corso di Studio in Fisica (L-DM270) <http://amslaurea.unibo.it/view/cds/CDS8007/>
2. A., Cerfogli (2018) Entropic forces and dynamical properties of a rigid body Boltzmann gas. [Master thesis], Università di Bologna, Corso di Studio in Fisica (LM-DM270) <http://amslaurea.unibo.it/view/cds/CDS8025/>
3. A. Bazzani, S. Vitali, C. E. Montanari, M. Monti e G. Castellani. (2019) Stochastic properties of colliding particles in a non-equilibrium thermal bath. (Abstract) <https://sites.google.com/view/lfo12-13aprile2019/>
  
# Compiling the project

## Dependencies

In order to properly compile the project, it's necessary to have `cmake` and a compiler that supports adequately the C++14 standard.

If you wish to have also the graphical support within your simulation executable, you will need a valid implementation of `freeglut3` and a proper `OpenGL` development environment. On Linux it's sufficient to have installed the `freeglut3-dev` package:

```bash
sudo apt install freeglut3-dev
```

## Compiling commands

### Compile everything from source

After you have written your code in `src/main.cpp`, you can execute the compilation and obtain the executable. To do so, follow these steps:

1. Create a `bin` folder;
2. Open a terminal in the `bin` folder;
3. Execute the following command if you wish to NOT have graphical support (i.e. every graphical method called in `src/main.cpp` will NOT be included in the compilation):

   ```bash
   cmake ..
   ```

   If instead you wish to have graphical support:

   ```bash
   cmake .. -DGRAPHICS=on
   ```

   Also you can of course set the build type with `-DCMAKE_BUILD_TYPE` and either work with `Debug` or `Release`.
4. Execute the compilation with the `make` command.
5. You will now have your executable `main` in the `bin` folder.
6. You will also find an executable for every source file available in the `examples` folder