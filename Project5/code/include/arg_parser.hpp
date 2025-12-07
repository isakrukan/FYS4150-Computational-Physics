#ifndef __arg_parser_hpp__
#define __arg_parser_hpp__

#include <string>
#include <iostream>

/**
 * @brief Struct to hold command-line arguments
 */
struct Args
{
    /***  General parameters ***/

    // Wave packet parameters
    double sigma_x = 0.05;
    double sigma_y = 0.05;
    double center_x = 0.25;
    double center_y = 0.5;
    double momentum_x = 200.0;
    double momentum_y = 0.0;

    // Wall/slit parameters
    double v0 = 1e10;    ///< Potential height of slit "wall".
    double wc = 0.5;    ///< Centre position of wall in the x-direction.
    double wt = 0.02;   ///< Wall thickness in the x-direction.
    double a = 0.05;    ///< Distance between the two slits.
    double d = 0.05;   ///< Height of slits (y-direction).

    // Simulation parameters
    double T = 0.008;          ///< Total simulation time
    double dt = 2.5e-5;        ///< Time step size
    double L = 1.0;            ///< Total simulation length
    double h = 0.005;          ///< Spatial step size
    bool single_slit = false;  ///< Whether to include single slit in simulation.
    bool double_slit = false;  ///< Whether to include double slit in simulation.
    bool triple_slit = false;  ///< Whether to include triple slit in simulation.

    int N;  ///< Number of time steps.
    int M;  ///< Number of spatial steps.
    std::string identifier = "";  ///< Identifier for output files.
};

/**
 * @brief Function for parsing command-line arguments
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return Args struct containing the parsed values
 */
Args parse_args(int argc, char *argv[]);

#endif