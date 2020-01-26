#ifdef __binding__

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
namespace py = pybind11;

#include <tuple>
#include <vector>
#include <iomanip>
#include <random>
#include <cmath>
#include <sstream>
#include <string>

#include "engine/engine.hpp"
#include "graphics/window.h"

class engine_wrapper
{
    // Members

    std::default_random_engine re;
    engine my_engine;
    double time;
    enum tags {traced1, traced2, traced3};

    std::vector<size_t> ids;

    std::vector<std::tuple<unsigned int, double, double, double, double, double, double, double>> tracking;

public:

    // CONSTRUCTOR

    engine_wrapper(unsigned int grid_size=1, bool rand_seed=false) : my_engine(grid_size), time(0.0)
    {
        if(rand_seed)
            re.seed(42);

        my_engine.on<events ::molecule>(
            traced1,
            [&](const report<events ::molecule> my_report) {
                if (std ::find(ids.begin(), ids.end(), my_report.alpha.id()) != ids.end())
                {
                    std::tuple<unsigned int, double, double, double, double, double, double, double> data(
                        my_report.alpha.id(),
                        my_report.time(),
                        my_report.alpha.mass(),
                        my_report.alpha.energy.after(),
                        my_report.alpha.position().x,
                        my_report.alpha.position().y,
                        my_report.alpha.velocity.after().x,
                        my_report.alpha.velocity.after().y);
                    tracking.push_back(data);
                }
                if (std ::find(ids.begin(), ids.end(), my_report.beta.id()) != ids.end())
                {
                    std::tuple<unsigned int, double, double, double, double, double, double, double> data(
                        my_report.beta.id(),
                        my_report.time(),
                        my_report.beta.mass(),
                        my_report.beta.energy.after(),
                        my_report.beta.position().x,
                        my_report.beta.position().y,
                        my_report.beta.velocity.after().x,
                        my_report.beta.velocity.after().y);
                    tracking.push_back(data);
                }
            });

        my_engine.on<events ::xline>(
            traced1,
            [&](const report<events ::xline> my_report) {
                std::tuple<unsigned int, double, double, double, double, double, double, double> data(
                    my_report.id(),
                    my_report.time(),
                    my_report.mass(),
                    my_report.energy.after(),
                    my_report.position().x,
                    my_report.position().y,
                    my_report.velocity.after().x,
                    my_report.velocity.after().y);
                tracking.push_back(data);
            });
    }

    // Public Methods

    void add_basic_xline(double position)
    {
        xline my_line(position);
        my_engine.add(my_line);
    }

    void add_fixed_xline(double position, double temperature)
    {
        xline my_line(
            position,
            temperature,
            false,
            false,
            true // LOCKED Y VELOCITY
        );
        my_engine.add(my_line);
    }

    void add_random_xline(double position, double temperature)
    {
        xline my_line(
            position,
            temperature,
            true,
            false,
            true, // LOCKED Y VELOCITY
            &re
        );
        my_engine.add(my_line);
    }

    void add_multiplicative_xline(double position, double elasticity)
    {
        xline my_line(
            position,
            elasticity,
            false,
            true,
            true // LOCKED Y VELOCITY
        );
        my_engine.add(my_line);
    }

    unsigned int add_molecule(double x, double y, std::vector<double> x_atom, std::vector<double> y_atom, std::vector<double> r_atom, std::vector<double> mass_atom, double vx, double vy, double orientation, double ang_rotation, bool tracking)
    {
        std::vector<atom> atoms;

        for(int i = 0; i < x_atom.size(); i++)
        {
            atoms.push_back(atom({x_atom[i], y_atom[i]}, mass_atom[i], r_atom[i]));
        }

        molecule my_molecule(
            atoms,
            {x, y},
            {vx, vy},
            orientation,
            ang_rotation
        );

        unsigned int my_molecule_id = my_engine.add(my_molecule);

        if(tracking)
        {
            my_engine.tag(my_molecule_id, traced1);
            ids.push_back(my_molecule_id);
        }
        return my_molecule_id;
    }
    
    std::vector<std::tuple<double, double, double, double, double, double, double>> get_sim_photo()
    {
        std::vector<std::tuple<double, double, double, double, double, double, double>> data_vec;

        my_engine.each<molecule>([&](const molecule &current_molecule) {
            data_vec.push_back(std::tuple<double, double, double,  double, double, double, double>(
                current_molecule.mass(),
                current_molecule.radius(),
                current_molecule.energy(),
                current_molecule.position().x,
                current_molecule.position().y,
                current_molecule.velocity().x,
                current_molecule.velocity().y
                ));
        });

        return data_vec;
    }

    std::vector<std::tuple<unsigned int, double, double, double, double, double, double, double>> get_tracking_data()
    {
        return tracking;
    }

    void clear_tracking_data()
    {
        tracking.clear();
    }

    void run(double time_interval)
    {
        my_engine.run(time + time_interval);
        time += time_interval;
    }
};

// PYTHON BINDINGS

PYBIND11_MODULE(engine_wrapper, m)
{
    py::class_<engine_wrapper>(m, "engine_wrapper")
        .def(py::init<int, bool>())
        .def("add_basic_xline", &engine_wrapper::add_basic_xline)
        .def("add_random_xline", &engine_wrapper::add_random_xline)
        .def("add_multiplicative_xline", &engine_wrapper::add_multiplicative_xline)
        .def("add_molecule", &engine_wrapper::add_molecule)
        .def("get_sim_photo", &engine_wrapper::get_sim_photo)
        .def("get_tracking_data", &engine_wrapper::get_tracking_data)
        .def("run", &engine_wrapper::run);
}

#endif
