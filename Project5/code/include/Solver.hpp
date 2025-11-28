
#pragma once 

#include <string>
#include "SlitExperiment.hpp"
#include "EvolverAlgorithm.hpp"
#include <armadillo>

/**
 * @brief Class to evolve wave packet for single or double slit experiment, see \ref SlitExperiment "SlitExperiment". 
 * 
 */
class Solver{


friend class SlitExperiment;
// friend class CrankNicolsen;

private:

std::unique_ptr<EvolverAlgorithm> evolverAlgorithm; ///< Instance of evolverAlgorithm, as instance of inherited class.
SlitExperiment slitExperiment;

bool evolverAlgorithm_set = false;
bool slitExperiment_set = false;


public:

arma::cx_mat V; ///< Potential.
unsigned int M; ///< Space points.
unsigned int N; ///< Time points. 
double T; ///< Simulation duration.
arma::vec t; ///< Time points. 

std::vector<arma::cx_mat> u; ///< Wave function in time and space, u(time index) = (x-points, y-points).
std::vector<arma::cx_mat> u_internal; ///< Interior points of \ref u "u".

Solver(std::string solvertype="CrankNicolsen", double h=0.005, double Delta_t=2.5*1e-5);

void add_SlitExperiment(SlitExperiment slitExperiment);

void evolve(unsigned int N, double T);

};