import numpy as np
from scipy.stats import linregress
import os
import matplotlib.pyplot as plt

latex_fonts_called = False 

def latex_fonts(double_coulmn=True):
    """
    Sets a bunch of parameters in matplotlib.pyplot to create nice figures.
    """

    global latex_fonts_called
    if latex_fonts_called:
        return 
    
    latex_fonts_called= True
    # \the\columnwidth\par -> 246.0pt
    # \the\textwidth\par   -> 510.pt

    # Default fig_width, for UiO master thesis template type 
    fig_width_pt = 418.25368  # Get this from LaTeX using \showthe\columnwidth

    if double_coulmn:   # Double column report
        fig_width_pt = 246 + 25
    
    inches_per_pt = 1.0/72.27               # Convert pt to inch
    golden_mean = (np.sqrt(5)-1.0)/2.0         # Aesthetic ratio
    fig_width = fig_width_pt * inches_per_pt  # width in inches
    fig_height = fig_width * golden_mean      # height in inches
    fig_size =  [fig_width,fig_height]

    plt.rcParams["text.latex.preamble"] = r"\usepackage{mathrsfs}"
    plt.rcParams["text.latex.preamble"] = r"\usepackage{siunitx}"
    
    plt.rcParams["text.usetex"] = True
    plt.rcParams["axes.titlepad"] = 10 
    plt.rcParams.update({
        "font.family": "euclid",
        "font.weight": "bold",
        "font.size": 10,       
        "axes.labelsize": 10,  
        "axes.titlesize": 10,  
        "xtick.labelsize": 10, 
        "ytick.labelsize": 10,
        "ytick.left": True,
        "xtick.bottom": True,
        "ytick.right": True,
        "xtick.top": True,
        "legend.fontsize": 10, 
        "figure.titlesize": 10,
        "legend.fontsize": 10,
        "figure.figsize": fig_size,
        "figure.subplot.bottom": 0.2,
    })
    # Ticks inside
    plt.rcParams["xtick.direction"] = "in"
    plt.rcParams["ytick.direction"] = "in"
    plt.rcParams["xtick.major.size"] = 4  
    plt.rcParams["ytick.major.size"] = 4
    plt.rcParams["xtick.minor.size"] = 2
    plt.rcParams["ytick.minor.size"] = 2

    plt.style.use('ggplot')

def combine_runs(DATA_DIR, L):
    """
    Combining observables from various simulations.
    """
    
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
    """
    Estimates the critical temperature at infinite lattice size using linear regression
    """
    x = 1 / L_vals
    y = T_vals

    slope, intercept, r_value, p_value, std_err = linregress(x, y)
    T_infinite = intercept
    return T_infinite, std_err
