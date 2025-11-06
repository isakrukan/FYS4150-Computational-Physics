#include "utils.hpp"

int pbc(int x, int L)
{
    return x - L * (x >= L) + L * (x < 0);
}

std::string generate_filename(const Args &args, const std::string &OUTDIR)
{
    // 1) Format temperature: precision 1 & replace '.' with 'p'
    std::ostringstream oss;
    oss.precision(1);
    oss << std::fixed << args.temperature;

    std::string T_str = oss.str();
    std::replace(T_str.begin(), T_str.end(), '.', 'p');

    // 2) Create filename, modified based T, L, and initial configuration
    std::string output_filename = OUTDIR + "MCMC";
    output_filename += args.ordered ? "-ordered" : "-unordered";
    output_filename += "-L" + std::to_string(args.lattice_size);
    output_filename += "-T" + T_str + ".csv";

    return output_filename;
}

void temperature_scan(Args &args, const std::string &OUTDIR)
{
    double T_min = args.temperature_min;
    double T_max = args.temperature_max;
    int T_steps = args.temperature_steps;

    // Define vectors to hold temperature, heat capacity, and susceptibility
    arma::colvec T = arma::linspace(T_min, T_max, T_steps + 1);
    arma::colvec eps = arma::zeros(T_steps + 1);
    arma::colvec m_abs = arma::zeros(T_steps + 1);
    arma::colvec C_V = arma::zeros(T_steps + 1);
    arma::colvec chi = arma::zeros(T_steps + 1);

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

        MCMC solver(lattice, args.total_cycles, args.steps_per_cycle, args.seed + i*100);
        solver.set_burn_in(args.burn_in_cycles);
        solver.run(args.total_cycles, args.steps_per_cycle);

        
        eps(i) = solver.results.avg_energy / lattice.N;
        m_abs(i) = solver.results.avg_magnetization_abs / lattice.N;
        C_V(i) = solver.results.heat_capacity / lattice.N;
        chi(i) = solver.results.susceptibility / lattice.N;
    }
    
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    
    std::cout << "Temperature scan completed in " << elapsed.count() << " seconds.\n";

    arma::mat results = arma::zeros(T_steps + 1, 5);
    results.col(0) = T;
    results.col(1) = eps;
    results.col(2) = m_abs;
    results.col(3) = C_V;
    results.col(4) = chi;

    results.save(OUTDIR + "T_scan-n" + std::to_string(args.temperature_steps) + "-L" + std::to_string(args.lattice_size) + ".csv", arma::csv_ascii);

}