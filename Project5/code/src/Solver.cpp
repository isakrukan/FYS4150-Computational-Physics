#include "Solver.hpp"
#include "utils.hpp"
#include <stdexcept>
#include <fstream>

Solver::Solver(double h, double Delta_t, std::string solvertype){

    if (solvertype == "CrankNicolsen"){
        evolverAlgorithm = std::make_unique<CrankNicolsen>(h, Delta_t);
    }

    else{
        throw std::logic_error(solvertype + " is not recognized as an (implemented) algorithm.");
    }

    M = int(1.0/h + 1.0);
    N = int(1.0/Delta_t + 1.0);
    h = double(1.0/(M-1));
    Delta_t = double(1.0/(N-1));
    evolverAlgorithm_set = true;
}


void Solver::add_SlitExperiment(SlitExperiment &slitExperiment_){

    slitExperiment = slitExperiment_;
    M = slitExperiment.M;
    V = slitExperiment.V;
    evolverAlgorithm->set_grid_size(M);
    evolverAlgorithm->set_potential(V);

    slitExperiment_set = true;
}

void Solver::evolve(double T_){

    if (!evolverAlgorithm_set || !slitExperiment_set){
        throw std::logic_error("Missing instance of either SlitExperiment or EvolerAlgorithm");
    }

    T = T_;

    t = arma::linspace(0,T,N);
    u.assign(N, arma::cx_vec(M*M, arma::fill::zeros));
    u_internal.assign(N, arma::cx_vec((M-2)*(M-2), arma::fill::zeros));
    u[0] = slitExperiment.u_initial;

    // Assigning initial values to internal points:
    for (int i=1; i<M-1; i++){
        for (int j=1; j<M-1; j++){
            int k_internal = get_flat_index(i-1, j-1, M-2);
            int k = get_flat_index(i, j, M);
            u_internal[0](k_internal) = u[0](k);
        }
    }

    for (int n=0; n<N-1; n++){
        u_internal[n+1] = evolverAlgorithm->update_u(u_internal[n]);
    }

    // Assigning values from internal values to u
    for (int n = 1; n < N; ++n) {
        for (int i = 1; i < M-1; ++i) {
            for (int j = 1; j < M-1; ++j) {
                int k_internal = get_flat_index(i-1, j-1, M-2);
                int k         = get_flat_index(i,   j,   M);
                u[n](k) = u_internal[n](k_internal);   // <-- swapped
            }
        }
    }

}


void Solver::save(const std::string &out_dir) const{

    // Storing parameters:
    arma::vec tmp(1);
    tmp(0) = M;
    tmp.save(arma::hdf5_name(out_dir + "data.h5", "M"), arma::hdf5_binary);
    tmp(0) = N;
    tmp.save(arma::hdf5_name(out_dir + "data.h5", "N"), arma::hdf5_binary);
    tmp(0) = h;
    tmp.save(arma::hdf5_name(out_dir + "data.h5", "h"), arma::hdf5_binary);
    tmp(0) = Delta_t;
    tmp.save(arma::hdf5_name(out_dir + "data.h5", "Delta_t"), arma::hdf5_binary);
    tmp(0) = slitExperiment.v0;
    tmp.save(arma::hdf5_name(out_dir + "data.h5", "v0"), arma::hdf5_binary);

    // Storing vecors/matrices:
    t.save(arma::hdf5_name(out_dir + "data.h5", "t"), arma::hdf5_binary);
    for (int n=0; n < N; n++) {
        u[n].save(arma::hdf5_name(out_dir + "data.h5", "u_" + std::to_string(n)),
                   arma::hdf5_binary);
    }
}
