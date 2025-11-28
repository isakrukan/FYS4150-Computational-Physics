#ifndef __double_slit_hpp__
#define __double_slit_hpp__

#include "CrankNicolson.hpp"
#include <armadillo>
#include <cmath>

class DoubleSlit 
{

private:

    // Parameters for the wall and slits
    double wall_thickness = 0.02;
    double wall_position  = 0.5;
    double slit_separation = 0.05;
    double slit_aperture = 0.05;
    double slit1_center = -slit_separation / 2.0;
    double slit2_center =  slit_separation / 2.0;

    unsigned int M;             ///< Number of spatial rows
    double L = 1.0;             ///< Length of the system
    double h;                   ///< Spatial step size

    arma::cube positions;       ///< Positions matrix
    arma::cx_mat current_state; ///< Current state matrix
    arma::cx_mat potential;     ///< Potential matrix
    arma::cx_cube solution;     ///< Solution storage cube

    /**
     * @brief Normalizes the current state matrix.
     */
    void normalize_state();

    /**
     * @brief Sets the potential matrix for the double slit configuration.
     */
    void set_potential();

public:
    DoubleSlit(double L, double h, double V_0);

    double V_0;     ///< Height of the potential barrier

    /** 
     * @brief Sets the initial state of the system.
     * @param state Initial state matrix
     */
    void set_initial_state(arma::cx_mat state);

    /**
     * @brief Sets the initial state of the system.
     * @param func Function f(x,y) to generate initial state based on position
     */
    void set_initial_state(std::function<arma::cx_double(double, double)> func);

    /** 
     * @brief Gets the x position at grid point (i, j).
     * @param i Row index
     * @param j Column index
     * @return x position
     */
    double get_x(int i, int j);

    /** 
     * @brief Gets the y position at grid point (i, j).
     * @param i Row index
     * @param j Column index
     * @return y position
     */
    double get_y(int i, int j);

    void solve(double T, double dt);
};

#endif