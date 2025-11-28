#include "utils.hpp"
#include "arg_parser.hpp"

arma::cx_double gaussian_wave_packet(double x, double y, double x_c, double y_c, double sigma_x, double sigma_y, double p_x, double p_y)
{
    double scaled_x = (x - x_c) / sigma_x;
    double scaled_y = (y - y_c) / sigma_y;

    arma::cx_double exponent = - scaled_x * scaled_x - scaled_y * scaled_y + I * (p_x * x + p_y * y);

    return std::exp(exponent);
}

arma::cx_double gaussian_wave_packet(double x, double y, Args args)
{
    double x_c = args.center_x;
    double y_c = args.center_y;
    double sigma_x = args.sigma_x;
    double sigma_y = args.sigma_y;
    double p_x = args.momentum_x;
    double p_y = args.momentum_y;

    return gaussian_wave_packet(x, y, x_c, y_c, sigma_x, sigma_y, p_x, p_y);
}


int get_flat_index(int i, int j, unsigned int M){
    return i*M + j;
}