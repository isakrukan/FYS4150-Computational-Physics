#include "utils.hpp"

int pbc(int x, int L)
{
    return x - L * (x >= L) + L * (x < 0);
}

std::string generate_temp_scan_name(const Args &args)
{
    std::ostringstream oss;
    oss << std::setprecision(0);

    oss << "temp_scan"
        << "-n" << args.temperature_steps 
        << "-L" << args.lattice_size;
    // oss << "L"              << args.lattice_size 
    //    << "-T"             << args.temperature_steps 
    //    << std::scientific
    //    << "-cyc"           << static_cast<double>(args.total_cycles)
    //    << "-n"             << static_cast<double>(args.steps_per_cycle)
    //    << "-brn"           << static_cast<double>(args.burn_in_cycles);

    std::string name = oss.str();
    //name.erase(std::remove(name.begin(), name.end(), '+'), name.end());

    return name;
}

std::string generate_run_name(const Args &args)
{
    // 1) Format temperature: precision 1 & replace '.' with 'p'
    std::ostringstream oss;
    oss << "MCMC" 
        << (args.ordered ? "-ordered" : "-unordered")
        << "-L" << args.lattice_size
        << std::fixed << std::setprecision(1)
        << "-T" << args.temperature;

    std::string name = oss.str();
    std::replace(name.begin(), name.end(), '.', 'p');
    
    return name;
}

void temperature_scan(Args &args)
{
    double T_min = args.temperature_min;
    double T_max = args.temperature_max;
    int T_steps = args.temperature_steps;

    // Prepare to save data if flag is set (default: true)
    arma::colvec T = arma::linspace(T_min, T_max, T_steps + 1);
    arma::mat results = arma::zeros(T_steps + 1, 9);
        
    // Check for OpenMP compiler flags
    #ifdef _OPENMP
    #define PARALLEL_FOR _Pragma("omp parallel for")
    #else
    #define PARALLEL_FOR
    #endif
    
    // Run temperature scan, and record time
    auto start = std::chrono::high_resolution_clock::now();
    
    PARALLEL_FOR
    for (int i = 0; i <= T_steps; ++i)
    {
        Lattice lattice(args.lattice_size, args.coupling, T(i));
        lattice.initialize_spins(args.ordered);

        MCMC solver(lattice, args.seed + i*100);
        solver.run(args.total_cycles, args.steps_per_cycle, args.burn_in_cycles);

        // Store output if save flag is set (default: true)
        if (args.save_data)
        {
            results(i, 0) = T(i);
            results(i, 1) = solver.results.avg_energy / lattice.N;
            results(i, 2) = solver.results.var_energy / (lattice.N * lattice.N);
            results(i, 3) = solver.results.avg_magnetization_abs / lattice.N;
            results(i, 4) = solver.results.var_magnetization_abs / (lattice.N * lattice.N);

            results(i, 5) = solver.results.MCSE_energy / lattice.N;
            results(i, 6) = solver.results.ESS_energy;
            results(i, 7) = solver.results.MCSE_magnetization_abs / lattice.N;
            results(i, 8) = solver.results.ESS_magnetization_abs;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Temperature scan completed in " << elapsed.count() << " seconds.\n";
    
    // Save data if flag is set (default: true)
    if (args.save_data)
    {
        results.save(args.out_dir + args.run_name + ".csv", arma::csv_ascii);
    }
    
    
}
