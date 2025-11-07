
#ifndef __lattice_hpp__
#define __lattice_hpp__

#include <armadillo>

/**
 * @brief Class representing a 2D lattice for MCMC simulations.
 *
 * @param L: Size of the lattice (L x L).
 * @param T: Temperature of the system, in units of 1/kB.
 * @param J: Interaction strength between neighboring spins.
 * @param E: Total energy of the lattice.
 * @param M: Total magnetization of the lattice.
 * @param grid: 2D Armadillo matrix representing the lattice spins.
 */
class Lattice
{
    friend class MCMC;

private:
    arma::imat grid;
    //std::vector<int> grid;

    /**
     * @brief Compute the total energy of the lattice.
     */
    void initialize_energy();

    /**
     * @brief Compute the total magnetization of the lattice.
     */
    void initialize_magnetization();

public:
    Lattice(int L, double J, double T) : L(L), rows(L), cols(L), T(T), J(J), N(L * L) {};

    const int rows;
    const int cols;
    const int L;
    const int N;

    double T;
    double J;
    double E;
    double M;
    
    /**
     * @brief Set initial spin configuration and compute initial E and M.
     * 
     * @param ordered If true, initialize in ordered state; else random.
     */
    void initialize_spins(bool ordered=false);

    int get_spin(int i, int j) const;

    void flip_spin(int i, int j);
    
    double get_energy() const;

    double get_magnetization() const;

};

#endif