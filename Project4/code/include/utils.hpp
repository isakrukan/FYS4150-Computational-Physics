#ifndef __utils_hpp__
#define __utils_hpp__

#include <iostream>
#include <iomanip>
#include <sstream>
#include <armadillo>
#include "lattice.hpp"
#include "mcmc.hpp"
#include "arg_parser.hpp"

/**
 * @brief Modulo function that handles negative values correctly.
 * @param a Dividend, must be > -b
 * @param b Divisor.
 * @return Result of a mod b.
 */
int pbc(int x, int L);

/**
 * @brief Generate name for a temperature scan run.
 * @param args Struct containing simulation parameters.
 * @return Generated temperature scan name.
 */
std::string generate_temp_scan_name(const Args &args);

/**
 * @brief Generate run name based on simulation parameters.
 * @param args Struct containing simulation parameters.
 * @return Generated run name.
 */
std::string generate_run_name(const Args &args);

/**
 * @brief Perform a temperature scan over a range of temperatures.
 * @param args Struct containing simulation parameters.
 */
void temperature_scan(Args &args);

#endif