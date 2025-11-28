#include "SlitExperiment.hpp"



SlitExperiment::SlitExperiment(unsigned int M) :M(M) {
    
    u_initial = arma::cx_mat(M, M, arma::fill::zeros);

    // Grid and time points:
    x = arma::linspace(0,1,M);
    y = arma::linspace(0,1,M);
};

void SlitExperiment::add_wave_packet(std::vector<double> xy_c, std::vector<double> sigma_xy, std::vector<double> p_xy){
    double normalization = 0;
    for (int i=1; i<M-1; i++){
        for (int j=1; j<M-1; j++){
            double real_part(-(x[i]-xy_c[0])*(x[i]-xy_c[0]) / (2.0 * sigma_xy[0]*sigma_xy[0]) - \
                             -(y[j]-xy_c[1])*(y[j]-xy_c[1]) / (2.0 * sigma_xy[1]*sigma_xy[1]));
            double imag_part = p_xy[0]*x[i] + p_xy[1]*y[j];                                      
            arma::cx_double u_tmp(std::exp(real_part) + std::cos(imag_part), std::sin(imag_part));     

            u_initial(i,j) = u_tmp;
            normalization += std::abs(u_tmp);
        }
    }
    u_initial /= normalization;
}