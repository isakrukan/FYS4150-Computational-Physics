
#ifndef __solver_hpp__
#define __solver_hpp__

#include "penningTrap.hpp"

/**
 * @brief Class to solve evolve the Penning trap using a variety of possible numerical integration methods.
 * 
 */
class Solver
{

public:

    /**
     * @brief Construct a new Solver object.
     * 
     * @param @ref PenningTrap "PenningTrap" object.
     */
    Solver(PenningTrap &trap);

    /**
     * @brief Evolves the particles inside the Penning trap in time using the Runge-Kutta 4 method.
     * 
     * @param dt 
     */
    void evolve_RK4(float dt);
    
    void evolve_FE(float dt);
    
private:
    PenningTrap &trap;

    /**
     * @brief The force experienced by particle 'i' due to the external field and Coulomb forces.
     *
     * @param i Particle index.
     * @param t Time \f$ \mu s\f$ (microseconds).
     * @return arma::vec force on particle @ref i "i".
     */
    arma::vec f(int i, float t);
};

#endif
