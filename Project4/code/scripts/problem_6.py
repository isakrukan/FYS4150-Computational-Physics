import numpy as np
import matplotlib.pyplot as plt
from tabulate import tabulate
plt.style.use('ggplot')

# NOTE: In order to reuse data generated in Problem 5, we # of burn-in 
# cycles is set to 0 in the raw data files. This means the burn-in cycles 
# must be discarded manually here, before doing the analysis.


DIR = "out/"                # data location
OUT_DIR = "../figs/"        # output directory

L = 20                      # lattice size
N = L * L                   # number of spins
burn_in_cycles = 10000      # number of burn-in cycles to discard
bin_width = 0.01            # fixed bin width used for both temperatures
variances = []              # to store variance results for later



# Create histograms of epsilon for T=1.0 and T=2.4:
fig, ax = plt.subplots(nrows=2, figsize=(6,4), sharex=True)

for i, T in enumerate([1.0, 2.4]):

    # as a consistency check, plot both ordered and unordered initial configurations
    for j, type in enumerate(["unordered", "ordered"]):
        
        # Load data
        identifier = f"MCMC-{type}-L{L}-T{str(T).replace('.', 'p')}/"
        filename = DIR + identifier + "raw_data.csv"
        data = np.loadtxt(filename, delimiter=",")
        
        # Extract epsilon values & compute variance, discarding burn-in:
        epsilon = data[burn_in_cycles:, 1] / N
        variances.append(np.var(epsilon))
        
        # Create histogram
        bins = np.arange(epsilon.min() - bin_width/2, epsilon.max() + bin_width/2, bin_width)

        ax[i].hist(epsilon, bins=bins, density=True, alpha=0.7, label=type.capitalize(), color=f"C{j}", histtype='stepfilled')
        ax[i].legend(title=rf"T = {T} ${{J/k_B}}$")
        

# Finalize & save figure
fig.supxlabel("Energy per spin")
fig.supylabel("Probability density")
plt.savefig(OUT_DIR + "epsilon_distribution.pdf")



# Print variances in a table:

data = [ ["T = 1.0 J/k_B", *variances[0:2]], 
         ["T = 2.4 J/k_B", *variances[2:4]] ]

headers = ["", "unordered", "ordered"]

title = f"Variance of eps for L={L} over 100,000 MC w/ burn-in of 10,000"

table = tabulate(data, headers=headers, tablefmt="rounded_grid")

print("\n--- Problem 6 ---\n")
print(f"{title}\n")
print(table)