
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

public:

//*** Public variables */

arma::cx_mat V; ///< Potential.
unsigned int M; ///< Space points.
unsigned int N; ///< Time points. 
double h; ///< Space separation.
double dt; ///< Time separation.
double T; ///< Simulation duration.
arma::vec t; ///< Time points. 

std::vector<arma::cx_vec> u; ///< Wave function in time and space, u(time index) = (x-points, y-points).

//********** Constructor(s) **********//

/**
 * @brief Construct a new Solver object.
 * 
 * @param h Space separation.
 * @param dt Time separation.
 * @param solvertype Type of solver to use, defaults to "CrankNicolsen".
 */
Solver(double h=0.005, double dt=2.5*1e-5, std::string solvertype="CrankNicolsen");

//********** Public methods **********//

/**
 * @brief Adds slit experiment to solver.
 * 
 * @param slitExperiment Instance of \ref SlitExperiment "SlitExperiment".
 */
void add_SlitExperiment(SlitExperiment &slitExperiment);

/**
 * @brief Evolves the wave function over time T.
 * 
 * @param T Total time to evolve wave function.
 */
void evolve(double T);

/**
 * @brief Saves the wave function and potential to output directory (as a .hd5 file).
 * 
 * @param out_dir Output directory.
 */
void save(const std::string &out_dir) const;

private:

//*** Private variables */

std::unique_ptr<EvolverAlgorithm> evolverAlgorithm; ///< Instance of evolverAlgorithm, as instance of inherited class.
SlitExperiment slitExperiment;  ///< Instance of slitExperiment.

bool evolverAlgorithm_set = false; ///< Whether instance of \ref evolverAlgorithm "evolverAlgorithm" has been provided.
bool slitExperiment_set = false; ///< Whether instance of \ref slitExperiment "slitExperiment" has been provided.

std::vector<arma::cx_vec> u_internal; ///< Interior points of \ref u "u".

};