import h5py
import numpy as np
import matplotlib.pyplot as plt 
from matplotlib import rc
from matplotlib.axes import Axes
from matplotlib.figure import Figure
from matplotlib.animation import FuncAnimation
import matplotlib.cm as cm
import scienceplots


def read_hd5_data(filename_and_path:str) -> dict:
    """
    Reads the data stored at "filename_and_path" and returns dictionary containing M, N, h, dt, v0, t and u.
    """
    tmp = {}
    with h5py.File(filename_and_path, "r") as f:

        # Parameters:
        tmp["M"] = int(f["M"][0][0])
        tmp["N"] = int(f["N"][0][0])
        tmp["h"] = float(f["h"][0][0])
        tmp["dt"] = float(f["dt"][0][0])
        tmp["v0"] = float(f["v0"][0][0])

        # Arrays:
        t = np.array(f["t"][:]).reshape(-1)   # full 1D array of length N

        u = np.array(f["u"][:])

        u_raw = np.array(f["u"][...])      # shape (M*M, N)

        # Converting from (x,y) to x + iy:
        real_id, imag_id = u_raw.dtype.names
        u_complex = u_raw[real_id] + 1j * u_raw[imag_id]  

        u_complex = u_complex.T                             # (N, M*M)
        u = u_complex.reshape(-1, tmp["M"], tmp["M"])       # (N, M_y, M_x)
        u = u.transpose(0, 2, 1)                    # (N, M_y, M_x)

        # Storing:
        tmp["t"] = t
        tmp["u"] = u      

        

    return tmp

def generate_animation(ax:Axes, fig:Figure, data:dict):
    h = data["h"]; dt = data["dt"]
    M = data["M"]
    
    x_points = np.arange(0, 1+h, h)
    y_points = np.arange(0, 1+h, h)


    x_points = np.linspace(0,1,M)
    y_points = np.linspace(0,1,M)

    x, y = np.meshgrid(x_points, y_points, sparse=True)

    #
    t = data["t"]
    u = data["u"]
    u_squared = [np.abs(un)**2 for un in u]

    # Some settings
    fontsize = 12
    t_min = t[0]
    x_min, x_max = x_points[0], x_points[-1]
    y_min, y_max = y_points[0], y_points[-1]

    # Create a colour scale normalisation according to the max z value in the first frame
    norm = cm.colors.Normalize(vmin=0.0, vmax=np.max(u_squared[0]))

    # Plot the first frame
    img = ax.imshow(u_squared[0], extent=[x_min,x_max,y_min,y_max], cmap=plt.get_cmap("viridis"), norm=norm)
    # plt.show()
    # Axis labels
    plt.xlabel("x", fontsize=fontsize)
    plt.ylabel("y", fontsize=fontsize)
    plt.xticks(fontsize=fontsize)
    plt.yticks(fontsize=fontsize)

    # Add a colourbar
    cbar = fig.colorbar(img, ax=ax)
    cbar.set_label("z(x,y,t)", fontsize=fontsize)
    cbar.ax.tick_params(labelsize=fontsize)

    # Add a text element showing the time
    time_txt = plt.text(0.95, 0.95, f"t = {t_min:.3e}", color="white", 
                        horizontalalignment="right", verticalalignment="top", fontsize=fontsize)

    # Function that takes care of updating the z data and other things for each frame
    def animation(i):
        # Normalize the colour scale to the current frame?
        norm = cm.colors.Normalize(vmin=0.0, vmax=np.max(u_squared[i]))
        img.set_norm(norm)

        # Update z data
        img.set_data(u_squared[i])

        # Update the time label
        time_txt.set_text(f"t = {t[i]:.3e}")

        return img

    # Use matplotlib.animation.FuncAnimation to put it all together
    ani = FuncAnimation(fig, animation, interval=1, frames=np.arange(0, len(u), 2), repeat=False, blit=0)
    
    return ani

def plot_colormap(data:dict, t0:float, f:callable, 
                  z_label=None, ax=None, fig=None, title=None, cmap="viridis") -> Axes|Figure:
    """
    Creates colormap of "u" using "f" for the given "times".
    """

    if (ax is None) or (fig is None):

        fig, ax = plt.subplots()

    t = data["t"]
    u = data["u"]

    index_t0 = np.argmin(np.abs(t-t0))

    norm = cm.colors.Normalize(vmin=0.0, vmax=np.max(f(u[index_t0])))

    img = ax.imshow(f(u[index_t0]), extent=[0,1,0,1], cmap=plt.get_cmap(cmap), norm=norm)


    fontsize = 12
    ax.set_xlabel("x", fontsize=fontsize)
    ax.set_ylabel("y", fontsize=fontsize)
    ax.tick_params(labelsize=fontsize)

    # Add a colourbar
    cbar = fig.colorbar(img, ax=ax)
    if not (z_label is None):
        cbar.set_label(z_label, fontsize=fontsize)
    cbar.ax.tick_params(labelsize=fontsize)


    if not(title is None):
        plt.title(title)

    return ax, fig

latex_fonts_called = False 

def latex_fonts():
    """
    To create nice figures in prl-style.
    """

    # plt.style.use(['science', 'ieee'])
    # plt.style.use(['science', 'nature'])
    plt.style.use(['science'])

    # Correct fonts:
    rc('font', **{'family': 'serif', 'serif': ['Times']})
    rc('text', usetex=True)
    plt.rcParams["text.latex.preamble"] = r"\usepackage{siunitx}" # For si units