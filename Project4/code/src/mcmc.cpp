#include "mcmc.hpp"
#include "utils.hpp"

MCMC::MCMC(Lattice &lattice, int total_cycles, int steps_per_cycle, int seed)
    : lattice(lattice), total_cycles(total_cycles), steps_per_cycle(steps_per_cycle)
    {
    if (seed == 0)
    {
        seed = std::random_device{}();
    }
    // Initialize random number generators
    generator.seed(seed);
    idx_distr = std::uniform_int_distribution<int>(0, lattice.L - 1);
    spin_distr = std::uniform_real_distribution<double>(0.0, 1.0);

    // Precompute Boltzmann factors for MCMC cycles
    for (double diff : {-8, -4, 0, 4, 8})
    {
        int idx = (diff + 8) / 4;
        boltzmann[idx] = std::exp(-lattice.J * diff / lattice.T);
    }
}

void MCMC::set_burn_in(int no_cycles)
{ 
    burn_in_cycles = no_cycles; 
}

void MCMC::cycle_MCMC(int steps)
{

    double diff;
    double random_number;

    int nrows = lattice.rows;
    int ncols = lattice.cols;

    for (int step = 0; step < steps; ++step)
    {
        int i = idx_distr(generator);
        int j = idx_distr(generator);

        int spin = lattice.get_spin(i, j);
        int up = lattice.get_spin(pbc(i - 1, nrows), j);
        int down = lattice.get_spin(pbc(i + 1, nrows), j);
        int left = lattice.get_spin(i, pbc(j - 1, ncols));
        int right = lattice.get_spin(i, pbc(j + 1, ncols));

        diff = 2 * spin * (up + down + left + right);

        if (spin_distr(generator) < boltzmann[(diff + 8) / 4])
        {
            lattice.flip_spin(i, j);
        }
    }
}

void MCMC::run(int total_cycles, int steps_per_cycle, const std::string &filename)
{

    bool save = !filename.empty();
    std::ofstream outfile(filename);

    if (save)
    {
        outfile << "cycle, E, <E>, M, <M>\n";
    }

    // Burn-in cycles
    for (int cycle = 0; cycle < burn_in_cycles; ++cycle)
    {
        cycle_MCMC(steps_per_cycle);
    }

    // Perform main MCMC cycles
    for (int cycle = 1; cycle < total_cycles; ++cycle)
    {
        cycle_MCMC(steps_per_cycle);

        double E_now = lattice.get_energy();
        double M_now = lattice.get_magnetization();

        results.avg_energy += E_now;
        results.avg_energy_squared += E_now * E_now;
        results.avg_magnetization += M_now;
        results.avg_magnetization_abs += std::abs(M_now);
        results.avg_magnetization_squared += M_now * M_now;

        if (save)
        {
            outfile << cycle << ", "
                    << E_now << ", " << results.avg_energy / cycle << ", "
                    << M_now << ", " << results.avg_magnetization / cycle << "\n";
        }
    }
    
    // Finalize averages and compute variances
    results.avg_energy /= total_cycles;
    results.avg_energy_squared /= total_cycles;
    results.avg_magnetization /= total_cycles;
    results.avg_magnetization_abs /= total_cycles;
    results.avg_magnetization_squared /= total_cycles;

    results.var_energy = results.avg_energy_squared - results.avg_energy * results.avg_energy;
    results.var_magnetization = results.avg_magnetization_squared - results.avg_magnetization_abs * results.avg_magnetization_abs;

    // Compute heat capacity and susceptibility
    results.heat_capacity = results.var_energy / (lattice.T * lattice.T);
    results.susceptibility = results.var_magnetization / lattice.T;

}
