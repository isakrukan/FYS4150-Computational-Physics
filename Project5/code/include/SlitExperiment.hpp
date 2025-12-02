#pragma once 

#include <armadillo>
#include <vector>

class SlitExperiment{

public:

//**** Public variables ****//
unsigned int M; ///< Space points. 
double h; ///< Space separation. 
arma::vec x;
arma::vec y;
arma::cx_mat V; ///< Potential.
arma::cx_vec u_initial; ///< Initial wave function.
double v0; ///< Potential height of slit wall.

//********** Constructor(s) **********//

/**
 * @brief Default constructor, sets up the space environment using the number of points.
 * @param M Number of space points.
 */
SlitExperiment(unsigned int M);

/**
 * @brief Default constructor, sets up the space environment using the spacing.
 * @param h Interval between space points.
 */
SlitExperiment(double h);
SlitExperiment() = default;

//********** Public methods **********//

/**
 * @brief Adds Gaussian wave packet to desired location.
 * @param xy_c Center of Gaussian.
 * @param sigma_xy Initial width of Gaussian.
 * @param p_xy Wave packet momentum.
 */
void add_wave_packet(std::vector<double> xy_c, std::vector<double> sigma_xy, std::vector<double> p_xy);

/**
 * @brief Adds a (symmetric) double slit to the experiment.
 * @param v0 Potential of slit "wall".
 * @param cx Centre position of wall in the x-direction.
 * @param wt Wall thickness in the x-direction.
 * @param d Distance between the two slits.
 * @param dy Height of slits (y-direction).
 * @param cy Centre position of the two slits in the y-direction, defaults to 0.5 (middle of grid).
 */
void add_double_slit(double v0=1e10, double cx=0.5, double wt=0.02, double d=0.05, double dy=0.05, double cy=0.5);

/**
 * @brief Adds a (symmetric) single slit to the experiment.
 * @param v0 Potential of slit "wall".
 * @param cx Centre position of wall in the x-direction.
 * @param wt Wall thickness in the x-direction.
 * @param dy Height of slit (y-direction).
 * @param cy Centre position of the slit in the y-direction, defaults to 0.5 (middle of grid).
 */
void add_single_slit(double v0=1e10, double cx=0.5, double wt=0.02,double dy=0.1, double cy=0.5);

/**
 * @brief Adds a (symmetric) triple slit to the experiment.
 * @param v0 Potential of slit "wall".
 * @param cx Centre position of wall in the x-direction.
 * @param wt Wall thickness in the x-direction.
 * @param d Distance between the three slits.
 * @param dy Height of slits (y-direction).
 * @param cy Centre position of the three slits in the y-direction, defaults to 0.5 (middle of grid).
 */
void add_triple_slit(double v0=1e10, double cx=0.5, double wt=0.02, double d=0.05, double dy=0.05, double cy=0.5);

};