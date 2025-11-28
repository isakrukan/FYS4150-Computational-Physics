#pragma once 

#include <armadillo>
#include <vector>

class SlitExperiment{

private:

public:


unsigned int M; ///< Space points. 
double h; ///< Space separation. 
arma::vec x;
arma::vec y;
arma::cx_mat V; ///< Potential.
arma::cx_vec u_initial; ///< Initial wave function.
double v0; ///< Potential height of slit wall.

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
 */
void add_double_slit(double v0=1e10, double cx=0.5, double wt=0.02, double d=0.05, double dy=0.05);


};