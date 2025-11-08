import numpy as np
import matplotlib.pyplot as plt
plt.style.use('ggplot')

DIR = "out/"

use_repeat_runs = True

if use_repeat_runs:
    num_repeats = 6
    DATA_DIR = f"out/temperature_scans/repeat_runs_1e5/"

else:
    num_repeats = 1
    DATA_DIR = "out/temperature_scans/"

n = 100 # temperature steps in original data

fig, ax = plt.subplots(nrows=4, sharex=True, tight_layout=True, figsize=(6, 8))
fig_err, ax_err = plt.subplots(nrows=3, sharex=True, tight_layout=True, figsize=(6, 6))

sum_inv_var_E = np.zeros(n+1)
sum_inv_var_M = np.zeros(n+1)

for L in [40, 60, 80, 100]:
    data_sum = np.zeros((n+1, 9))

    for i in range(1, num_repeats + 1):

        if use_repeat_runs:
            filename = DATA_DIR + f'results-L{L}/scan_{i}.csv'
        else:
            filename = DATA_DIR + f'temp_scan-n{n}-L{L}.csv'

        data = np.loadtxt(filename.format(L=L, scan=i), delimiter=',')

        data_sum += data

        sum_inv_var_E += 1/data[:, 5]**2
        sum_inv_var_M += 1/data[:, 7]**2

    T = data_sum[:, 0] / num_repeats
    E = data_sum[:, 1] / num_repeats
    M = data_sum[:, 2] / num_repeats
    C = data_sum[:, 3] / num_repeats
    X = data_sum[:, 4] / num_repeats

    E_ESS  = data_sum[:, 6]*L**2
    M_ESS  = data_sum[:, 8]*L**2
    E_MSCE = 1/np.sqrt(sum_inv_var_E)
    M_MSCE = 1/np.sqrt(sum_inv_var_M)

    M_rel_error = M_MSCE / np.abs(M)
    E_rel_error = E_MSCE / np.abs(E)
    
    # Multiply errors to see better on plot
    #E_MSCE *= 50
    #M_MSCE *= 50

    # Plot main results, with error bands on E and M
    ax[0].fill_between(T, E - E_MSCE, E + E_MSCE, alpha=0.2)
    ax[1].fill_between(T, M - M_MSCE, M + M_MSCE, alpha=0.2)

    ax[0].plot(T, E, linestyle='-', label=f'L={L}')
    ax[1].plot(T, M, linestyle='-', label=f'L={L}')
    ax[2].plot(T, C, linestyle='-', label=f'L={L}')
    ax[3].plot(T, X, linestyle='-', label=f'L={L}')


    # Also plot error estimates separately
    ax_err[0].semilogy(T, E_rel_error, linestyle='-', label=f'L={L}')
    ax_err[1].semilogy(T, M_rel_error, linestyle='-', label=f'L={L}')

    line_E = ax_err[2].semilogy(T, E_ESS, label=f'L={L} ESS')
    line_M = ax_err[2].semilogy(T, M_ESS, linestyle='--', label=f'L={L} ESS', color=line_E[0].get_color(), alpha=0.5)

# Label axes and save figures
ax[0].set_ylabel(r'$\langle \epsilon \rangle$')
ax[1].set_ylabel(r'$\langle |m| \rangle$')
ax[2].set_ylabel(r'$C_V/N$')
ax[3].set_ylabel(r'$\chi/N$')
ax[3].set_xlabel('Temperature $[k_B T / J]$')
ax[3].set_xlim([2.1, 2.4])

ax[0].legend()
fig.savefig(DIR + f'temp_scan.pdf')

ax_err[0].set_ylabel(r'$\mathrm{MCSE}(\epsilon)$')
ax_err[1].set_ylabel(r'$\mathrm{MCSE}(|m|)$')
ax_err[2].legend()
ax_err[2].set_ylabel('ESS Estimates')
ax_err[2].set_xlabel('Temperature $[k_B T / J]$')
ax_err[2].set_xlim([2.1, 2.4])

linestyles_ESS = ['-', '--']
alphas_ESS = [1.0, 0.5]
handles = [plt.Line2D([0], [0], color='dimgray', alpha=alpha, linestyle=ls) for alpha, ls in zip(alphas_ESS, linestyles_ESS)]

ax_err[0].legend(loc='upper left')
ax_err[2].legend(handles, ['ESS(ε)', 'ESS(|m|)'])
fig_err.savefig(DIR + f'temp_scan_error.pdf')
