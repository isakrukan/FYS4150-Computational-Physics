
#ifndef __lattice_hpp__
#define __lattice_hpp__

#include <armadillo>

/**
 * @brief A single location on the \ref Lattice "Lattice" class.
 */
struct Location
{
    Location *left = nullptr;     ///< Pointer to the location to the left.
    Location *right = nullptr;    ///< Pointer to the location to the right.
    Location *up = nullptr;       ///< Pointer to the location to the up.
    Location *down = nullptr;     ///< Pointer to the location to the down.

    int s = 0;              ///< Spin state (\f$s = \pm 1\f$) of location. Initialized to zero for debugging.

    Location(int s_): s(s_) {};
    Location() = default; // To avoid possible errors from resizing grid in Lattice.
};


/**
 * @brief Representations of a 2D Ising lattice for MCMC simulations.
 */
class Lattice
{
    friend class MCMC;

private:
    std::vector<std::vector<Location>> grid;    ///< 2D nested vector of \ref Location "Locations".

    /**
     * @brief Sets up the pointers in the lattice.
     */
    void assign_pointers();

    /**
     * @brief Compute the total energy of the lattice.
     */
    void initialize_energy();

    /**
     * @brief Compute the total magnetization of the lattice.
     */
    void initialize_magnetization();

public:

    /**
     * @brief Only possible constructor for \ref Lattice "Lattice" class, assigns values to 
     * \ref L "L", \ref J "J", \ref T "T", and calls 
     * \ref assign_pointers "assign_pointers", \ref initialize_energy "initialize_energy" and \ref initialize_magnetization "initialize_magnetization".
     */
    Lattice(int L_, double J_, double T_) : L(L_), rows(L_), cols(L_), T(T_), J(J_), N(L_ * L_) {};

    const int rows; ///< Number of rows
    const int cols; ///< Number of cols
    const int L;    ///< Size of the lattice (L x L).
    const int N;    ///< Number of spin locations.

    double T;       ///< Temperature of the system, in units of 1/kB.
    double J;       ///< Interaction strength between neighboring spins.
    double E;       ///< Total energy of the lattice.
    double M;       ///< Total magnetization of the lattice.
    
    /**
     * @brief Set initial spin configuration and compute initial E and M.
     * 
     * @param ordered If true, initialize in ordered state; else random.
     */
    void initialize_spins(bool ordered=false);

    /**
     * @brief Returns the spin at position (i, j).
     * 
     * @param i Row index.
     * @param j Column index.
     */
    int get_spin(int i, int j) const;

    /**
     * @brief Flips the spin at position (i, j) and updates E and M accordingly.
     * 
     * @param i Row index.
     * @param j Column index.
     */
    void flip_spin(int i, int j);
    
    /**
     * @brief Returns the total energy of the lattice.
     */
    double get_energy() const;

    /**
     * @brief Returns the total magnetization of the lattice.
     */
    double get_magnetization() const;

    /**
     * @brief Returns the column of location at row @ref row_index 
     * 
     * @param row_index Row index.
     */
    std::vector<Location>& operator[](int row_index);

};

#endif