import numpy as np
import matplotlib.pyplot as plt
plt.style.use('ggplot')

def plot_trajectories(**kwargs):
    """
    Plots the trajectories of particles given their x and y coordinates.
    Possible keyword arguments:
    
    **kwargs:
        [....] (array): A 2D numpy array with shape (N, 2) representing coordinates of particle i. Key-name [....] will be used as the legend label, replacing underscores with spaces.
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
    

###################
# Single particle
##################
data = np.loadtxt('out/r1-single-n4000.txt')

plt.figure(figsize=(6,4))
plot_trajectories(Particle_1 = data[:,[0,3]],
                  xlabel = r'$t\ \ [\mu s]$',
                  ylabel = r'$z\ \ [\mu m]$')
plt.savefig('out/single-tz-n4000.pdf')

################################
# Two non-interacting particles
################################
data1 = np.loadtxt('out/r1-single-n4000.txt')
data2 = np.loadtxt('out/r2-single-n4000.txt')

plt.figure(figsize=(6,6))

plot_trajectories(Particle_1 = data1[:,1:3],
                  Particle_2 = data2[:,1:3],
                  xlabel = r'$x\ \ [\mu m]$',
                  ylabel = r'$y\ \ [\mu m]$')

plt.gca().set_aspect('equal', adjustable='box')
plt.savefig('out/noninteracting-xy-n4000.pdf')


############################
# Two interacting particles
############################

data1 = np.loadtxt('out/r1-interacting-n4000.txt')
data2 = np.loadtxt('out/r2-interacting-n4000.txt')
plt.figure(figsize=(6,6))

plot_trajectories(Particle_1 = data1[:,1:3], 
                  Particle_2 = data2[:,1:3],
                  xlabel = r'$x\ \ [\mu m]$',
                  ylabel = r'$y\ \ [\mu m]$')

plt.gca().set_aspect('equal', adjustable='box')
plt.savefig('out/interacting-xy-n4000.pdf')

