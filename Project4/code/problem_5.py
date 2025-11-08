import numpy as np
import matplotlib.pyplot as plt
plt.style.use('ggplot')

L = 20
N = L * L

DIR = "out/"



colors = ['#377eb8', '#e41a1c', '#4daf4a', '#984ea3']
color_idx = 0

plt.figure(figsize=(6,4), tight_layout=True)
for type in ["unordered", "ordered"]:

    for T in [1.0, 2.4]:

        identifier = f"MCMC-{type}-L{L}-T{str(T).replace('.', 'p')}/"
        raw_data = np.loadtxt(DIR + identifier + "raw_data.csv", delimiter=",")

        n_cycles = raw_data[:, 0]
        eps = raw_data[:, 1] / N
        mean_eps = np.cumsum(eps) / n_cycles    

        plt.semilogx(n_cycles, eps, color=colors[color_idx], linewidth=1.0, alpha=0.4)
        plt.semilogx(n_cycles, mean_eps, color=colors[color_idx], linewidth=2.0, label=f'$T={T}$ $J/k_{{B}}$, {type}')

        color_idx += 1

plt.xlim([1, 1e4])
plt.xlabel("No. of MC cycles")
plt.ylabel("Energy per spin")
plt.legend()
plt.savefig(DIR + "equilibration.pdf")
