#ifndef __arg_parser_hpp__
#define __arg_parser_hpp__

#include <string>
#include <iostream>

/**
 * @brief Struct to hold command-line arguments
 */
struct Args
{
    // General parameters
    double sigma_x = 0.05;
    double sigma_y = 0.05;
    double center_x = 0.25;
    double center_y = 0.5;
    double momentum_x = 200.0;
    double momentum_y = 0.0;
    double V_0 = 0.0;

    // Simulation parameters
    double T = 0.008;          ///< Total simulation time
    double dt = 2.5e-5;        ///< Time step size
    double L = 1.0;            ///< Total simulation length
    double h = 0.005;          ///< Spatial step size
};

/**
 * @brief Function for parsing command-line arguments
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return Args struct containing the parsed values
 */
Args parse_args(int argc, char *argv[]);

#endif