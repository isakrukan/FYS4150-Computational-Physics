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

    i = 0
    for key, data in kwargs.items():
    
        x = data[:,0]
        y = data[:,1]

        label = key.replace('_', ' ')
        plt.scatter(x[0], y[0], color=f"C{i}", zorder=10)  # Mark the start point
        plt.scatter(x[0], y[0], color=f"w", s=50, linewidths=1.5, zorder=9)  # Mark the end point
        plt.scatter(x[-1], y[-1], marker="x", color=f"C{i}", zorder=10)  # Mark the end point
        plt.scatter(x[-1], y[-1], marker="x", color=f"w", s=50, linewidths=2.5, zorder=9)  # Mark the end point
        i += 1
        plt.plot(x, y, label=label)

    plt.legend()
    

##########################
# Single particle tz plot
##########################
data = np.loadtxt('out/r1-single-RK4-n4000.txt')

plt.figure(figsize=(6,4))
plot_trajectories(Particle_1 = data[:,[0,3]],
                  xlabel = r'$t\ \ [\mu s]$',
                  ylabel = r'$z\ \ [\mu m]$')
plt.savefig('out/single-tz-RK4-n4000.pdf')

for n_steps in [4000]:
    for type in ['interacting', 'noninteracting']:
        
        if type == 'interacting':
            data1 = np.loadtxt(f'out/r1-interacting-RK4-n{n_steps}.txt')
            data2 = np.loadtxt(f'out/r2-interacting-RK4-n{n_steps}.txt')
            data1v = np.loadtxt(f'out/v1-interacting-RK4-n{n_steps}.txt')
            data2v = np.loadtxt(f'out/v2-interacting-RK4-n{n_steps}.txt')
        else:
            data1 = np.loadtxt(f'out/r1-single-RK4-n{n_steps}.txt')
            data2 = np.loadtxt(f'out/r2-single-RK4-n{n_steps}.txt')
            data1v = np.loadtxt(f'out/v1-single-RK4-n{n_steps}.txt')
            data2v = np.loadtxt(f'out/v2-single-RK4-n{n_steps}.txt')        

        #############################
        # Plot trajectories xy-plane
        ##############################
        plt.figure(figsize=(6,6))
        plot_trajectories(Particle_1 = data1[:,1:3],
                            Particle_2 = data2[:,1:3],
                            xlabel = r'$x\ \ [\mu m]$',
                            ylabel = r'$y\ \ [\mu m]$')
        plt.gca().set_aspect('equal', adjustable='box')
        plt.savefig(f'out/{type}-xy-RK4-n{n_steps}.pdf')

        #####################################
        # Plot x & z phase space trajectories
        #####################################
        for idx, label in [(1, 'x'), (3, 'z')]:
            temp_1 = np.array([data1[:,idx], data1v[:,idx]]).T
            temp_2 = np.array([data2[:,idx], data2v[:,idx]]).T
            plt.figure(figsize=(6,6))
            plot_trajectories(Particle_1 = temp_1, 
                                Particle_2 = temp_2,
                                xlabel = r'$' +      label   + r'\ \ [\mu m/\mu s]$',
                                ylabel = r'$' + f'v_{label}' + r'\ \ [\mu m/\mu s]$')
            plt.savefig(f'out/phasespace-{label}-{type}-RK4-n{n_steps}.pdf')