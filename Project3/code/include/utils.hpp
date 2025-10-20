#ifndef __utils_hpp__
#define __utils_hpp__

#include <armadillo>
#include "penningTrap.hpp"
#include "solver.hpp"
#include "arg_parser.hpp"

bool check_exists(const arma::mat &data, double omega);

int single_run_problem9(double f, double omega, double d, const Args &args);

#endif