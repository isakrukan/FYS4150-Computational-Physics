import h5py
import numpy as np
from plot_trajectories import plot_trajectories
import matplotlib.pyplot as plt
plt.style.use('ggplot')

plt.figure(figsize=(6,4))

for f in [0.1, 0.4, 0.7]:
    
    f_str = f"{f:.1f}".replace('.', '')
    with h5py.File(f'out/n_escaped-p100-f{f_str}-RK4-int-n4000.h5', 'r') as dataset:
        data = np.array(dataset['escape_data'])

    frequencies = data[:, 0]
    fraction_inside = 1 - data[:, 1] / 100  # since we simulated 100 particles

    plt.plot(frequencies, fraction_inside, label=f'f={f}')

plt.xlabel(r'$\omega\ \ [MHz]$')
plt.ylabel('Fraction of particles inside the trap')
plt.legend()
plt.savefig('out/trapped_fraction.pdf')

