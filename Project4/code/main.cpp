#include "lattice.hpp"
#include "utils.hpp"
#include "arg_parser.hpp"
#include "mcmc.hpp"
#include <filesystem>
#include <chrono>

//std::string OUTDIR = "out/";

int main(int argc, char *argv[])
{

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::cout << "Starting script " << std::endl;

    // Parse command-line arguments using (see `arg_parser.hpp/.cpp`)
    Args args = parse_args(argc, argv);

    // If run flag is set, perform MCMC simulation
    if (args.run)
    {
        // Generate run name if not provided
        args.out_dir  = args.out_dir.empty() ? "out/" + generate_run_name(args) : args.out_dir;

        // Set and create output directory
        std::string out_dir = args.out_dir + "/";
        std::filesystem::create_directories(out_dir);

        // Initialize lattice and run MCMC solver
        Lattice lattice(args.lattice_size, args.coupling, args.temperature);
        lattice.initialize_spins(args.ordered);

        MCMC solver(lattice, args.seed);
        solver.run(args.total_cycles, args.steps_per_cycle, args.burn_in_cycles);
        solver.save_data(out_dir);
    }

    // If temperature scan flag is set, perform temperature scan
    if (args.temperature_scan)
    {
        // + generate_temp_scan_name(args) + "/";

        args.run_name = args.run_name.empty() ? generate_temp_scan_name(args) : args.run_name;
        args.out_dir = args.out_dir.empty() ? "out/temperature_scans/" : args.out_dir;

        // Set and create output directory
        std::filesystem::create_directories(args.out_dir);

        // Perform temperature scan (see `utils.hpp/.cpp`)
        temperature_scan(args);
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Finished running script, total elapsed time: " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << " seconds" << std::endl;

    return 0;
}