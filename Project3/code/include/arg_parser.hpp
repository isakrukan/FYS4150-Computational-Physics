#ifndef __arg_parser_cpp__
#define __arg_parser_cpp__

#include <string>
#include <iostream>
#include "penningTrap.hpp"

/**
 * @brief Struct to hold command-line arguments
 */
struct Args
{
    unsigned int n_steps = 4000;
    int seed = 1234;
    double T = 50.0;
    int n_particles = 2;
    bool problem_8 = false;
    bool problem_9 = false;
    double amplitude = 0.1;
    int n_freq = 10;
};

/**
 * @brief Function for parsing command-line arguments
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return Args struct containing the parsed values
 */
Args parse_args(int argc, char *argv[]);

#endif