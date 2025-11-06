#ifndef __utils_hpp__
#define __utils_hpp__

#include <iostream>
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

std::string generate_filename(const Args &args, const std::string &OUTDIR = "out/");

/**
 * @brief Perform a temperature scan over a range of temperatures.
 * @param args Struct containing simulation parameters.
 */
void temperature_scan(Args &args, const std::string &OUTDIR = "out/");

#endif