import numpy as np
import matplotlib.pyplot as plt
plt.style.use('ggplot')

DIR = "out/"

fig, ax = plt.subplots(nrows=4, sharex=True, figsize=(8, 10))

n = 20

for L in [40, 60, 80, 100]:
    data = np.loadtxt(DIR + f'temp_scan-n{n}-L{L}.csv', delimiter=',')

    T = data[:, 0]
    E = data[:, 1]
    E_MSCE = data[:, 2]
    M = data[:, 3]
    M_MSCE = data[:, 4]
    C = data[:, 5]
    X = data[:, 6]



    ax[0].plot(T, E, linestyle='-', label=f'L={L}')
    ax[1].plot(T, M, linestyle='-', label=f'L={L}')
    ax[0].fill_between(T, E - E_MSCE, E + E_MSCE, alpha=0.3)
    ax[1].fill_between(T, M - M_MSCE, M + M_MSCE, alpha=0.3)
    ax[2].plot(T, C, linestyle='-', label=f'L={L}')
    ax[3].plot(T, X, linestyle='-', label=f'L={L}')



ax[0].set_ylabel(r'$\langle \epsilon \rangle$')
ax[1].set_ylabel(r'$\langle |m| \rangle$')
ax[2].set_ylabel(r'$C_V/N$')
ax[3].set_ylabel(r'$\chi/N$')
ax[3].set_xlabel('Temperature $[k_B T / J]$')


plt.legend()
plt.savefig(DIR + f'T_scan_n{n}.pdf')
