#include "lattice.hpp"
#include "utils.hpp"

Lattice::Lattice(int L, double J, double temp) : L(L), rows(L), cols(L), temperature(temp), J(J)
{
    // Initialize random number generators
    generator.seed(std::random_device{}());
    idx_distr  = std::uniform_int_distribution<int>(0, L - 1);
    spin_distr = std::uniform_real_distribution<double>(0.0, 1.0);

    // Precompute Boltzmann factors for MCMC cycles
    for (double diff : {-8, -4, 0, 4, 8})
    {
        boltzmann_diff[diff] = std::exp(-J * diff / temperature);
    }
}

void Lattice::initialize_spins(bool ordered)
{
    if (ordered)
    {
        grid = arma::imat(rows, cols);
        grid.fill(1);
    }
    else
    {
        grid = arma::randi<arma::imat>(rows, cols, arma::distr_param(0, 1));
        grid.replace(0, -1);
    }

    compute_energy();
    compute_magnetization();
}

void Lattice::compute_energy()
{
    E = 0.0;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            int spin  = grid(i, j);
            int right = grid(i, mod(j + 1, cols));
            int below = grid(mod(i + 1, rows), j);

            E += -J * spin * (right + below);
        }
    }
}

void Lattice::compute_magnetization()
{
    M = arma::accu(grid);
}

void Lattice::cycle_MCMC(int steps)
{

    double diff;
    double random_number;
    
    for (int step = 0; step < steps; ++step)
    {
        int i = idx_distr(generator);
        int j = idx_distr(generator);

        int spin  = grid(i, j);
        int above = grid(mod(i - 1, rows), j);
        int below = grid(mod(i + 1, rows), j);
        int left  = grid(i, mod(j - 1, cols));
        int right = grid(i, mod(j + 1, cols));

        diff = 2 * spin * (above + below + left + right);

        if (spin_distr(generator) < boltzmann_diff[diff])
        {
            grid(i, j) *= -1;
            E += J * diff;
            M += 2 * spin;
        }

    }
}

void Lattice::run_MCMC(int total_cycles, int steps_per_cycle, const std::string &filename)
{

    bool save = !filename.empty();
    std::ofstream outfile(filename);

    if (save)
    {
        outfile << "cycle, E, <E>, M, <M>\n";
    }

    double E_avg = 0.0;
    double E2_avg = 0.0;
    double M_avg = 0.0;
    double M2_avg = 0.0;

    for (int cycle = 1; cycle < total_cycles; ++cycle)
    {
        cycle_MCMC(steps_per_cycle);

        E_avg  += E;
        E2_avg += E * E;
        M_avg  += M;
        M2_avg += M * M;

        if (save)
        {
            outfile << cycle << ", " 
                    << E << ", " << E_avg / cycle << ", " 
                    << M << ", " << M_avg / cycle << "\n";
        }
    }

    double divisor = total_cycles * N;
    double eps_avg  = E_avg / divisor;
    double eps2_avg = E2_avg / divisor;
    double m_avg    = M_avg / divisor;
    double m2_avg   = M2_avg / divisor;

    double eps_var = eps2_avg - eps_avg * eps_avg;
    double m_var   = m2_avg - m_avg * m_avg;
    
    double heat_capacity  = N * eps_var / (temperature * temperature);
    double susceptibility = N * m_var   / temperature;

    /**
    std::cout << "Energy (avg): " << eps_avg << std::endl;
    std::cout << "Energy (var): " << eps_var << std::endl;
    std::cout << "Magnetization (avg): " << m_avg << std::endl;
    std::cout << "Magnetization (var): " << m_var << std::endl;

    std::cout << "Heat capacity (specific): " << heat_capacity << std::endl;
    std::cout << "Susceptibility (specific): " << susceptibility << std::endl;
    
    outfile.close();
    */
}
