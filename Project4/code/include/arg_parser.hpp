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
    int lattice_size = 2;
    double coupling = 1.0;
    double temperature = 1.0;
    bool ordered = false;
    
    // MCMC parameters
    bool run = false;
    int steps_per_cycle = 0;    // If not changed, will be set to L^2
    int total_cycles = 1000;
    int burn_in_cycles = 0;
    int seed = 1234;

    // Temperature scan parameters
    bool temperature_scan = false;
    double temperature_min = 2.1;
    double temperature_max = 2.4;
    int temperature_steps = 30;

    // Output parameters
    std::string run_name = "";
    std::string out_dir = "";
};

/**
 * @brief Function for parsing command-line arguments
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return Args struct containing the parsed values
 */
Args parse_args(int argc, char *argv[]);

#endif