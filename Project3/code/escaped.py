import h5py
import numpy as np
from plot_trajectories import plot_trajectories
import matplotlib.pyplot as plt
plt.style.use('ggplot')
    
    
with h5py.File('out/p10-RK4-int-n4000.h5', 'r') as dataset:
    # Load the data from the HDF5 file
    times = np.array(dataset['times'])[0,:].T
    positions = np.array(dataset['positions']).T
    velocities = np.array(dataset['velocities']).T


n_particles = positions.shape[0]


for particle in range(n_particles):
    traj = positions[particle, :, :]

    plot_trajectories(**{f'Particle_{particle+1}': traj,
                         'xlabel': r'$x\ \ [\mu m]$',
                         'ylabel': r'$y\ \ [\mu m]$'})
    plt.title(f'Trajectory of Particle {particle+1} in xy-plane')
plt.show()    

d = 500

print(positions.shape)

print(positions)
escaped = 0


escaped_array = np.zeros(len(times))

radius = np.linalg.norm(positions[:, :, :], axis=2)
for step, t in enumerate(times):

    escaped_mask = np.where(radius[:, step] > d, 1, 0)

    escaped_array[step] = np.sum(escaped_mask)


plt.figure(figsize=(6,4))
plt.plot(times, escaped_array, label='Escaped particles')
plt.xlabel(r'$t\ \ [\mu s]$')
plt.ylabel('Number of escaped particles')
plt.legend()
plt.show()



escaped = np.sum(radius[:, -1] > d)

print(escaped)