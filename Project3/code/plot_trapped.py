import h5py
import numpy as np
from plot_trajectories import plot_trajectories
import matplotlib.pyplot as plt
plt.style.use('ggplot')


def plot_trapped(filename, amplitudes=[0.1,0.4,0.7], frequency_range=None, linestyle=None, colors=None, alpha=1.0):

    if not linestyle:
        linestyle = '-'

    if not colors:
        colors = [f'C{i}' for i in range(len(amplitudes))]

    if "$(FWILD)" not in filename:
        with h5py.File(filename, 'r') as dataset:
            data = np.array(dataset['escape_data'])
        
        frequencies = data[:, 0]
        fraction_inside = 1 - data[:, 1] / 100  # since we
        plt.plot(frequencies, fraction_inside, label=f'f={f}')

    else:
        for i, f in enumerate(amplitudes):
            filename_temp = filename.replace('$(FWILD)', f"{f:.1f}".replace('.', ''))

            with h5py.File(filename_temp, 'r') as dataset:
                data = np.array(dataset['escape_data'])

            frequencies = data[:, 0]
            fraction_inside = 1 - data[:, 1] / 100  # since we simulated 100 particles

            plt.plot(frequencies, fraction_inside, label=f'f={f}', linestyle=linestyle, color=colors[i], alpha=alpha)

    if frequency_range:
        plt.xlim(frequency_range)
    else:
        plt.xlim(frequencies.min(), frequencies.max())
    
    plt.xlabel(r'$\omega\ \ [MHz]$')
    plt.ylabel('Fraction of particles inside the trap')


if __name__ == "__main__":
    import os

    OUTDIR = 'plots/'
    if not os.path.exists(OUTDIR):
        os.makedirs(OUTDIR)
        
        
    plt.figure(figsize=(6,4))
    plot_trapped('out/n_escaped-p100-f$(FWILD)-RK4-int-n4000.h5')
    plt.savefig(OUTDIR + 'trapped_fraction.pdf')
    plt.legend()

    plt.figure(figsize=(6,4))
    linestyles = ['--', '-']
    types = ['int', 'nonint']
    alphas = [0.7, 1.0]
    for i in range(2):
        plot_trapped(f'out/n_escaped-p100-f$(FWILD)-RK4-{types[i]}-n4000.h5', frequency_range=(1, 1.7), linestyle=linestyles[i], colors=['C0', 'C1', 'C2'], alpha=alphas[i])

    plt.legend(['','','','f=0.1', 'f=0.4', 'f=0.7'],ncol=2, columnspacing=-0.3, loc='lower left')
    plt.savefig(OUTDIR + 'trapped_fraction_zoomed.pdf')