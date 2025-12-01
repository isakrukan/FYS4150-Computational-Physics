#include "Solver.hpp"
#include "utils.hpp"
#include <stdexcept>
#include <fstream>
#include <chrono>
#include <iomanip>


//********** Constructor(s) **********//

Solver::Solver(double h, double dt, std::string solvertype) : h(h), dt(dt){

    if (solvertype == "CrankNicolsen"){
        evolverAlgorithm = std::make_unique<CrankNicolsen>(h, dt);
    }

    else{
        throw std::logic_error(solvertype + " is not recognized as an (implemented) algorithm.");
    }

    M = static_cast<int>(std::llround(1.0 / h) + 1);  // Rounding 1/h + 1
    evolverAlgorithm_set = true;
}


//********** Public methods **********//

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
    t = arma::regspace(0.0, dt, T);
    N = static_cast<int>(t.n_elem);

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

    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Starting " << N << "-time points simulation of " << M << "x" << M << " grid" << std::endl;; 
    
    int step = (N-1)/100; // Dividing up the following loop, as to print progress
    int remainder = (N-1) % 100;
    if (step < 1){ // I.e. (N-1) < 100
        for (int n=0; n<N-1; n++){
        u_internal[n+1] = evolverAlgorithm->update_u(u_internal[n]);
    }
    }

    else {
        std::chrono::duration<double> elapsed; 
        for (int n=0; n<N-1; n+=step){
            for (int k=n; k<n+step && k<N - 1; k++){
                u_internal[k+1] = evolverAlgorithm->update_u(u_internal[k]);
            }

            // Progress update:
            auto end = std::chrono::high_resolution_clock::now();
            elapsed = end - start;
            double percent = 100.0 * n / (N - 1);
            std::cout << "\r" // Starting from start of line
                    << std::fixed << std::setprecision(2)
                        << percent << "% complete, time elapsed: "
                        << elapsed.count() << " seconds"
                        << std::flush;
        }

        // 100% complete:
        std::cout << std::fixed << "100.00% complete, time elapsed: "
                        << elapsed.count() << " seconds" << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Simulation completed in " << elapsed.count() << " seconds\n";

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
    tmp.save(arma::hdf5_name(out_dir + "data.h5", "M"));
    tmp(0) = N;
    tmp.save(arma::hdf5_name(out_dir + "data.h5", "N", arma::hdf5_opts::append));
    tmp(0) = h;
    tmp.save(arma::hdf5_name(out_dir + "data.h5", "h", arma::hdf5_opts::append));
    tmp(0) = dt;
    tmp.save(arma::hdf5_name(out_dir + "data.h5", "dt", arma::hdf5_opts::append));
    tmp(0) = slitExperiment.v0;
    tmp.save(arma::hdf5_name(out_dir + "data.h5", "v0", arma::hdf5_opts::append));

    // Storing vecors/matrices:
    t.save(arma::hdf5_name(out_dir + "data.h5", "t", arma::hdf5_opts::append));
    arma::cx_mat U(N, u[0].size());  // or arma::mat if real

    for (int n = 0; n < N; ++n) {
        U.row(n) = u[n].st();        // assuming u[n] is a column vector
    }

    U.save(arma::hdf5_name(out_dir + "data.h5", "u", arma::hdf5_opts::append));
}
