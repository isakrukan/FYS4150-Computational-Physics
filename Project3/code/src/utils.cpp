#include "utils.hpp"

bool check_exists(const arma::mat &data, double omega)
{
    for (arma::uword i = 0; i < data.n_cols; ++i)
    {
        if (std::abs(data(0, i) - omega) < 1e-12)
        { // handle floating precision
            return true;
        }
    }
    return false;
}

int single_run_problem9(double f, double omega, double d, const Args &args)
{
    arma::arma_rng::set_seed(args.seed);

    double V_0 = 2.41e6;
    auto V = [V_0, f, omega](double t)
    { return V_0 * (1 + f * std::cos(omega * t)); };

    PenningTrap trap(V, 9.65e1, d);
    trap.interacting = args.interacting;

    for (int i = 0; i < args.n_particles; i++)
    {
        arma::vec r = arma::randn<arma::vec>(3) * 0.1 * d;
        arma::vec v = arma::randn<arma::vec>(3) * 0.1 * d;
        trap.add_particle(1, 40, r, v);
    }

    Solver solver(trap);
    solver.evolve(args.T, args.n_steps, "RK4");

    int escape_count = args.n_particles - trap.count_inside();

    return escape_count;
}