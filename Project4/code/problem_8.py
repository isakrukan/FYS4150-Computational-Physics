import numpy as np
import matplotlib.pyplot as plt
plt.style.use('ggplot')

DIR = "out/"

fig, ax = plt.subplots(nrows=4, sharex=True, figsize=(8, 10))
fig_err, ax_err = plt.subplots(nrows=2, sharex=True, figsize=(8, 5))

n = 100

for L in [40, 60, 80, 100]:
    
    data = np.loadtxt(DIR + f'temperature_scans/temp_scan-n{n}-L{L}.csv', delimiter=',')

    T = data[:, 0]
    E = data[:, 1]
    E_MSCE = data[:, 2]
    M = data[:, 3]
    M_MSCE = data[:, 4]
    C = data[:, 5]
    X = data[:, 6]

    M_rel_error = M_MSCE / np.abs(M)
    E_rel_error = E_MSCE / np.abs(E)

    # Plot main results, with error bands on E and M
    ax[0].plot(T, E, linestyle='-', label=f'L={L}')
    ax[1].plot(T, M, linestyle='-', label=f'L={L}')
    ax[2].plot(T, C, linestyle='-', label=f'L={L}')
    ax[3].plot(T, X, linestyle='-', label=f'L={L}')
    
    ax[0].fill_between(T, E - E_MSCE, E + E_MSCE, alpha=0.3)
    ax[1].fill_between(T, M - M_MSCE, M + M_MSCE, alpha=0.3)
    
    # Also plot error estimates separately
    ax_err[0].plot(T, E_rel_error, linestyle='-', label=f'L={L}')
    ax_err[1].plot(T, M_rel_error, linestyle='-', label=f'L={L}')


# Label axes and save figures
ax[0].legend()
ax[0].set_ylabel(r'$\langle \epsilon \rangle$')
ax[1].set_ylabel(r'$\langle |m| \rangle$')
ax[2].set_ylabel(r'$C_V/N$')
ax[3].set_ylabel(r'$\chi/N$')
ax[3].set_xlabel('Temperature $[k_B T / J]$')
fig.savefig(DIR + f'T_scan_n{n}.pdf')

ax_err[0].legend()
fig_err.suptitle('Estimated Relative Errors')
ax_err[0].set_ylabel(r'of $\langle \epsilon \rangle / N$')
ax_err[1].set_ylabel(r'of $\langle |m| \rangle / N$')
ax_err[1].set_xlabel('Temperature $[k_B T / J]$')
fig_err.savefig(DIR + f'T_scan_n{n}_error.pdf')
