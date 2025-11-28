#include "Solver.hpp"
#include <stdexcept>

Solver::Solver(std::string solvertype, double h, double Delta_t){
    t = arma::linspace(0,T,N);

    if (solvertype == "CrankNicolsen"){
        evolverAlgorithm = std::make_unique<CrankNicolsen>(h, Delta_t);
    }

    else{
        throw std::logic_error(solvertype + " is not recognized as a (implemented) algorithm.");
    }

    evolverAlgorithm_set = true;
}


void Solver::add_SlitExperiment(SlitExperiment slitExperiment_){

    slitExperiment = slitExperiment_;
    M = slitExperiment.M;
    V = slitExperiment.V;
    evolverAlgorithm->set_grid_size(M);
    evolverAlgorithm->set_potential(V);

}

void Solver::evolve(unsigned int N, double T){

    if (!evolverAlgorithm_set || !slitExperiment_set){
        throw std::logic_error("Missing instance of either SlitExperiment or EvolerAlgorithm");
    }

    u.resize(N, arma::cx_mat(M, M));
    u[0] = slitExperiment.u_initial;

    // u_internal as submatrix of u:
    for (int n=0; n<N; n++){
        arma::cx_mat &u_tmp = u[n];
        u_internal[n] = u_tmp.submat(1, 1, M-2, M-2);
    }

    for (int n=0; n<N; n++){
        u_internal[n+1] = evolverAlgorithm->update_u(n);
    }
}

