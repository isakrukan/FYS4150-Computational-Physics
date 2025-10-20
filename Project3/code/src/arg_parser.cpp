#include "arg_parser.hpp"

Args parse_args(int argc, char *argv[])
{
    Args args;

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        if (arg == "--problem_8")
        {
            args.problem_8 = true;
        }
        else if (arg == "--problem_9")
        {
            args.problem_9 = true;
        }
        else if (arg == "--n_steps" && i + 1 < argc)
        {
            args.n_steps = std::stoi(argv[++i]);
        }
        else if (arg == "--n_particles" && i + 1 < argc)
        {
            args.n_particles = std::stoi(argv[++i]);
        }
        else if (arg == "--T" && i + 1 < argc)
        {
            args.T = std::stod(argv[++i]);
        }
        else if (arg == "--seed" && i + 1 < argc)
        {
            args.seed = std::stoi(argv[++i]);
        }
        else if (arg == "--amplitude" && i + 1 < argc)
        {
            args.amplitude = std::stod(argv[++i]);
        }
        else if (arg == "--n_freq" && i + 1 < argc)
        {
            args.n_freq = std::stoi(argv[++i]);
        }
        else
        {
            std::cerr << "Unknown argument: " << arg << "\n";
            exit(1);
        }
    }

    return args;
}