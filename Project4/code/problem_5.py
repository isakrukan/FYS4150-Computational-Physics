import numpy as np
import matplotlib.pyplot as plt
plt.style.use('ggplot')

L = 20
N = L * L

DIR = "out/"



colors = ['#377eb8', '#e41a1c', '#4daf4a', '#984ea3']
color_idx = 0

plt.figure(figsize=(9,6))
for type in ["unordered", "ordered"]:

    for T in [1.0, 2.4]:

        data = np.loadtxt(DIR + f"MCMC-{type}-L{L}-T{str(T).replace('.', 'p')}.csv", delimiter=",", skiprows=1)
        
        n_MC_cycles = data[:, 0]
        eps = data[:, 1] / N
        mean_eps = data[:, 2] / N   

        plt.semilogx(n_MC_cycles, eps, color=colors[color_idx], linewidth=1.0, alpha=0.4)
        plt.semilogx(n_MC_cycles, mean_eps, color=colors[color_idx], linewidth=2.0, label=f'$T={T}$ $J/k_{{B}}$, {type}')

        color_idx += 1


plt.xlabel("No. of MC cycles")
plt.ylabel("Energy per spin")
plt.legend()
plt.savefig(DIR + "equilibration.pdf")
