#ifndef __utils_hpp__
#define __utils_hpp__

#include "arg_parser.hpp"
#include <armadillo>

const arma::cx_double I(0.0, 1.0);
const double V_0 = 1e14;

arma::cx_double gaussian_wave_packet(double x, double y, double x_c, double y_c, double sigma_x, double sigma_y, double p_x, double p_y);

arma::cx_double gaussian_wave_packet(double x, double y, Args args);

#endif