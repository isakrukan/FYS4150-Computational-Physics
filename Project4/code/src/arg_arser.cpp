#include "arg_parser.hpp"
#include <map>

Args parse_args(int argc, char *argv[])
{
    Args args;

    // Map aliases to canonical names
    std::map<std::string, std::string> alias_map = {
        {"-L", "--lattice_size"},
        {"-J", "--coupling"},
        {"-T", "--temperature"},
        {"-n_steps", "--steps_per_cycle"},
        {"-n_cycles", "--total_cycles"},
        {"-h", "--help"}};

    // Parse each argument
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        // Check for aliases
        if (alias_map.count(arg)) arg = alias_map[arg];

        // Match and assign values
        if (arg == "--lattice_size" && i + 1 < argc)
        {
            args.lattice_size = std::stoi(argv[++i]);
        }
        else if (arg == "--coupling" && i + 1 < argc)
        {
            args.coupling = std::stod(argv[++i]);
        }
        else if (arg == "--steps_per_cycle" && i + 1 < argc)
        {
            args.steps_per_cycle = std::stoi(argv[++i]);
        }
        else if (arg == "--total_cycles" && i + 1 < argc)
        {
            args.total_cycles = std::stoi(argv[++i]);
        }
        else if (arg == "--temperature" && i + 1 < argc)
        {
            args.temperature = std::stod(argv[++i]);
        }
        else if (arg == "--ordered")
        {
            args.ordered = true;
        }
        else if (arg == "--filename" && i + 1 < argc)
        {
            args.filename = argv[++i];
        }
        else if (arg == "--help")
        {
            std::cout << "Usage:\n"
                      << "  ./program [options]\n"
                      << "\n"
                      << "Options:\n"
                      << "  --lattice_size,    -L           size of the lattice (default: 2)\n"
                      << "  --coupling,        -J           coupling constant (default: 1.0)\n"
                      << "  --temperature,     -T           temperature (default: 1.0)\n"
                      << "  --steps_per_cycle, -n_steps     number of steps per cycle (default: 1000)\n"
                      << "  --total_cycles,    -n_cycles    number of total cycles (default: 100)\n";

            exit(0);
        }
        else
        {
            std::cerr << "Unknown argument: " << arg << "\n";
            exit(1);
        }
    }

    return args;
}