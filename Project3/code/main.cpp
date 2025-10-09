#include "penningTrap.hpp"
#include "solver.hpp"
#include "arg_parser.hpp"
#include <iostream>


void problem8(Args args){
    //// Problem 8
    arma::vec r1 = {20, 0, 20}; // [mu m]
    arma::vec v1 = {0, 25, 0}; // [mu m/mu s]
    arma::vec r2 = {25, 25, 0}; // [mu m]
    arma::vec v2 = {0, 40, 5}; // [mu m/mu s]

    /// Single particle 
    int T = 50; // [mu s]
    PenningTrap single_particle;
    single_particle.add_particle(1, 40, r1, v1);
    Solver single_solver(single_particle);
    single_solver.evolve(T, args.n_steps); // Uses RK4 by default
    single_solver.save();

    /// Two particles
    PenningTrap interacting_trap;
    interacting_trap.add_particle(1, 40, r1, v1);
    interacting_trap.add_particle(1, 40, r2, v2);
    Solver interacting_solver(interacting_trap);
    interacting_solver.evolve(T, args.n_steps); // Uses RK4 by default
    interacting_solver.save();

    PenningTrap non_interacting_trap;
    non_interacting_trap.add_particle(1, 40, r1, v1);
    non_interacting_trap.add_particle(1, 40, r2, v2);
    non_interacting_trap.interacting = false;
    Solver non_interacting_solver(non_interacting_trap);
    non_interacting_solver.evolve(T, args.n_steps); // Uses RK4 by default
    non_interacting_solver.save();
    
    // Single particle, various specific time steps n 
    std::vector<int> n = {4000, 8000, 16000, 32000};

    for (int i=0; i<n.size(); i++){
        PenningTrap single_particle;
        single_particle.add_particle(1, 40, r1, v1);
        Solver single_solver_(single_particle);
        single_solver_.evolve(T, n[i]); // Uses RK4 by default
        single_solver_.save();
    }
}

int main(int argc, char *argv[])
{
    Args args = parse_args(argc, argv);
    problem8(args);

    return 0;

}
