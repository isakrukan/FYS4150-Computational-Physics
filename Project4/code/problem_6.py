import numpy as np
import matplotlib.pyplot as plt
from tabulate import tabulate
plt.style.use('ggplot')

DIR = "out/"                # output directory & data location

L = 20                      # lattice size
N = L * L                   # number of spins
burnin_cycles = 1000        # number of MC before sampling
bin_width = 0.02            # fixed bin width used for both temperatures

variances = []


# Create histograms of epsilon after burn-in for T=1.0 and T=2.4:

fig, ax = plt.subplots(nrows=2, figsize=(9,6), sharex=True)

for i, T in enumerate([1.0, 2.4]):

    # as a consistency check, plot both ordered and unordered initial configurations
    for j, type in enumerate(["unordered", "ordered"]):
        filename = DIR + f"MCMC-{type}-L20-T{str(T).replace('.', 'p')}.csv"
        epsilon = np.loadtxt(filename, delimiter=",", skiprows=1)[burnin_cycles:, 1] / N
        bins = np.arange(epsilon.min() - bin_width/2, epsilon.max() + bin_width/2, bin_width)

        ax[i].hist(epsilon, bins=bins, density=True, alpha=0.7, label=type.capitalize(), color=f"C{j}")
        leg = ax[i].legend(title=f"T = {T} $\mathbf{{J/k_{{B}}}}$")
        leg.get_title().set_fontweight('bold')
        # compute variance of epsilon and store for later use
        variances.append(np.var(epsilon))
        

fig.supxlabel("Energy per spin")
fig.supylabel("Probability density")
plt.savefig(DIR + "epsilon_distribution.pdf")



# Print variance results in a table:

data = [ ["T = 1.0 J/k_B", *variances[0:2]], 
         ["T = 2.4 J/k_B", *variances[2:4]] ]

headers = ["", "unordered", "ordered"]

title = f"Variance of ε after burn-in ({burnin_cycles} cycles)"

table = tabulate(data, headers=headers, tablefmt="rounded_grid")


print(f"\n{title}\n")
print(table)