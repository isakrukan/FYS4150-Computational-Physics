#include "lattice.hpp"
#include "utils.hpp"
#include "arg_parser.hpp"

std::string OUTDIR = "out/";

int main(int argc, char *argv[])
{
    Args args = parse_args(argc, argv);

    Lattice lattice(args.lattice_size, args.coupling, args.temperature);
    lattice.initialize_spins(args.ordered);
    lattice.run_MCMC(args.total_cycles, args.steps_per_cycle, OUTDIR + args.filename);

    return 0;
}