#include "utils.hpp"

int pbc(int x, int L)
{
    return x - L * (x >= L) + L * (x < 0);
}

std::string generate_temp_scan_name(const Args &args)
{
    std::ostringstream oss;
    oss << std::fixed << args.temperature;

    std::string T_str = oss.str();
    std::replace(T_str.begin(), T_str.end(), '.', 'p');

    std::string identifier = "temp_scan";
    identifier += "-n" + std::to_string(args.temperature_steps);
    identifier += "-L" + std::to_string(args.lattice_size);

    return identifier;
}

std::string generate_run_name(const Args &args)
{
    // 1) Format temperature: precision 1 & replace '.' with 'p'
    std::ostringstream oss;
    oss.precision(1);
    oss << std::fixed << args.temperature;

    std::string T_str = oss.str();
    std::replace(T_str.begin(), T_str.end(), '.', 'p');

    // 2) Create filename, modified based T, L, and initial configuration
    std::string identifier = "MCMC";
    identifier += args.ordered ? "-ordered" : "-unordered";
    identifier += "-L" + std::to_string(args.lattice_size);
    identifier += "-T" + T_str;

    return identifier;
}

void temperature_scan(Args &args, const std::string &out_dir)
{
    double T_min = args.temperature_min;
    double T_max = args.temperature_max;
    int T_steps = args.temperature_steps;

    // Define vectors to hold temperature, heat capacity, and susceptibility
    arma::colvec T = arma::linspace(T_min, T_max, T_steps + 1);
    // arma::colvec eps = arma::zeros(T_steps + 1);
    // arma::colvec m_abs = arma::zeros(T_steps + 1);
    // arma::colvec C_V = arma::zeros(T_steps + 1);
    // arma::colvec chi = arma::zeros(T_steps + 1);

    arma::mat results = arma::zeros(T_steps + 1, 7);
    
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

        results(i, 0) = T(i);
        results(i, 1) = solver.results.avg_energy / lattice.N;
        results(i, 2) = solver.results.MCSE_energy / lattice.N;
        results(i, 3) = solver.results.avg_magnetization_abs / lattice.N;
        results(i, 4) = solver.results.MCSE_magnetization / lattice.N;
        results(i, 5) = solver.results.heat_capacity / lattice.N;
        results(i, 6) = solver.results.susceptibility / lattice.N;

        //eps(i) = solver.results.avg_energy / lattice.N;
        //m_abs(i) = solver.results.avg_magnetization_abs / lattice.N;
        //C_V(i) = solver.results.heat_capacity / lattice.N;
        //chi(i) = solver.results.susceptibility / lattice.N;
    }
    
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    
    std::cout << "Temperature scan completed in " << elapsed.count() << " seconds.\n";

    // arma::mat results = arma::zeros(T_steps + 1, 5);
    // results.col(0) = T;
    // results.col(1) = eps;
    // results.col(2) = m_abs;
    // results.col(3) = C_V;
    // results.col(4) = chi;

    // results.save(out_dir + "T_scan-n" + std::to_string(args.temperature_steps) + "-L" + std::to_string(args.lattice_size) + ".csv", arma::csv_ascii);

    results.save(out_dir + generate_temp_scan_name(args) + ".csv", arma::csv_ascii);
    
}
