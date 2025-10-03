import h5py
import numpy as np
import matplotlib.pyplot as plt

path = "out/"
with h5py.File(path + "problem8_interacting_r.h5", "r") as f:
    print(list(f.keys()))       # ['dataset']
    data = np.array(f["dataset"])
    

def plot_trajectories(**kwargs):
    """
    Plots the trajectories of particles given their x and y coordinates.
    Possible keyword arguments:
    
    **kwargs:
        [....] (array): A 2D numpy array with shape (N, 2) representing coordinates of particle i. 
        Key-name [....] will be used as the legend label, replacing underscores with spaces.
        xlabel (string): Label for the x-axis.
        ylabel (string): Label for the y-axis.
    """
        
    if 'xlabel' in kwargs:
        plt.xlabel(kwargs.pop('xlabel'))

    if 'ylabel' in kwargs:
        plt.ylabel(kwargs.pop('ylabel'))

    for key, data in kwargs.items():
    
        x = data[:,0]
        y = data[:,1]

        label = key.replace('_', ' ')
        plt.plot(x, y, label=label)

    plt.legend()


path = "out/"
with h5py.File(path + "problem8_interacting_r.h5", "r") as f:
    data = np.array(f["dataset"])

num_particles, n_steps, dim = np.shape(data)

print(dim)

plt.figure(figsize=(6,4))
for n in range(num_particles):
    plot_trajectories(Particle_1 = data[n,:,:],
                    xlabel = r'$t\ \ [\mu s]$',
                    ylabel = r'$z\ \ [\mu m]$')
plt.savefig('out/test.pdf')