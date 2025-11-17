#ifndef __mcmc_hpp__
#define __mcmc_hpp__

#include <random>
#include <fstream>
#include <array>
#include "lattice.hpp"
#include "stats.hpp"


/**
 * @brief Struct to hold MCMC simulation results.
 */
struct MCMC_data
{
    // Averages and variances of main observables
    double avg_energy = 0.0;
    double avg_energy_squared = 0.0;
    double var_energy = 0.0;

    double avg_magnetization = 0.0;
    double avg_magnetization_abs = 0.0;
    double avg_magnetization_squared = 0.0;
    double var_magnetization_abs = 0.0;

    // Derived quantities
    double heat_capacity = 0.0;
    double susceptibility = 0.0;

    // Error estimates
    double MCSE_energy = 0.0;
    double MCSE_magnetization = 0.0;
    double MCSE_magnetization_abs = 0.0;
    
    // Effective sample size estimates
    double ESS_energy = 0.0;
    double ESS_magnetization = 0.0;
    double ESS_magnetization_abs = 0.0;
};

/**
 * @brief Class representing a Markov Chain Monte Carlo (MCMC) solver for the Lattice class.
 * 
 * @param lattice: Reference to the Lattice object to perform MCMC on.
 * @param generator: Mersenne Twister random number generator.
 * @param idx_distr: Uniform integer distribution for selecting lattice indices.
 * @param spin_distr: Uniform real distribution for spin flip decisions.
 */
class MCMC
{
private:
    Lattice &lattice;
    std::mt19937 generator;
    std::uniform_int_distribution<int> idx_distr;
    std::uniform_real_distribution<double> spin_distr;
    
    int total_cycles;
    int steps_per_cycle;
    int burn_in_cycles = 0;

    arma::mat simulation_data;

    std::array<double, 5> boltzmann;

public:

    MCMC_data results;

    MCMC(Lattice &lattice, int seed);

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
    void run(int total_cycles, int steps_per_cycle, int burn_in_cycles);

    /**
     * @brief Process the collected simulation data to compute averages, variances, errors and derived quantities.
     */
    void process_results();

    /**
     * @brief Save `simulation_data` to a csv file and `results` to a json file
     * in the specified directory `out_dir`.
     *
     * @param out_dir name of the directory to save results to.
     */
    void save_data(const std::string &out_dir) const;

};

#endif