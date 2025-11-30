#include "arg_parser.hpp"
#include "DoubleSlit.hpp"
#include "Solver.hpp"
#include "utils.hpp"

int main(int argc, char *argv[]){

    // test_matrix_initialization();

    // Args args = parse_args(argc, argv);

    // // Set initial Gaussian wave packet based on input arguments
    // auto initial_state_func = [&args](double x, double y) {
    //     return gaussian_wave_packet(x, y, args);
    // };


    // DoubleSlit system(args.L, args.h, args.V_0);
    // system.set_initial_state(initial_state_func);
    // system.solve(args.T, args.dt);

    // return 0;

    Args args = parse_args(argc, argv);
    SlitExperiment doubleSlit = SlitExperiment(args.h);
    // doubleSlit.add_double_slit();
    doubleSlit.add_wave_packet({0.25, 0.5}, {0.05, 0.05}, {200,0});

    Solver solve = Solver(args.h, args.Delta_t); // Defaults to CrankNicolsen with arma::spsolve (the only one implemented yet)
    solve.add_SlitExperiment(doubleSlit);
    solve.evolve(args.T);
    solve.save("out/");


    
}   
