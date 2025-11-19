#include "mcmc.hpp"
#include "utils.hpp"

MCMC::MCMC(Lattice &lattice_, int seed)
    : lattice(lattice_)
    {
    if (seed == 0)
    {
        seed = std::random_device{}();
    }

    // Initialize random number generators using the uniform distribution as proposal distribution. 
    generator.seed(seed);

    // NOTE: 
    // Changed to generating idx ∈ [0, N-1] to avoid multiple RNG calls per step
    idx_distr = std::uniform_int_distribution<int>(0, lattice.N - 1);
    spin_distr = std::uniform_real_distribution<double>(0.0, 1.0);

    // Precompute Boltzmann factors for MCMC cycles
    for (double diff_initialize : {-8, -4, 0, 4, 8})
    {
        int idx = (diff_initialize + 8) / 4;
        boltzmann[idx] = std::exp(-lattice.J * diff_initialize / lattice.T);
    }
}


void MCMC::cycle_MCMC(int steps)
{

    for (int step = 0; step < steps; step++)
    {
        randint = idx_distr(generator);             // Random index [0, ..., N-1] of flattened lattice
        i_cycle_location = randint / lattice.L;     // Convert to 2D indices
        j_cycle_location = randint % lattice.L;     // Convert to 2D indices

        Location &location = lattice[i_cycle_location][j_cycle_location];

        diff = 2 * location.s * ((*location.up).s +
                                 (*location.down).s +
                                 (*location.left).s +
                                 (*location.right).s);

        // Always accept if ΔE is negative (avoids unnecessary RNG calls)
        if (diff <= 0)
        {
            lattice.flip_spin(i_cycle_location, j_cycle_location);
            continue;
        }
        // Otherwise, do RNG call for acceptance rule
        if (spin_distr(generator) < boltzmann[(diff + 8) / 4])
        {
            lattice.flip_spin(i_cycle_location, j_cycle_location); // Energy and magnetization is updated within lattice.
        }
    }
}

void MCMC::run(int total_cycles, int steps_per_cycle, int burn_in_cycles)
{

    // Burn-in cycles
    for (int cycle = 0; cycle < burn_in_cycles; cycle++)
    {
        cycle_MCMC(steps_per_cycle);
    }

    
    // Initialize storage for simulation data
    simulation_data.set_size(total_cycles, 3); // of the form (#cycles, energy, magnetization)
    simulation_data.zeros();
    
    simulation_data(0, 0) = 1; // Number of cycles
    simulation_data(0, 1) = lattice.get_energy(); // Energy
    simulation_data(0, 2) = lattice.get_magnetization(); // Magnetization
    
    // Perform main MCMC cycles
    for (int cycle = 1; cycle < total_cycles; cycle++)
    {
        cycle_MCMC(steps_per_cycle);

        simulation_data(cycle, 0) = cycle + 1;
        simulation_data(cycle, 1) = lattice.get_energy();
        simulation_data(cycle, 2) = lattice.get_magnetization();

    }
    
    // Compute final averages, variances, etc.
    process_results();
}


void MCMC::process_results()
{
    int total_cycles = simulation_data.n_rows;

    arma::vec cycle_numbers = simulation_data.col(0);
    arma::vec energies = simulation_data.col(1);
    arma::vec magnetizations = simulation_data.col(2);
    arma::vec magnetizations_abs = arma::abs(magnetizations);

    // Main observables - averages & variances:
    results.avg_energy = arma::mean(energies);
    results.avg_energy_squared = arma::mean(energies % energies);
    results.avg_magnetization = arma::mean(magnetizations);
    results.avg_magnetization_abs = arma::mean(magnetizations_abs);
    results.avg_magnetization_squared = arma::mean(magnetizations % magnetizations);

    results.var_energy = results.avg_energy_squared - results.avg_energy * results.avg_energy;
    results.var_magnetization_abs = results.avg_magnetization_squared - results.avg_magnetization_abs * results.avg_magnetization_abs;

    // Derived quantities
    results.heat_capacity = results.var_energy / (lattice.T * lattice.T);
    results.susceptibility = results.var_magnetization_abs / lattice.T;
    // MCSE errors & effective samples accounting for autocorrelation:
    int max_lag = std::min(total_cycles / 2, 5000);
    
    results.MCSE_energy = standard_error(energies);
    results.ESS_energy = n_eff(energies, max_lag);
    
    results.MCSE_magnetization = standard_error(magnetizations);
    results.MCSE_magnetization_abs = standard_error(magnetizations_abs);
    results.ESS_magnetization = n_eff(magnetizations, max_lag);
    results.ESS_magnetization_abs = n_eff(magnetizations_abs, max_lag);
}

void MCMC::save_data(const std::string &out_dir) const
{
    simulation_data.save(out_dir + "raw_data.csv", arma::csv_ascii);
    std::ofstream stats_file(out_dir + "stats.json");

    stats_file << "{\n";
    stats_file << "  \"avg_energy\": "                << results.avg_energy                 << ",\n";
    stats_file << "  \"avg_energy_squared\": "        << results.avg_energy_squared         << ",\n";
    stats_file << "  \"var_energy\": "                << results.var_energy                 << ",\n";
    stats_file << "  \"avg_magnetization\": "         << results.avg_magnetization          << ",\n";
    stats_file << "  \"avg_magnetization_abs\": "     << results.avg_magnetization_abs      << ",\n";
    stats_file << "  \"avg_magnetization_squared\": " << results.avg_magnetization_squared  << ",\n";
    stats_file << "  \"var_magnetization_abs\": "     << results.var_magnetization_abs      << ",\n";
    stats_file << "  \"MCSE_energy\": "               << results.MCSE_energy                << ",\n";
    stats_file << "  \"MCSE_magnetization\": "        << results.MCSE_magnetization         << ",\n";
    stats_file << "  \"MCSE_magnetization_abs\": "    << results.MCSE_magnetization_abs     << ",\n";
    stats_file << "  \"heat_capacity\": "             << results.heat_capacity              << ",\n";
    stats_file << "  \"susceptibility\": "            << results.susceptibility             << "\n";
    stats_file << "}\n";

    stats_file.close();
}