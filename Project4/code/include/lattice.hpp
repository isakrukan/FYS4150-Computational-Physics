
#ifndef __lattice_hpp__
#define __lattice_hpp__

#include <armadillo>
#include <cmath>
#include <random>
#include <map>
#include <iostream>
#include <fstream>

/**
 * @brief Class representing a 2D lattice for MCMC simulations.
 *
 * @param L: Size of the lattice (L x L).
 * @param temperature: Temperature of the system, in units of 1/kB.
 * @param grid: 2D Armadillo matrix representing the lattice spins.
 * @param J: Interaction strength between neighboring spins.
 * @param generator: Mersenne Twister random number generator.
 * @param idx_distr: Uniform integer distribution for selecting lattice indices.
 * @param spin_distr: Uniform real distribution for spin flip decisions.
 */
class Lattice
{

private:
    int rows;
    int cols;
    int L;
    int N = L * L;
    arma::imat grid;
    double temperature;
    double J;
    double E;
    double M;

    std::map <int, double> boltzmann_diff;

    std::mt19937 generator;
    std::uniform_int_distribution<int> idx_distr;
    std::uniform_real_distribution<double> spin_distr;

    /**
     * @brief Compute the total energy of the lattice.
     */
    void compute_energy();

    /**
     * @brief Compute the total magnetization of the lattice.
     */
    void compute_magnetization();

public:
    Lattice(int L, double J, double temp);

    void initialize_spins(bool ordered=false);

    /**
     * @brief Perform a single MCMC cycle on the lattice.
     * 
     * @param steps Number of steps to use.
     */
    void cycle_MCMC(int steps);

    /**
     * @brief Run a full MCMC simulation on the lattice.
     * 
     * @param total_cycles Number of total cycles to run.
     * @param steps_per_cycle Number of steps per cycle.
     */
    void run_MCMC(int total_cycles, int steps_per_cycle, const std::string &filename = "");
};

#endif