#include "tests/test_matrix_initialization.cpp"
#include "utils.hpp"
#include "arg_parser.hpp"
#include "DoubleSlit.hpp"

int main(int argc, char *argv[]){

    // test_matrix_initialization();

    Args args = parse_args(argc, argv);

    // Set initial Gaussian wave packet based on input arguments
    auto initial_state_func = [&args](double x, double y) {
        return gaussian_wave_packet(x, y, args);
    };


    DoubleSlit system(args.L, args.h, args.V_0);
    system.set_initial_state(initial_state_func);
    system.solve(args.T, args.dt);

    return 0;
}