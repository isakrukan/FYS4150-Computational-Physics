import numpy as np
from scipy.stats import linregress
import os

def combine_runs(DATA_DIR, L):
    N = L * L
    
    # Loading first data_set to get shape
    data = np.loadtxt(DATA_DIR + f'results-L{L}/scan_{1}.csv', delimiter=',')
    data_shape = np.shape(data)
    
    data_combined = np.zeros((data_shape[0], 9)) # Should always be 9 on second axis
    num_repeats = len(os.listdir(DATA_DIR + f'results-L{L}/') )
    
    sum_inv_sqr_MCSE_E = 0.0
    sum_inv_sqr_MCSE_M = 0.0
    
    for i in range(1, num_repeats + 1):
        data = np.loadtxt(DATA_DIR + f'results-L{L}/scan_{i}.csv', delimiter=',')
        data_combined += data
        sum_inv_sqr_MCSE_E += 1.0 / (data[:,5]**2)
        sum_inv_sqr_MCSE_M += 1.0 / (data[:,7]**2)

    # Average all except ESS (indices 6 and 8), which are just summed:
    data_combined[:, :6] = data_combined[:, :6] / num_repeats
    data_combined[:,  7] = data_combined[:,  7] / num_repeats


    return data_combined

def estimate_T_infinite(L_vals, T_vals):
    x = 1 / L_vals
    y = T_vals

    slope, intercept, r_value, p_value, std_err = linregress(x, y)
    T_infinite = intercept
    return T_infinite, std_err
