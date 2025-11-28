#include "SlitExperiment.hpp"
#include "utils.hpp"
#include <cmath>

SlitExperiment::SlitExperiment(unsigned int M) : M(M) {
    
    h = double(1/(M-1));
    u_initial = arma::cx_vec(M*M, arma::fill::zeros);

    // Grid and time points:
    x = arma::linspace(0,1,M);
    y = arma::linspace(0,1,M);
    V.zeros(M, M);  
};

SlitExperiment::SlitExperiment(double h){
    
    M = int(1/h + 1);
    h = double(1/(M-1));
    u_initial = arma::cx_vec(M*M, arma::fill::zeros);

    // Grid and time points:
    x = arma::linspace(0,1,M);
    y = arma::linspace(0,1,M);
    V.zeros(M, M);  
};

void SlitExperiment::add_wave_packet(std::vector<double> xy_c, std::vector<double> sigma_xy, std::vector<double> p_xy){
    double normalization = 0;
    for (int i=1; i<M-1; i++){
        for (int j=1; j<M-1; j++){
            double real_part(-(x[i]-xy_c[0])*(x[i]-xy_c[0]) / (2.0 * sigma_xy[0]*sigma_xy[0]) \
                             -(y[j]-xy_c[1])*(y[j]-xy_c[1]) / (2.0 * sigma_xy[1]*sigma_xy[1]));
            double imag_part = p_xy[0]*x[i] + p_xy[1]*y[j];                                      
            auto envelope = std::exp(real_part);
            arma::cx_double phase(std::cos(imag_part), std::sin(imag_part));
            arma::cx_double u_tmp = envelope * phase;
   

            int k = get_flat_index(i,j,M);
            u_initial(k) = u_tmp;
            normalization += std::norm(u_tmp);
        }
    }
    u_initial /= std::sqrt(normalization);
}

void SlitExperiment::add_double_slit(double v0, double cx, double wt, double d, double dy) : v0(v0) {

    double y_center = 0.5;

    for (int i=0; i<M; i++){
        for (int j=0; j<M; j++){
            
            // Wall is centered at cx and has a thickness wt
            if (std::abs(x[i] - cx) <= wt/2.0){
                
                // Between the two slits || outside the two slits
                if ((std::abs(y[j] - y_center) <= d/2.0) || (std::abs(y[j] - y_center) > d/2.0 + dy/2.0)){
                    V(i,j) = v0;
                }
            }
        }
    }
}