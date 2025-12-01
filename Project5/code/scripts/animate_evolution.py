from utils import *



data = read_hd5_data("out/default_noSlit_data.h5")
data = read_hd5_data("out/default_Slit_data.h5")
u = data["u"]


# Create figure
fig = plt.figure(figsize=(9,9))
ax = plt.gca()
generate_animation(ax, fig, data)
# Run the animation!
plt.show()

# # Save the animation
# anim.save('./animation.mp4', writer="ffmpeg", bitrate=10000, fps=15)  # The fps (frames per second) sets the animation speed
