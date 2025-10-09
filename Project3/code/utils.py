import numpy as np
import matplotlib.pyplot as plt
import h5py

latex_fonts_called = False

def latex_fonts(double_coulmn=False):
    """
    Sets a bunch of parameters in matplotlib.pyplot to create nice figures.
    """

    global latex_fonts_called
    if latex_fonts_called:
        return 
    
    latex_fonts_called= True
    # \the\columnwidth\par -> 246.0pt
    # \the\textwidth\par   -> 510.pt

    # Default fig_width, for UiO master thesis template type 
    fig_width_pt = 418.25368  # Get this from LaTeX using \showthe\columnwidth

    if double_coulmn:   # Double column report
        fig_width_pt = 246 + 25
    
    inches_per_pt = 1.0/72.27               # Convert pt to inch
    golden_mean = (np.sqrt(5)-1.0)/2.0         # Aesthetic ratio
    fig_width = fig_width_pt * inches_per_pt  # width in inches
    fig_height = fig_width * golden_mean      # height in inches
    fig_size =  [fig_width,fig_height]

    plt.rcParams["text.latex.preamble"] = r"\usepackage{mathrsfs}"
    
    plt.rcParams["text.usetex"] = True
    plt.rcParams["axes.titlepad"] = 10 
    plt.rcParams.update({
        "font.family": "euclid",
        "font.weight": "bold",
        "font.size": 10,       
        "axes.labelsize": 10,  
        "axes.titlesize": 10,  
        "xtick.labelsize": 10, 
        "ytick.labelsize": 10,
        "ytick.left": True,
        "xtick.bottom": True,
        "ytick.right": True,
        "xtick.top": True,
        "legend.fontsize": 10, 
        "figure.titlesize": 10,
        "legend.fontsize": 10,
        "figure.figsize": fig_size,
        "figure.subplot.bottom": 0.2,
    })
    # Ticks inside
    plt.rcParams["xtick.direction"] = "in"
    plt.rcParams["ytick.direction"] = "in"
    plt.rcParams["xtick.major.size"] = 4  # adjust tick length if you want
    plt.rcParams["ytick.major.size"] = 4
    plt.rcParams["xtick.minor.size"] = 2
    plt.rcParams["ytick.minor.size"] = 2

def plot_trajectories(**kwargs):
    """
    Plots the trajectories of particles given their x and y coordinates.
    Possible keyword arguments:
    
    **kwargs:
        [....] (array): A 2D numpy array with shape (N, 2) representing coordinates of particle i. Key-name [....] will be used as the legend label, replacing underscores with spaces.
        xlabel (string): Label for the x-axis.
        ylabel (string): Label for the y-axis.
    """
    
    latex_fonts()
    fig, ax = plt.subplots()
    if "xlabel" in kwargs:
        ax.set_xlabel(kwargs.pop("xlabel"))

    if "ylabel" in kwargs:
        ax.set_ylabel(kwargs.pop("ylabel"))

    i = 0
    for key, data in kwargs.items():
    
        x = data[0]
        y = data[1]

        label = key.replace("_", " ")
        ax.scatter(x[0], y[0], color=f"C{i}", zorder=10)  # Mark the start point
        ax.scatter(x[0], y[0], color=f"w", s=50, linewidths=1.5, zorder=9)  # Mark the end point
        ax.scatter(x[-1], y[-1], marker="x", color=f"C{i}", zorder=10)  # Mark the end point
        ax.scatter(x[-1], y[-1], marker="x", color=f"w", s=50, linewidths=2.5, zorder=9)  # Mark the end point
        i += 1
        ax.plot(x, y, label=label)

    ax.legend()

def get_nsteps(filename:str):
    r"""
    @brief Returns the n_steps in a filename on the form "problem<x>-<...>-n<n_steps>-<position/velocity>.h5" 
    """
    tmp = filename.split("-")[-2] 
    tmp = int(tmp[1:])

    return tmp 

class AnalyticSolution:

    def __init__(self, r0:np.ndarray, v0:np.ndarray, q=1, B0=9.65*1e1, V0=2.41*1e6, d=500, m=40):
        r"""
        @brief Analytic solution to a single particle inside the Penning trap. 

        @param r0 Initial position
        @param v0 Initial position
        @param q Electric charge [\f$ e \f$] (elementary charge).
        @param m Mass [\f$ u \f$] (atomic mass unit).
        @param float B_0 Magnetic field strength [\f$ u/(\mu s)e^{-1} \f$] (atomic mass unit per microsend per electric charge).
        @param float V_0 Applied potential [\f$ u/(\mu s)^2e^{-1} \f$] (atomic mass unit per microsend squared per electric charge).
        @param d Characteristic dimension [\f$ \mu m \f$] (micrometer).
        """
        self.omega0 = q*B0/m; self.omegaz_2 = 2*q*V0 / (m*d**2)

        tmp = np.sqrt(self.omega0 - 2*self.omegaz_2)
        self.omega_pluss = (self.omega0 + tmp) / 2; self.omega_minus = (self.omega0 - tmp) / 2
        self.A_pluss = (v0[1] + self.omega_minus*r0[0]) / (self.omega_minus - self.omega_pluss)
        self.A_minus = (v0[1] + self.omega_pluss*r0[0]) / (self.omega_pluss - self.omega_minus)

        self.r0 = r0; self.v0 = v0

    def __call__(self, t):
        f_t = self.A_pluss * np.exp(-1j*(self.omega_pluss * t)) + self.A_minus * np.exp(-1j*(self.omega_minus * t))
        return np.array([np.real(f_t), np.imag(f_t), self.r0[2]*np.cos(np.sqrt(self.omegaz_2))*t])


def problem8(
        single_particle="p1-RK4-int-n4000-r.h5",
        two_particles_pos=["p2-RK4-nonint-n4000-r.h5", "p2-RK4-int-n4000-r.h5"],
        two_particles_vel=["p2-RK4-nonint-n4000-v.h5", "p2-RK4-int-n4000-v.h5"],
        analytic_single=["p1-RK4-int-n4000-r.h5", 
                         "p1-RK4-int-n8000-r.h5", 
                         "p1-RK4-int-n16000-r.h5", 
                         "p1-RK4-int-n32000-r.h5"], 
        path = "out/",
        T=50):
    r"""
    @brief Creates three plots for simulation of the Penning trap with static E-field.

    @details Assumptions:
        - Filenames are of the form:
                '<details separated by '-'>-p<# particles>-<method>-<int/nonint>-n<time steps>-<'r' or 'v'> 
        - @ref two_particles_pos "two_particles_pos" and @ref two_particles_vel "two_particles_vel" are 
            assumed to stem from the same simulation. 

    @details Generates the following:
        1. Motion of single particle in z-direction as a function of time.
        2. Motion of two particles in (x,y)-plane with and without particle interactions.
        3. Phase space trajectories of the two particles in 2.
        4. Relative error for simulations time steps in {4000, 8000, 16000, 32000}.

    Parameters:
    @param single_particle Filename (.h5) for single particle simulation.
    @param two_particles_pos List of filenames (.h5) for position data for non-interaction and interaction simulation for two-particles.
    @param two_particles_vel List of filenames (.h5) for velocity data for non-interaction and interaction simulation for two-particles.
    @param analytic_single List of filenames (.h5), for different simulations of single particle.  
    @param path Default path to files.
    @param T Duration of simulation, in units \f$\mu s\f$.
    """
    # Reading files:
    n1_step = get_nsteps(single_particle)
    with h5py.File(path + single_particle, "r") as f:
        data1 = np.array(f["dataset"]).transpose(2,1,0) # Apparently, axis 0 and 2 are 'flipped' in some part of this process

    n2_steps = []; data2_pos = []; data2_vel = []
    for filename_pos, filename_vel in zip(two_particles_pos, two_particles_vel):
        with h5py.File(path + filename_pos, "r") as f:
            n2_steps.append(get_nsteps(filename_pos))
            tmp = np.array(f["dataset"]).transpose(2,1,0) # Apparently, axis 0 and 2 are 'flipped' in some part of this process
            data2_pos.append(tmp)
        
        with h5py.File(path + filename_vel, "r") as f:
            tmp = np.array(f["dataset"]).transpose(2,1,0) # Apparently, axis 0 and 2 are 'flipped' in some part of this process
            data2_vel.append(tmp)

    n3_steps, data3 = [], []
    for filename in analytic_single:
        with h5py.File(path + filename, "r") as f:
            n3_steps.append(get_nsteps(filename))
            tmp = np.array(f["dataset"]).transpose(2,1,0) # Apparently, axis 0 and 2 are 'flipped' in some part of this process
            data3.append(tmp)

    ### 1. (t,z)-plot ###
    t = np.linspace(0, T, n1_step)
    z = data1[0,:, 2]
    plot_trajectories(single_particle = [t, z],
                            xlabel = r"$t\ \ [\mu s]$",
                            ylabel = r"$z\ \ [\mu m]$")
    plt.gca().set_aspect("equal", adjustable="box")
    plt.savefig(path + f"problem8-single-n{n1_step}.pdf")

    ### 2. Motion in (x,y)-plane ###
    ## Non-interacting:
    # Here, data2[0] gives non-interacting data set of shape (particle, time_step, (x,y,z))
    p1_xy = [data2_pos[0][0,:,0], data2_pos[0][0,:,1]]; p2_xy = [data2_pos[0][1,:,0], data2_pos[0][1,:,1]]
    plot_trajectories(Particle_1 = p1_xy,
                      Particle_2 = p2_xy,
                            xlabel = r"$x\ \ [\mu m]$",
                            ylabel = r"$y\ \ [\mu m]$")
    plt.gca().set_aspect("equal", adjustable="box")
    plt.savefig(path + f"problem8-double-nonint-n{n2_steps[0]}.pdf")

    ## Interacting:
    # Here, data2[1] gives interacting data set of shape (particle, time_step, (x,y,z))
    p1_xy = [data2_pos[1][0,:,0], data2_pos[0][0,:,1]]; p2_xy = [data2_pos[1][1,:,0], data2_pos[0][1,:,1]]
    plot_trajectories(Particle_1 = p1_xy,
                      Particle_2 = p2_xy,
                            xlabel = r"$x\ \ [\mu m]$",
                            ylabel = r"$y\ \ [\mu m]$")
    plt.gca().set_aspect("equal", adjustable="box")
    plt.savefig(path + f"problem8-double-nonint-n{n2_steps[1]}.pdf")

    ### 3. Phase space (x,v_x) and (z, v_z) ###
    ## Non-interacting ##
    # Here, data2[0] gives non-interacting data set of shape (particle, time_step, (x,y,z))
    p1_x_vx = [data2_pos[0][0,:,0], data2_vel[0][0,:,0]]; p2_x_vx = [data2_pos[0][1,:,0], data2_vel[0][1,:,0]]
    plot_trajectories(Particle_1 = p1_x_vx,
                      Particle_2 = p2_x_vx,
                            xlabel = r"$x\ \ [\mu m]$",
                            ylabel = r"$v_x\ \ [\mu m]$")
    plt.gca().set_aspect("equal", adjustable="box")
    plt.savefig(path + f"problem8-double-nonint-n{n2_steps[0]}.pdf")

    ## Interacting ##
    # Here, data2[1] gives interacting data set of shape (particle, time_step, (x,y,z))
    p1_z_vz = [data2_pos[0][0,:,2], data2_vel[0][0,:,2]]; p2_z_vz = [data2_pos[0][1,:,2], data2_vel[0][1,:,2]]
    plot_trajectories(Particle_1 = p1_z_vz,
                      Particle_2 = p2_z_vz,
                            xlabel = r"$z\ \ [\mu m]$",
                            ylabel = r"$v_z\ \ [\mu m]$")
    plt.gca().set_aspect("equal", adjustable="box")
    plt.savefig(path + f"problem8-double-nonint-n{n2_steps[1]}.pdf")

    ### 4. Relative error ###
    pos_analytic = AnalyticSolution([20, 0, 20], [0, 25, 0])
    fig, ax = plt.subplots()
    for i in range(len(n3_steps)):
        t = np.linspace(0, T, n3_steps[i])
        computed = data3[i][0]; analytic = pos_analytic(t).T
        rel_error = np.linalg.norm(analytic - computed, axis=1) / np.linalg.norm(analytic, axis=1)
        ax.plot(t, rel_error, label=rf"n={n3_steps[i]}")
    ax.legend()
    plt.savefig(path + f"problem8-rel-errors.pdf")
    plt.show()