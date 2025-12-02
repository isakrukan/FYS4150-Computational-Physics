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

    if (args.single_slit){ // If true, adds single slit
        doubleSlit.add_single_slit( args.v0,
                                args.wc, 
                                args.wt, 
                                args.dy);
    }

    if (args.double_slit){ // If true, adds double slit
        doubleSlit.add_double_slit( args.v0, 
                                args.wc, 
                                args.wt, 
                                args.d, 
                                args.dy);
    }

    if (args.triple_slit){ // If true, adds triple slit
        doubleSlit.add_triple_slit( args.v0, 
                                args.wc, 
                                args.wt, 
                                args.d, 
                                args.dy);
    }

    doubleSlit.add_wave_packet({args.center_x, args.center_y}, {args.sigma_x, args.sigma_y}, {args.momentum_x, args.momentum_y});

    Solver solve = Solver(args.h, args.dt); // Defaults to CrankNicolsen with arma::spsolve (the only one implemented yet)
    solve.add_SlitExperiment(doubleSlit);
    solve.evolve(args.T);
    solve.save("out/" + args.identifier);


    
}   
