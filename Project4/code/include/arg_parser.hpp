#ifndef __arg_parser_cpp__
#define __arg_parser_cpp__

#include <string>
#include <iostream>

/**
 * @brief Struct to hold command-line arguments
 */
struct Args
{
    // General/lattice parameters
    int lattice_size = 2;           ///< Size of \ref Lattice "Lattice".
    double coupling = 1.0;          ///< Coupling constant J related to energy of \ref Lattice "Lattice".
    double temperature = 1.0;       ///< Temperature of \ref Lattice "Lattice".
    bool ordered = false;           ///< Whether the initial configuration of the \ref Lattice "Lattice" is ordered (all pointing upwards) or not.
    
    // MCMC parameters
    bool run = false;               ///< Whether to run a \ref MCMC "MCMC" simulation or not 
    int steps_per_cycle = 0;        ///< Steps per cycle in \ref MCMC "MCMC" simulation. Defaults to L^2.
    int total_cycles = 100000;      ///< Total amount of cycles in \ref MCMC "MCMC" simulation.
    int burn_in_cycles = 0;         ///< Number of burn in cycles for \ref MCMC "MCMC" simulation.
    int seed = 1234;                ///< Seed for random number generator(s).

    // Temperature scan parameters
    bool temperature_scan = false;  ///< Whether to run \ref generate_temp_scan_name "temperature scan" or not.
    double temperature_min = 2.1;   ///< Minimum temperature to scan over.
    double temperature_max = 2.4;   ///< Maximum temperature to scan over.
    int temperature_steps = 30;     ///< Number of temperatures to scan over, ranging from \ref Args::temperature_min "temperature_min" to \ref Args::temperature_max "temperature_max".

    // Output parameters
    bool save_data = true;          ///< Whether to save data or not.
    std::string run_name = "";      ///< Name identifier for run.
    std::string out_dir = "";       ///< Where to store data.
};

/**
 * @brief Function for parsing command-line arguments
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return Args struct containing the parsed values
 */
Args parse_args(int argc, char *argv[]);

#endif