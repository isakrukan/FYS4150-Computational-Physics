#include "arg_parser.hpp"
#include <map>

Args parse_args(int argc, char *argv[])
{
    Args args;

    // Map aliases to canonical names
    std::map<std::string, std::string> alias_map = {
        {"-sx", "--sigma_x"},
        {"-sy", "--sigma_y"},
        {"-cx", "--center_x"},
        {"-cy", "--center_y"},
        {"-px", "--momentum_x"},
        {"-py", "--momentum_y"},    
    };

    // Parse each argument
    for (int i = 1; i < argc; i++){
        std::string arg = argv[i];

        // Check for aliases
        if (alias_map.count(arg))
            arg = alias_map[arg];

        /***  Wave packet parameters ***/
        if (arg == "--sigma_x" && i + 1 < argc){
            args.sigma_x = std::stod(argv[++i]);
        }
        else if (arg == "--sigma_y" && i + 1 < argc){
            args.sigma_y = std::stod(argv[++i]);
        }
        else if (arg == "--center_x" && i + 1 < argc){
            args.center_x = std::stod(argv[++i]);
        }
        else if (arg == "--center_y" && i + 1 < argc){
            args.center_y = std::stod(argv[++i]);
        }
        else if (arg == "--momentum_x" && i + 1 < argc){
            args.momentum_x = std::stod(argv[++i]);
        }
        else if (arg == "--momentum_y" && i + 1 < argc){
            args.momentum_y = std::stod(argv[++i]);
        }

        /***  Wall/slit parameters ***/
        else if (arg == "--wc" && i + 1 < argc){
            args.wc = std::stod(argv[++i]);
        }
        else if (arg == "--wt" && i + 1 < argc){
            args.wt = std::stod(argv[++i]);
        }
        else if (arg == "--d" && i + 1 < argc){
            args.d = std::stod(argv[++i]);
        }
        else if (arg == "--dy" && i + 1 < argc){
            args.dy = std::stod(argv[++i]);
        }
        else if (arg == "--v0" && i + 1 < argc){
            args.v0 = std::stod(argv[++i]);
        }
        
        /***  Simulation parameters ***/
        else if (arg == "--double_slit" && i + 1 < argc){
            std::string val = argv[++i];
            if (val == "true" || val == "1")
                args.double_slit = true;
            else if (val == "false" || val == "0")
                args.double_slit = false;
            else {
                std::cerr << "Invalid value for --double_slit: " << val << "\n";
                std::exit(1);
    }
        }
        else if (arg == "--dt" && i + 1 < argc){
            args.dt = std::stod(argv[++i]);
        }
        else if (arg == "--L" && i + 1 < argc){
            args.L = std::stod(argv[++i]);
        }
        else if (arg == "--h" && i + 1 < argc){
            args.h = std::stod(argv[++i]);
        }
        else if (arg == "--T" && i + 1 < argc){
            args.T = std::stod(argv[++i]);
        }
        else if (arg == "--identifier" && i + 1 < argc){
            args.identifier = std::string(argv[++i]);
        }
        else if (arg == "--help"){
            std::cout 
                    << "Usage:\n"
                    << "  ./main [options]\n"
                    << "\n"
                    << "Options:\n"
                    << "  --sigma_x,     -sx              Standard deviation in x\n"
                    << "  --sigma_y,     -sy              Standard deviation in y\n"
                    << "  --center_x,    -cx              Center position in x\n"
                    << "  --center_y,    -cy              Center position in y\n"
                    << "  --momentum_x,  -px              Momentum in x\n"
                    << "  --momentum_y,  -py              Momentum in y\n"
                    << "\n"
                    << "  --help                 Show this help message\n";

            exit(0);
        }
        else{
            std::cerr << "Unknown argument: " << arg << "\n";
            exit(1);
        }
    }

    return args;
}