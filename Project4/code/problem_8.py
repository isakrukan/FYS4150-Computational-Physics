import numpy as np
import matplotlib.pyplot as plt
plt.style.use('ggplot')
from scipy.signal import savgol_filter


DIR = "out/"

fig, ax = plt.subplots(nrows=4, sharex=True, figsize=(8, 10))

n = 100

for L in [40, 60, 80, 100]:
    data = np.loadtxt(DIR + f'T_scan-n{n}-L{L}.csv', delimiter=',')

    T = data[:, 0]
    E = data[:, 1]
    M = data[:, 2]
    C = data[:, 3]
    X = data[:, 4]

    
    ax[0].plot(T, E, linestyle='-', label=f'L={L}')
    ax[1].plot(T, M, linestyle='-', label=f'L={L}')
    ax[2].plot(T, C, linestyle='-', label=f'L={L}')
    ax[3].plot(T, X, linestyle='-', label=f'L={L}')

ax[0].set_ylabel(r'$\langle \epsilon \rangle$')
ax[1].set_ylabel(r'$\langle |m| \rangle$')
ax[2].set_ylabel(r'$C_V/N$')
ax[3].set_ylabel(r'$\chi/N$')
ax[3].set_xlabel('Temperature $[k_B T / J]$')


plt.legend()
plt.savefig(DIR + f'T_scan_n{n}.pdf')
