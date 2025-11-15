import os
import numpy as np 
from scipy.stats import linregress
from tabulate import tabulate
import utils

# Set directories
OUT_DIR = "out/"
DATA_DIR = OUT_DIR + "temperature_scans/repeat_runs/"

num_temps = 101                                # Number of temperature points
lattice_sizes = np.array([40, 60, 80, 100])    # Lattice sizes used
window = 5                                     # Fitting window size (see below)

if not os.path.exists(os.path.join(os.getcwd(), DATA_DIR)):
    DATA_DIR = OUT_DIR + "temperature_scans/tests/"
    num_temps = 11
    print(f"WARNING: Default data directory not found. Using test data: {DATA_DIR}")

# Temperature values & arrays for T_C and T_X estimates
T = np.linspace(2.1, 2.4, num_temps)
T_C = np.zeros(len(lattice_sizes))
T_X = np.zeros(len(lattice_sizes))

for i, L in enumerate(lattice_sizes):
    N = L * L
    
    # --- Load data ---
    data = utils.combine_runs(DATA_DIR, L, num_temps=num_temps)
    C = data[:, 2] * N / (T * T) 
    X = data[:, 4] * N / T
    
    # --- Problem 9 ---
    maxidx_C = np.argmax(C)
    maxidx_X = np.argmax(X)
    
    # ---
    # NOTE: Due to noisy data, picking out the max point directly may give the wrong 
    # estimate for the critical temperature. Instead, fit a quadratic polynomial to 
    # a small window around the max and use the peak of this polynomial:
    # ---
    T_fit_C = T[maxidx_C - window : maxidx_C + window + 1]
    T_fit_X = T[maxidx_X - window : maxidx_X + window + 1]
    C_fit = C[maxidx_C - window : maxidx_C + window + 1]
    X_fit = X[maxidx_X - window : maxidx_X + window + 1]

    coeffs_C = np.polyfit(T_fit_C, C_fit, 2)
    coeffs_X = np.polyfit(T_fit_X, X_fit, 2)

    T_C[i] = -coeffs_C[1] / (2 * coeffs_C[0])
    T_X[i] = -coeffs_X[1] / (2 * coeffs_X[0])


T_C_infinite, T_C_std_err = utils.estimate_T_infinite(lattice_sizes, T_C)
T_X_infinite, T_X_std_err = utils.estimate_T_infinite(lattice_sizes, T_X)

print("\n--- Problem 9 ---")

col_1 = ["40", "60", "80", "100"]
col_2 = [f"{temp:.5f}" for temp in T_C]
col_3 = [f"{temp:.5f}" for temp in T_X]
table = tabulate(zip(col_1, col_2, col_3), headers=["L", "T_C (J/k_B)", "T_X (J/k_B)"], tablefmt="rounded_grid")
print(table)

print(f"Estimated T(∞) using heat capacity: {T_C_infinite} ± {T_C_std_err}")
print(f"Estimated T(∞) using susceptibility: {T_X_infinite} ± {T_X_std_err}")
