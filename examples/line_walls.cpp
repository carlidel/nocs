#ifdef __main__

#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <random>
#include <vector>
#include <algorithm>
#include <unistd.h>

#include "engine/engine.hpp"
#include "graphics/window.h"

// Parametri:

const bool LOAD_FROM_FILE = false;

const bool LOCKED_Y_VELOCITY = true; // blocco velocità y su collisioni su linee

const bool WALLS = true; // Ci sono o no le pareti di xlines calde e fredde?

// CASO BASIC
const bool BASIC_XLINES = false;

// CASO FIXED (temperatura che restta la velocità x)
const bool FIXED_XLINES = false;
const double HOT_TEMPERATURE = 2.0;
const double COLD_TEMPERATURE = 1.0;

// CASO RANDOM
const bool RANDOM_XLINES = false; // ci sono bumpers con distribuzione randomica delle velocità x generate alle varie diverse collisioni?
/* NELLA DISTRIBUZIONE ESPONENZIALE IN C++ SI IMPOSTA
   IL VALORE DI LAMBDA SECONDO LA FORMULA
   f(x) = \lambda \exp(- \lambda x)
   QUESTI DUE PARAMETRI k1 k2 SOTTO PERò VERRANNO PASSATI IN FORMA
   \lambda1 = (1 / k1)
   \lambda2 = (1 / k2)
   Così maggiore valore indica maggiore velocità media nella distribuzione*/
const double HOT_DISTRIBUTION = 10.0;
const double COLD_DISTRIBUTION = 1.0;

// CASO MOLTIPLICATIVO (coefficiente elastico invece che temperatura)
const bool MULTIPLICATIVE_XLINES = false;
const double HOT_ELASTICITY = 1.1;
const double COLD_ELASTICITY = 0.9;

// LIGHT MOLECULES
const unsigned int N_LIGHT_MOLECULES = 2000;
const unsigned int N_TRACED_LIGHT_MOLECULES = 20;
const double LIGHT_MOLECULE_RADIUS = 0.005;
const double LIGHT_MOLECULE_SPACING = 0.001;
const double LIGHT_MOLECULE_MASS = 1.0;
const double LIGHT_MOLECULE_STARTING_ENERGY = 0.05;

// HOT MOLECULES
const unsigned int N_HEAVY_MOLECULES = 20;
const unsigned int N_TRACED_HEAVY_MOLECULES = 20;
const double HEAVY_MOLECULE_RADIUS = 0.001;
const double HEAVY_MOLECULE_SPACING = 0.001;
const double HEAVY_MOLECULE_MASS = 25.0;
const double HEAVY_MOLECULE_STARTING_ENERGY = 0.05;

const double TIME_TRACING_SKIP = 0.0; // visto che le molecole partono tutte pigiate come acciughe... magari val la pena di saltare il tracciamento degli eventi pigiati "al tempo 0"? Non lo so... val la pena di testare.

const double SIMULATION_TIME = 50.0; // Tempo di arrivo finale della simulazione
const unsigned int N_SAMPLES = 5000;   // Quanti samples intermedi far fare alla simulazione? (questi verranno distribuiti uniformemente lungo l'esecuzione)
const unsigned int GRID_NUM = 25;    // Grid number

int main()
{
    // RANDOM ENGINE SETUP
    std::default_random_engine re;
    // PSEUDORANDOM SEED (credo si faccia così?)
    //re.seed(42);

    // Input setup
    std ::ifstream in_light;
    std ::ifstream in_heavy;
    double temp_x, temp_y;
    if (LOAD_FROM_FILE)
    {
        in_light = std ::ifstream("light_input.txt");
        in_heavy = std ::ifstream("heavy_input.txt");
    }

    // Output setup
    std ::ofstream out_all("output_all.txt");
    std ::ofstream out_traced("output_traced.txt");

    graphics :: window my_window;           // Per la grafica...
    engine my_engine(GRID_NUM); // Regolare in base a dimensioni scelte

    // Costruzione delle pareti
    if (WALLS)
    {
        if (BASIC_XLINES)
        {
            // xlines puramente elastiche
            xline sx_line(0.0001);
            xline dx_line(0.9999);
            my_engine.add(sx_line);
            my_engine.add(dx_line);
        }
        else if (FIXED_XLINES)
        {
            xline cold_line(
                0.0001,           // coordinata x sinistra
                COLD_TEMPERATURE, // temperatura fredda
                false,
                false,
                LOCKED_Y_VELOCITY);
            xline hot_line(
                0.9999,          // coordinata x destra
                HOT_TEMPERATURE, // temperatura calda
                false,
                false,
                LOCKED_Y_VELOCITY);
            my_engine.add(cold_line);
            my_engine.add(hot_line);
        }
        else if (RANDOM_XLINES)
        {
            bumper cold_line(
                0.0001, // coodinata x sinistra
                1 / COLD_DISTRIBUTION,
                true,
                false,
                LOCKED_Y_VELOCITY,
                &re);
            bumper hot_line(
                0.9999, // coordinata x destra
                1 / HOT_DISTRIBUTION,
                true,
                false,
                LOCKED_Y_VELOCITY,
                &re);
            my_engine.add(cold_line);
            my_engine.add(hot_line);
        }
        else if (MULTIPLICATIVE_XLINES)
        {
            bumper cold_line(
                0.0001, // coodinata x sinistra
                1 / COLD_DISTRIBUTION,
                false,
                true,
                LOCKED_Y_VELOCITY);
            bumper hot_line(
                0.9999, // coordinata x destra
                1 / HOT_DISTRIBUTION,
                false,
                true,
                LOCKED_Y_VELOCITY);
            my_engine.add(cold_line);
            my_engine.add(hot_line);
        }
    }

        
    // Creazione dei tag
    enum tags
    {
        light,
        heavy,
        traced
    };
    // Vector per gli ID tracciati
    std ::vector<size_t> ids;

    double lower_bound = 0;
    double upper_bound = M_PI * 2;
    std::uniform_real_distribution<double> unif(lower_bound, upper_bound);

    // Aggiunta delle molecole leggere

    double starting_velocity = pow(LIGHT_MOLECULE_STARTING_ENERGY / (0.5 * LIGHT_MOLECULE_MASS), 0.5);

    unsigned int inserted = 0;
    if (N_LIGHT_MOLECULES)
        for (double y = LIGHT_MOLECULE_RADIUS; y < 1.0 - LIGHT_MOLECULE_RADIUS; y += 2 * LIGHT_MOLECULE_RADIUS + LIGHT_MOLECULE_SPACING)
        {
            for (double x = 2 * (LIGHT_MOLECULE_RADIUS + 0.0002); x < 1.0 - 2 * (LIGHT_MOLECULE_RADIUS + 0.0002); x += 2 * LIGHT_MOLECULE_RADIUS + LIGHT_MOLECULE_SPACING)
            {
                double angle = unif(re);
                double v_x = starting_velocity * cos(angle);
                double v_y = starting_velocity * sin(angle);
                if (LOAD_FROM_FILE)
                    in_light >> temp_x >> temp_y >> v_x >> v_y;
                molecule my_molecule(
                    {{{0., 0.}, LIGHT_MOLECULE_MASS, LIGHT_MOLECULE_RADIUS}},
                    {(LOAD_FROM_FILE ? temp_x : x), (LOAD_FROM_FILE ? temp_y : y)},
                    {v_x, v_y},
                    0.,
                    0.);
                size_t my_id = my_engine.add(my_molecule);
                my_engine.tag(my_id, light);
                if (inserted < N_TRACED_LIGHT_MOLECULES)
                {
                    my_engine.tag(my_id, traced);
                    ids.push_back(my_id);
                }
                inserted++;
                if (inserted >= N_LIGHT_MOLECULES)
                    break;
            }
            if (inserted >= N_LIGHT_MOLECULES)
                break;
        }

    // Aggiunta delle molecole pesanti (occhio che non si intersechino!)

    starting_velocity = pow(HEAVY_MOLECULE_STARTING_ENERGY / (0.5 * HEAVY_MOLECULE_MASS), 0.5);

    inserted = 0;
    if (N_HEAVY_MOLECULES)
        for (double y = 1.0 - HEAVY_MOLECULE_RADIUS; y > HEAVY_MOLECULE_RADIUS; y -= 2 * HEAVY_MOLECULE_RADIUS + HEAVY_MOLECULE_SPACING)
        {
            for (double x = 2 * (HEAVY_MOLECULE_RADIUS + 0.0002); x < 1.0 - 2 * (HEAVY_MOLECULE_RADIUS + 0.0002); x += 2 * HEAVY_MOLECULE_RADIUS + HEAVY_MOLECULE_SPACING)
            {
                double angle = unif(re);
                double v_x = starting_velocity * cos(angle);
                double v_y = starting_velocity * sin(angle);
                if (LOAD_FROM_FILE)
                    in_heavy >> temp_x >> temp_y >> v_x >> v_y;
                molecule my_molecule(
                    {{{0., 0.}, HEAVY_MOLECULE_MASS, HEAVY_MOLECULE_RADIUS}},
                    {(LOAD_FROM_FILE ? temp_x : x), (LOAD_FROM_FILE ? temp_y : y)},
                    {v_x, v_y},
                    0.,
                    0.);
                size_t my_id = my_engine.add(my_molecule);
                my_engine.tag(my_id, light);
                if (inserted < N_TRACED_HEAVY_MOLECULES)
                {
                    my_engine.tag(my_id, traced);
                    ids.push_back(my_id);
                }
                inserted++;
                if (inserted >= N_HEAVY_MOLECULES)
                    break;
            }
            if (inserted >= N_HEAVY_MOLECULES)
                break;
        }

    // Sottoscrizione agli eventi delle molecole sotto tracciamento

    my_engine.on<events ::molecule>(
        traced,
        [&](const report<events ::molecule> my_report) {
            if (my_report.time() >= TIME_TRACING_SKIP)
            {
                if (std ::find(ids.begin(), ids.end(), my_report.alpha.id()) != ids.end())
                    out_traced << std ::fixed << std ::setprecision(8) << my_report.time() << "\t"
                               << std ::fixed << my_report.alpha.id() << "\t"
                               << std ::fixed << std::setprecision(8) << my_report.alpha.mass() << "\t"
                               << std ::fixed << std::setprecision(8) << my_report.alpha.energy.after() << "\t"
                               << std ::fixed << std::setprecision(8) << my_report.alpha.position().x << "\t"
                               << std ::fixed << std::setprecision(8) << my_report.alpha.position().y << "\t"
                               << std ::fixed << std::setprecision(8) << my_report.alpha.velocity.after().x << "\t"
                               << std ::fixed << std::setprecision(8) << my_report.alpha.velocity.after().y << "\t"
                               << "molecule-molecule" << std::endl;
                else
                    out_traced << std ::fixed << std ::setprecision(8) << my_report.time() << "\t"
                               << std ::fixed << my_report.beta.id() << "\t"
                               << std ::fixed << std::setprecision(8) << my_report.beta.mass() << "\t"
                               << std ::fixed << std::setprecision(8) << my_report.beta.energy.after() << "\t"
                               << std ::fixed << std::setprecision(8) << my_report.beta.position().x << "\t"
                               << std ::fixed << std::setprecision(8) << my_report.beta.position().y << "\t"
                               << std ::fixed << std::setprecision(8) << my_report.beta.velocity.after().x << "\t"
                               << std ::fixed << std::setprecision(8) << my_report.beta.velocity.after().y << "\t"
                               << "molecule-molecule" << std::endl;
            }
        });

    my_engine.on<events ::bumper>(
        traced,
        [&](const report<events ::bumper> my_report) {
            if (my_report.time() >= TIME_TRACING_SKIP)
            {
                out_traced << std ::fixed << std ::setprecision(8) << my_report.time() << "\t"
                           << std ::fixed << my_report.id() << "\t"
                           << std ::fixed << std::setprecision(8) << my_report.mass() << "\t"
                           << std ::fixed << std::setprecision(8) << my_report.energy.after() << "\t"
                           << std ::fixed << std::setprecision(8) << my_report.position().x << "\t"
                           << std ::fixed << std::setprecision(8) << my_report.position().y << "\t"
                           << std ::fixed << std::setprecision(8) << my_report.velocity.after().x << "\t"
                           << std ::fixed << std::setprecision(8) << my_report.velocity.after().y << "\t"
                           << "bumper-molecule" << std::endl;
            }
        });

    // Resto delle cose brutte

    my_window.draw(my_engine); // Draw the content of the engine on the window
    
    my_window.wait_click();    // Guess what
#ifdef __graphics__
    std ::cout << "Simulation Start!!!" << std ::endl;
#endif

    double time_interval = SIMULATION_TIME / N_SAMPLES;

    for (unsigned int i = 0; i <= N_SAMPLES; ++i)
    {
        my_engine.run(i * time_interval); // Run UNTIL time

        // Report di ogni molecola sul file di testo...
        my_engine.each<molecule>([&](const molecule &current_molecule) {
            out_all << std ::fixed << std ::setprecision(2) << i * time_interval << "\t"
                    << current_molecule.mass() << "\t"
                    << std ::fixed << std ::setprecision(8) << current_molecule.energy() << "\t"
                    << std ::fixed << std ::setprecision(8) << current_molecule.position().x << "\t"
                    << std ::fixed << std ::setprecision(8) << current_molecule.position().y << "\t"
                    << std ::fixed << std ::setprecision(8) << current_molecule.velocity().x << "\t"
                    << std ::fixed << std ::setprecision(8) << current_molecule.velocity().y << std::endl;
        });

        // Graphics
        my_window.draw(my_engine);
        
#ifdef __graphics__
        usleep(10.e4);
#endif
    }
}

#endif
