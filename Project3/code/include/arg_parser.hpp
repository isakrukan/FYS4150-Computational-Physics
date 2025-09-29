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
    uint n_steps = 4000;
    int seed = 0.0;
    double T = 50.0;
};

/**
 * @brief Function for parsing command-line arguments
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return Args struct containing the parsed values
 */
Args parse_args(int argc, char *argv[]);

#endif