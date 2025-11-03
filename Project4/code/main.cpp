#include "lattice.hpp"
#include "utils.hpp"
#include "arg_parser.hpp"

std::string OUTDIR = "out/";

int main(int argc, char *argv[])
{
    // Parse command-line arguments using (see `arg_parser.hpp/.cpp`)
    Args args = parse_args(argc, argv);

    // -------------------------------------------------------
    // Generate filename for intermediate samples during MCMC:
    // -------------------------------------------------------

    // 1) Format temperature: precision 1 & replace '.' with 'p'
    std::ostringstream T_stream;
    T_stream.precision(1);
    T_stream << std::fixed << args.temperature;
    std::string T_str = T_stream.str();
    std::replace(T_str.begin(), T_str.end(), '.', 'p');

    // 2) Create filename, modified based T, L, and initial configuration
    std::string filename_MCMC;
    filename_MCMC = OUTDIR + "MCMC";
    filename_MCMC += args.ordered ? "-ordered" : "-unordered";
    filename_MCMC += "-L" + std::to_string(args.lattice_size);
    filename_MCMC += "-T" + T_str + ".csv";

    // ----------------------------------------
    // Create lattice and run MCMC simulation
    // ----------------------------------------
    Lattice lattice(args.lattice_size, args.coupling, args.temperature);
    lattice.initialize_spins(args.ordered);
    lattice.run_MCMC(args.total_cycles, args.steps_per_cycle, filename_MCMC);

    return 0;
}