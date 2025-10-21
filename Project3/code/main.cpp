#include "penningTrap.hpp"
#include "solver.hpp"
#include "arg_parser.hpp"
#include <iostream>
#include "utils.hpp"
#include <iomanip>
#include <chrono>

void problem8(Args args){
    //// Problem 8
    arma::vec r1 = {20, 0, 20}; // [mu m]
    arma::vec v1 = {0, 25, 0}; // [mu m/mu s]
    arma::vec r2 = {25, 25, 0}; // [mu m]
    arma::vec v2 = {0, 40, 5}; // [mu m/mu s]

    /// Single particle 
    int T = 50; // [mu s]
    PenningTrap single_particle;
    single_particle.add_particle(1, 40, r1, v1);
    Solver single_solver(single_particle);
    single_solver.evolve(T, args.n_steps); // Uses RK4 by default
    single_solver.save();

    /// Two particles
    PenningTrap interacting_trap;
    interacting_trap.add_particle(1, 40, r1, v1);
    interacting_trap.add_particle(1, 40, r2, v2);
    Solver interacting_solver(interacting_trap);
    interacting_solver.evolve(T, args.n_steps); // Uses RK4 by default
    interacting_solver.save();

    PenningTrap non_interacting_trap;
    non_interacting_trap.add_particle(1, 40, r1, v1);
    non_interacting_trap.add_particle(1, 40, r2, v2);
    non_interacting_trap.interacting = false;
    Solver non_interacting_solver(non_interacting_trap);
    non_interacting_solver.evolve(T, args.n_steps); // Uses RK4 by default
    non_interacting_solver.save();
    
    // Single particle, various specific time steps n 
    std::vector<int> n = {4000, 8000, 16000, 32000};

    for (std::string method : {"RK4", "FE"}) 
    {
        for (int i=0; i<n.size(); i++){
            PenningTrap single_particle;
            single_particle.add_particle(1, 40, r1, v1);
            Solver single_solver_(single_particle);
            single_solver_.evolve(T, n[i], method); // Uses RK4 by default
            single_solver_.save();
        }
    }
}


void problem9(const Args &args)
{

    double f = args.amplitude;
    double d = 500;
    std::ostringstream f_str;
    f_str << std::fixed << std::setprecision(1) << f; // e.g. "0.10"

    // Filename for the created/loaded dataset
    std::string fclean = f_str.str();
    fclean.erase(std::remove(fclean.begin(), fclean.end(), '.'), fclean.end());
    std::string outdir = "out/";
    std::string info = "n_escaped-p" + std::to_string(args.n_particles) + "-f" + fclean + "-RK4";
    if (args.interacting) {
        info += "-int";
    }
    else {
        info += "-nonint";
    }
    info += "-n" + std::to_string(args.n_steps);

    std::string filename = outdir + info + ".h5";

    arma::mat data;

    // Try to load existing data. If fails, create empty dataset.
    try
    {
        arma::hdf5_name data_hdf5 = arma::hdf5_name(filename, "escape_data");
        data.load(data_hdf5);
    }
    catch (...)
    {
        data.set_size(2, 0); // 2 rows, 0 cols initially
    }



    // Compute frequencies not already in the dataset
    arma::vec omega_values = arma::linspace(args.freq_min, args.freq_max, args.n_freq);
    for (double omega : omega_values)
    {
        // Get current time for logging
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        auto time_str = std::put_time(std::localtime(&now_c), "%H:%M:%S");
        
        // Check if this omega already exists in the dataset
        bool exists = check_exists(data, omega);
        
        if (exists)
        {
            std::cout << "[" << time_str << "] Skipping omega=" << omega << " (already in dataset)" << std::endl;
            continue;
        }
        else
        {
            std::cout << "[" << time_str << "] Running omega=" << omega << std::endl;
            int escape_count = single_run_problem9(f, omega, d, args);
            arma::vec new_col = {omega, static_cast<double>(escape_count)};
            data.insert_cols(data.n_cols, new_col);
        }

        
    }


    // Sort dataset columns by the frequency values in column 0 
    arma::uvec sort_idx = arma::sort_index(data.row(0));
    data = data.cols(sort_idx);

    // Save data back to file
    arma::hdf5_name data_hdf5 = arma::hdf5_name(filename, "escape_data", arma::hdf5_opts::replace);
    data.save(data_hdf5, arma::hdf5_binary);
}

int main(int argc, char *argv[])
{
    Args args = parse_args(argc, argv);

    if (args.problem_8) {
        problem8(args);
    }
    if (args.problem_9) {
        problem9(args);
    }

    return 0;
}
