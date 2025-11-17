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
        {"-T_scan", "--temperature_scan"},
        {"-T_min", "--temperature_min"},
        {"-T_max", "--temperature_max"},
        {"-T_steps", "--temperature_steps"},
        {"-n_steps", "--steps_per_cycle"},
        {"-n_cycles", "--total_cycles"},
        {"-n_burn", "--burn_in_cycles"},
        {"-h", "--help"}};

    // Parse each argument
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        // Check for aliases
        if (alias_map.count(arg)) arg = alias_map[arg];

        // ----------------------------
        // General/lattice parameters
        // ----------------------------
        if (arg == "--lattice_size" && i + 1 < argc)
        {
            args.lattice_size = std::stoi(argv[++i]);
        }
        else if (arg == "--coupling" && i + 1 < argc)
        {
            args.coupling = std::stod(argv[++i]);
        }
        else if (arg == "--temperature" && i + 1 < argc)
        {
            args.temperature = std::stod(argv[++i]);
        }
        else if (arg == "--ordered")
        {
            args.ordered = true;
        }
        // ----------------
        // MCMC parameters
        // ----------------
        else if (arg == "--run")
        {
            args.run = true;
        }
        else if (arg == "--steps_per_cycle" && i + 1 < argc)
        {
            args.steps_per_cycle = std::stoi(argv[++i]);
        }
        else if (arg == "--total_cycles" && i + 1 < argc)
        {
            args.total_cycles = std::stoi(argv[++i]);
        }
        else if (arg == "--burn_in_cycles" && i + 1 < argc)
        {
            args.burn_in_cycles = std::stoi(argv[++i]);
        }
        else if (arg == "--seed" && i + 1 < argc)
        {
            args.seed = std::stoi(argv[++i]);
        }

        // ----------------------------
        // Temperature scan parameters
        // ----------------------------
        else if (arg == "--temperature_scan")
        {
            args.temperature_scan = true;
        }
        else if (arg == "--temperature_min" && i + 1 < argc)
        {
            args.temperature_min = std::stod(argv[++i]);
        }
        else if (arg == "--temperature_max" && i + 1 < argc)
        {
            args.temperature_max = std::stod(argv[++i]);
        }
        else if (arg == "--temperature_steps" && i + 1 < argc)
        {
            args.temperature_steps = std::stoi(argv[++i]);
        }

        // ---------------
        // Misc.
        // ---------------
        else if (arg == "--save_data" && i + 1 < argc)
        {
            if (std::string(argv[++i]) == "false")
            {
                args.save_data = false;
            }
            else if (std::string(argv[i]) == "true")
            {
                args.save_data = true;
            }
            else
            {
                std::cerr << "Invalid value for --save_data. Use true or false.\n";
                exit(1);
            }
        }
        else if (arg == "--name" && i + 1 < argc)
        {
            args.run_name = argv[++i];
        }
        else if (arg == "--outdir" && i + 1 < argc)
        {
            args.out_dir = argv[++i];
            if (args.out_dir.back() != '/')
            {
                args.out_dir += "/";
            }
        }
        else if (arg == "--help")
        {
            std::cout << "Usage:\n"
                      << "  ./main [options]\n"
                      << "\n"
                      << "Options:\n"
                      << "  --lattice_size,    -L               size of the lattice (default: 2)\n"
                      << "  --coupling,        -J               coupling constant (default: 1.0)\n"
                      << "  --temperature,     -T               temperature (default: 1.0)\n"
                      << "  --ordered                           start with ordered spins (default: false)\n"
                      << "\n"
                      << "  --run              -r               run MCMC simulation (default: false)\n"
                      << "  --steps_per_cycle, -n_steps         number of steps per cycle (default: 1000)\n"
                      << "  --total_cycles,    -n_cycles        number of total cycles (default: 100)\n"
                      << "  --burn_in_cycles,  -n_burn          number of burn-in cycles (default: 0)\n"
                      << "  --seed                              random seed (default: 1234)\n"
                      << "\n"
                      << "  --temperature_scan,  -T_scan        perform temperature scan (default: false)\n"
                      << "  --temperature_min,   -T_min         minimum temperature for scan (default: 2.1)\n"
                      << "  --temperature_max,   -T_max         maximum temperature for scan (default: 2.4)\n"
                      << "  --temperature_steps, -T_steps       number of temperature steps (default: 30)\n"
                      << "\n"
                      << "  --outdir,                           output directory (default: out/)\n"
                      << "  --name,                             run name (default: autogenerates based on parameters)\n"
                      << "  --help,             -h              display this help message\n";

            exit(0);
        }
        else
        {
            std::cerr << "Unknown argument: " << arg << "\n";
            exit(1);
        }
    }

    // If no value set for steps_per_cycle, set to L^2
    if( args.steps_per_cycle == 0) {
        args.steps_per_cycle = args.lattice_size * args.lattice_size;
    }

    return args;
}