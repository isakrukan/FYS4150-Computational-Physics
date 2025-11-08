import numpy as np 
from scipy.stats import linregress

L_vals = np.array([40, 60, 80, 100])
repeat_idx = [1, 2]
n_repeats = len(repeat_idx)
DATA_DIR = "out/temperature_scans/repeat_runs/"

T = np.linspace(2.1, 2.4, 101)
steps_T = len(T)

T_C = np.zeros(len(L_vals))
T_X = np.zeros(len(L_vals))

for i, L in enumerate(L_vals):
    data_averaged = np.zeros((steps_T, 7))
    for j in repeat_idx:
        data_averaged += np.loadtxt(DATA_DIR + f'results-L{L}/scan_{j}.csv', delimiter=',')
        
    data_averaged /= n_repeats

    maxidx_C = np.argmax(data_averaged[:, 5])
    maxidx_X = np.argmax(data_averaged[:, 6])

    T_C[i] = data_averaged[maxidx_C, 0]
    T_X[i] = data_averaged[maxidx_X, 0]



def estimate_T_infinite(L_vals, T_vals):
    x = 1 / L_vals
    y = T_vals

    slope, intercept, r_value, p_value, std_err = linregress(x, y)
    T_infinite = intercept
    return T_infinite, std_err

T_C_infinite, T_C_std_err = estimate_T_infinite(L_vals, T_C)
T_X_infinite, T_X_std_err = estimate_T_infinite(L_vals, T_X)

print(f"Estimated T(∞) using heat capacity: {T_C_infinite} ± {T_C_std_err}")
print(f"Estimated T(∞) using susceptibility: {T_X_infinite} ± {T_X_std_err}")
