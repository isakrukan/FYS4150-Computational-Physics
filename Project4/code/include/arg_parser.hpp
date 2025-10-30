#ifndef __arg_parser_cpp__
#define __arg_parser_cpp__

#include <string>
#include <iostream>

/**
 * @brief Struct to hold command-line arguments
 */
struct Args
{
    double temperature = 1.0;
    double coupling = 1.0;
    int lattice_size = 2;
    int steps_per_cycle = 1000;
    int total_cycles = 100;
    std::string filename = "";
    bool ordered = false;
};

/**
 * @brief Function for parsing command-line arguments
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return Args struct containing the parsed values
 */
Args parse_args(int argc, char *argv[]);

#endif