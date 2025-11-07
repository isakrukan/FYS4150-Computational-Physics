#ifndef __stats_hpp__
#define __stats_hpp__

#include <armadillo>

/**
 * @brief Compute effective sample size of `data` using autocorrelation up to max_lag.
 * 
 * @param data Column vector of data points.
 * @param max_lag Maximum lag to consider for autocorrelation.
 * @return Effective sample size.
 */
double n_eff(const arma::colvec &data, int max_lag);

/**
 * @brief Compute autocorrelation of `data` at given lag.
 * 
 * @param data Column vector of data points.
 * @param lag Lag value for autocorrelation.
 * @return Autocorrelation at specified lag.
 */
double autocorrelation(const arma::colvec &data, int lag);

/**
 * @brief Compute standard error of the mean for `data` considering autocorrelation.
 * 
 * @param data Column vector of data points.
 * @return Standard error of the mean.
 */
double standard_error(const arma::colvec &data);

#endif