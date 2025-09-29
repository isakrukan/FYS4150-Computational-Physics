
#ifndef __solver_hpp__
#define __solver_hpp__

#include "penningTrap.hpp"

class Solver
{

public:
    Solver(PenningTrap &trap);

    void evolve_RK4(float dt);
    
    void evolve_FE(float dt);
    
private:
    PenningTrap &trap;
 
    arma::vec f(int i, float t, arma::vec x, arma::vec v);
};

#endif
