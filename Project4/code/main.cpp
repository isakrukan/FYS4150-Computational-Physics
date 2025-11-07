#include "lattice.hpp"
#include "utils.hpp"
#include "arg_parser.hpp"
#include "mcmc.hpp"

std::string OUTDIR = "out/";

int main(int argc, char *argv[])
{
    // Parse command-line arguments using (see `arg_parser.hpp/.cpp`)
    Args args = parse_args(argc, argv);

    // If no filename provided, generate one based on parameters
    if (args.filename.empty())
    {
        args.filename = generate_filename(args, OUTDIR);
    }

    std::cout << args.filename << std::endl;
    std::cout << args.run << std::endl;
    // If run flag is set, perform MCMC simulation
    if (args.run)
    {
        Lattice lattice(args.lattice_size, args.coupling, args.temperature);
        lattice.initialize_spins(args.ordered);

        MCMC solver(lattice, args.total_cycles, args.steps_per_cycle, args.seed);
        solver.set_burn_in(args.burn_in_cycles);
        solver.run(args.total_cycles, args.steps_per_cycle, args.filename);
    }

    // If temperature scan flag is set, perform temperature scan
    if (args.temperature_scan)
    {
        temperature_scan(args);
    }
    return 0;
}