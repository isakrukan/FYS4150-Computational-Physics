#include "lattice.hpp"
#include "utils.hpp"
#include "arg_parser.hpp"
#include "mcmc.hpp"
#include <filesystem>

//std::string OUTDIR = "out/";

int main(int argc, char *argv[])
{
    // Parse command-line arguments using (see `arg_parser.hpp/.cpp`)
    Args args = parse_args(argc, argv);

    // If run flag is set, perform MCMC simulation (default: false)
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

        // Save data if flag is set (default: true)
        if (args.save_data)
        {
            solver.save_data(out_dir);
        }
    }

    // If temperature scan flag is set, perform temperature scan (default: false)
    if (args.temperature_scan) 
    {

        // Create output directory if saving data (default: true)
        if (args.save_data)
        {
            args.run_name = args.run_name.empty() ? generate_temp_scan_name(args) : args.run_name;
            args.out_dir = args.out_dir.empty() ? "out/temperature_scans/" : args.out_dir;

            // Set and create output directory
            std::filesystem::create_directories(args.out_dir);
        }

        // Perform temperature scan (see `utils.hpp/.cpp`)
        temperature_scan(args);
    }
    return 0;
}