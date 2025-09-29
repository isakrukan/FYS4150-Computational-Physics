#include "penningTrap.hpp"
#include "solver.hpp"
#include <iostream>

int main(){

    PenningTrap trap;
    arma::vec r1 = arma::vec({20, 0, 20}); // column vector
    arma::vec v1 = arma::vec({0, 25, 0});  // column vector

    arma::vec r2 = {25, 25, 25};
    arma::vec v2 = {0, 30, 0};

    trap.add_particle(1, 40, r1, v1);
    // trap.add_particle(1, 40, r2_0, v2_0);

    Solver solver(trap);


    int N = 1000;            // Number of steps
    double T = 50.0;         // Total time [μs]
    double dt = T/N;         // Time step [μs]

    arma::mat r1_mat(N, 3);
    r1_mat.row(0) = r1.t();
    arma::mat r2_mat(N, 3);

    for (int i = 1; i < N; i++)
    {
        solver.evolve_FE(dt);
        r1_mat.row(i) = r1.t();
    }

    r1_mat.save("problem_8.txt", arma::raw_ascii);

    return 0;
}
