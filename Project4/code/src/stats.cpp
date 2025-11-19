
#include <armadillo>
#include "stats.hpp"

double n_eff(const arma::colvec &data, int max_lag)
{
    double sum = 0.0;

    double autocorr_new = 1.0;
    double autocorr_old = 1.0;

    for (int lag = 1; lag <= max_lag; ++lag)
    {
        autocorr_old = autocorr_new;
        autocorr_new = autocorrelation(data, lag);

        if (autocorr_old + autocorr_new < 0)
        {
            break;
        }

        sum += autocorr_new;
    }

    double n_effective = data.n_elem / (1 + 2 * sum);
    
    return n_effective;
}

double autocorrelation(const arma::colvec &data, int lag)
{
    int n = data.n_elem;
    double mean = arma::mean(data);
    double var = arma::var(data);

    double sum = 0;

    for (int i = 0; i < n - lag; ++i)
    {
        sum += (data(i) - mean) * (data(i + lag) - mean);
    }

    double auto_corr = sum / ((n - lag) * var);

    return auto_corr;
}


double standard_error(const arma::colvec &data)
{
    double var = arma::var(data);
    double n_effective = n_eff(data, data.n_elem / 2);

    return std::sqrt(var / n_effective);
}