#pragma once 

#include <armadillo>


class SlitExperiment{

private:

public:


unsigned int M; ///< Space points. 
arma::vec x;
arma::vec y;
arma::cx_mat V; ///< Potential.
arma::cx_mat u_initial; ///< Initial wave function.

/**
 * @brief Default constructor, sets up the space environment.
 * @param M Number of space points.
 */
SlitExperiment(unsigned int M);
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
 * @param cx Centre position of wall in the x-direction.
 * @param wt Wall thickness in the x-direction.
 * @param d Distance between the two slits.
 * @param dy Height of slits (y-direction).
 */
void add_double_slit(double cx=0.5, double wt=0.02, double d=0.05, double dy=0.05);


};