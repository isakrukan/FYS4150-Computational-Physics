import matplotlib.pyplot as plt
import numpy as np
from utils import *
from matplotlib.animation import FuncAnimation
import matplotlib.cm as cm
from matplotlib.axes import Axes
from matplotlib.figure import Figure

def generate_animation(ax:Axes, fig:Figure, data:dict):
    h = data["h"]; dt = data["dt"]
    
    # Set up a 2D xy grid
    h = 0.005
    x_points = np.arange(0, 1+h, h)
    y_points = np.arange(0, 1+h, h)
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
    time_txt = plt.text(0.95, 0.95, "t = {:.3e}".format(t_min), color="white", 
                        horizontalalignment="right", verticalalignment="top", fontsize=fontsize)

    # Function that takes care of updating the z data and other things for each frame
    def animation(i):
        # Normalize the colour scale to the current frame?
        norm = cm.colors.Normalize(vmin=0.0, vmax=np.max(u_squared[i]))
        img.set_norm(norm)

        # Update z data
        img.set_data(u_squared[i])

        # Update the time label
        current_time = t_min + i * dt
        time_txt.set_text("t = {:.3e}".format(current_time))

        return img

    # Use matplotlib.animation.FuncAnimation to put it all together
    ani = FuncAnimation(fig, animation, interval=1, frames=np.arange(0, len(u), 2), repeat=False, blit=0)
    plt.show()
    return ani

data = read_hd5_data("out/data.h5")

u = data["u"]
print(np.linalg.norm(u[1]))

# Create figure
fig = plt.figure(figsize=(9,9))
ax = plt.gca()
generate_animation(ax, fig, data)
# Run the animation!
plt.show()

# # Save the animation
# anim.save('./animation.mp4', writer="ffmpeg", bitrate=10000, fps=15)  # The fps (frames per second) sets the animation speed
