
#pragma once 

#include <string>
#include "SlitExperiment.hpp"
#include "EvolverAlgorithm.hpp"
#include <armadillo>
#include <memory>
#include <vector>

/**
 * @brief Class to evolve wave packet for single or double slit experiment, see \ref SlitExperiment "SlitExperiment". 
 * 
 */
class Solver{


friend class SlitExperiment;
friend class EvolverAlgorithm;

private:

std::unique_ptr<EvolverAlgorithm> evolverAlgorithm; ///< Instance of evolverAlgorithm, as instance of inherited class.
SlitExperiment slitExperiment;

bool evolverAlgorithm_set = false;
bool slitExperiment_set = false;

std::vector<arma::cx_vec> u_internal; ///< Interior points of \ref u "u".

public:

arma::cx_mat V; ///< Potential.
unsigned int M; ///< Space points.
unsigned int N; ///< Time points. 
double h; ///< Space separation.
double Delta_t; ///< Time separation.
double T; ///< Simulation duration.
arma::vec t; ///< Time points. 

std::vector<arma::cx_vec> u; ///< Wave function in time and space, u(time index) = (x-points, y-points).

Solver(double h=0.005, double Delta_t=2.5*1e-5, std::string solvertype="CrankNicolsen");

void add_SlitExperiment(SlitExperiment &slitExperiment);

void evolve(double T);

void save(const std::string &out_dir) const;
};