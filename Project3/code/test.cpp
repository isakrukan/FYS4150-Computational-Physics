#include <H5Cpp.h>
#include <armadillo>
#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>

// Assuming you already have: Args, PenningTrap, Solver, etc.

struct Entry
{
    double w;
    int n;
};

int single_run(double f, double omega, double d, const Args &args);
{
    arma::arma_rng::set_seed(args.seed);

    double V_0 = 2.41e6;
    auto V = [V_0, f, omega](double t)
    { return V_0 * (1 + f * std::cos(omega * t)); };

    PenningTrap trap(V, 9.65e1, d);

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

void problem9(const Args &args)
{

    double f = args.amplitude;
    double d = 500;

    std::string filename = "p" + std::to_string(args.n_particles) + "f" + std::to_string(f) + "-RK4-int-n" + std::to_string(escape_count) + ".h5"

    if (arma::load(data, filename, "escape_data"))
    {
        // okay, loaded existing data
    }
    else
    {
        data.set_size(2, 0); // 2 rows, 0 cols initially
    }

    for (double omega : args.omega_values)
    {
        bool exists = check_exists(data, omega);

        int escape_count = single_run(f, omega, d, args);

        if (exists)
        {
            std::cout << "Skipping ω=" << omega << " (already in dataset)\n";
            continue;
        }
        else
        {
            std::cout << "Running ω=" << omega << "\n";
            arma::vec new_col = {omega, static_cast<double>(escape_count)};
            data.insert_cols(data.n_cols, new_col);

        }
    }

    // Sort columns by frequency (row 0)
    arma::uvec sort_idx = arma::sort_index(data.row(0));
    data = data.cols(sort_idx);

    // Save data back to file
    arma::hdf5_name data_hdf5 = arma::hdf5_name(filename, "escape_data", arma::hdf5_opts::replace);
    data.save(data_hdf5, arma::hdf5_binary);

}


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
