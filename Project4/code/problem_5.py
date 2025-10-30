import numpy as np
import matplotlib.pyplot as plt
plt.style.use('ggplot')

L = 20
N = L * L

DIR = "out/"

data_T10_u = np.loadtxt(DIR + "random-L20-T1p0.csv", delimiter=",", skiprows=1)
data_T10_o = np.loadtxt(DIR + "ordered-L20-T1p0.csv", delimiter=",", skiprows=1)
data_T24_u = np.loadtxt(DIR + "random-L20-T2p4.csv", delimiter=",", skiprows=1)
data_T24_o = np.loadtxt(DIR + "ordered-L20-T2p4.csv", delimiter=",", skiprows=1)


n_MC_cycles = data_T10_u[:, 0]
eps_T10_u   = data_T10_u[:, 1] / N
eps_T10_o   = data_T10_o[:, 1] / N
eps_T24_u   = data_T24_u[:, 1] / N
eps_T24_o   = data_T24_o[:, 1] / N
mean_eps_T10_u = data_T10_u[:, 2] / N
mean_eps_T10_o = data_T10_o[:, 2] / N
mean_eps_T24_u = data_T24_u[:, 2] / N
mean_eps_T24_o = data_T24_o[:, 2] / N

plt.figure(figsize=(9,6))
plt.semilogx(n_MC_cycles, eps_T10_u, '-', color='#377eb8', alpha=0.4, linewidth=1.0)
plt.semilogx(n_MC_cycles, eps_T10_o, '-', color='#4daf4a', alpha=0.4, linewidth=1.0)
plt.semilogx(n_MC_cycles, eps_T24_u, '-', color='#e41a1c', alpha=0.4, linewidth=1.0)
plt.semilogx(n_MC_cycles, eps_T24_o, '-', color='#984ea3', alpha=0.4, linewidth=1.0)
plt.semilogx(n_MC_cycles, mean_eps_T10_u, '-', linewidth=2.0, color='#377eb8', label='$T=1.0$ $J/k_{B}$, unordered')
plt.semilogx(n_MC_cycles, mean_eps_T10_o, '-', linewidth=2.0, color='#4daf4a', label='$T=1.0$ $J/k_{B}$, ordered')
plt.semilogx(n_MC_cycles, mean_eps_T24_u, '-', linewidth=2.0, color='#e41a1c', label='$T=2.4$ $J/k_{B}$, unordered')
plt.semilogx(n_MC_cycles, mean_eps_T24_o, '-', linewidth=2.0, color='#984ea3', label='$T=2.4$ $J/k_{B}$, ordered')

plt.xlabel("No. of MC cycles")
plt.ylabel("Energy per spin")
plt.legend()
plt.savefig(DIR + "equilibration.pdf")