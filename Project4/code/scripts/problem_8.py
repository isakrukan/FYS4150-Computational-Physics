import sys
import os
import numpy as np
import matplotlib.pyplot as plt
plt.style.use('ggplot')
import utils

# Set directories
DIR = "out/"
OUT_DIR = "../figs/"
DATA_DIR = DIR + "temperature_scans/repeat_runs/"

num_temps = 101                                # Number of temperature points
lattice_sizes = np.array([40, 60, 80, 100])    # Lattice sizes used

# Checking whether full-scaled runs exists (which we create using bioint01 and 'run_repeat_trials.sh')
for i, L in enumerate(lattice_sizes):
    filename = DATA_DIR + f"/results-L{L}"
    if not os.path.exists(os.path.join(os.getcwd(), filename)): # If not, switch to downscaled versions
        DATA_DIR = DIR + "temperature_scans/tests/"
        print(f"WARNING: Default data directory not found. Using test data: {DATA_DIR}")
        break 

# Creating figures for main results + error estimates
fig, ax = plt.subplots(nrows=4, sharex=True, tight_layout=True, figsize=(6, 8))
fig_err, ax_err = plt.subplots(nrows=2, sharex=True, constrained_layout=True, figsize=(6, 4))

for i, L in enumerate(lattice_sizes):
    N = L * L
    
    # --- Loading data ---
    data = utils.combine_runs(DATA_DIR, L)
    
    T = data[:, 0]
    E_avg = data[:, 1]
    E_var = data[:, 2]
    M_avg = data[:, 3]
    M_var = data[:, 4]
    
    E_MCSE = data[:, 5]
    E_ESS  = data[:, 6]
    M_MCSE = data[:, 7]
    M_ESS  = data[:, 8]
    
    C = (E_var) * N / (T * T) 
    X = (M_var) * N / T
    
    # --- Plotting main results ---
    err_scale = 10
    
    ax[0].fill_between(T, E_avg - err_scale * E_MCSE, E_avg + err_scale * E_MCSE, alpha=0.2)
    ax[1].fill_between(T, M_avg - err_scale * M_MCSE, M_avg + err_scale * M_MCSE, alpha=0.2)
    ax[0].plot(T, E_avg, linestyle='-', label=f'L={L}')
    ax[1].plot(T, M_avg, linestyle='-', label=f'L={L}')
    ax[2].plot(T, C, linestyle='-', label=f'L={L}')
    ax[3].plot(T, X, linestyle='-', label=f'L={L}')


    # --- Plotting error estimates ---
    M_rel_error = M_MCSE / np.abs(M_avg)
    E_rel_error = E_MCSE / np.abs(E_avg)
    
    ax_err[0].semilogy(T, E_rel_error)
    ax_err[1].semilogy(T, E_ESS)

    color = ax_err[0].lines[-1].get_color()
    ax_err[0].semilogy(T, M_rel_error, linestyle='--', color=color, alpha=0.5)
    ax_err[1].semilogy(T, M_ESS, linestyle='--', color=color, alpha=0.5)



# --- Main plots ---:
ax[0].set_ylabel(r'$\langle \epsilon \rangle$')
ax[1].set_ylabel(r'$\langle |m| \rangle$')
ax[2].set_ylabel(r'$C_V/N$')
ax[3].set_ylabel(r'$\chi/N$')
ax[3].set_xlabel('Temperature $[k_B T / J]$')
ax[3].set_xlim([2.1, 2.4])
ax[0].legend()
fig.savefig(OUT_DIR + f'temp_scan.pdf')


# --- Error plots ---:
ax_err[0].set_ylabel('MCSE')
ax_err[1].set_ylabel('ESS')
ax_err[1].set_xlabel('Temperature $[k_B T / J]$')
ax_err[1].set_xlim([2.1, 2.4])

# Creating legends manually:

# -----------------------------------
# LEGEND #2: colors (lattice size L)
# -----------------------------------
handles_color = [plt.Line2D([0], [0], color=f"C{i}") for i in range(len(lattice_sizes))]
labels_color = [f"$L = {L}$" for L in lattice_sizes]

legend_color = ax_err[0].legend(handles_color, 
                                labels_color,
                                framealpha=0,
                                title="Lattice size",
                                loc="upper left",
                                bbox_to_anchor=(1.0, 1.07))

legend_color._legend_box.align = "left"
 
# ---------------------------------
# LEGEND #1: linestyles (epsilon vs |m|)
# ---------------------------------
handles_style = [plt.Line2D([0], [0], linestyle='-',  color='black', alpha=1.0),
                 plt.Line2D([0], [0], linestyle='--', color='black', alpha=0.5)]
labels_style = [r'$\epsilon$', r'$|m|$']

legend_style = ax_err[1].legend(handles_style, 
                                labels_style, 
                                framealpha=0,
                                title="Observable",
                                loc="upper left",
                                bbox_to_anchor=(1.0, 1.5),
                                )

ax_err[1].add_artist(legend_style)

fig_err.savefig(OUT_DIR + f'temp_scan_error.pdf')