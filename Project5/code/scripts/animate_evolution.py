from utils import *



data_noSlit = read_hd5_data("out/default_noSlit_data.h5")
data_singleSlit = read_hd5_data("out/single_Slit_problem9_data.h5")
data_doubleSlit = read_hd5_data("out/default_Slit_data.h5")
data_tripleSlit = read_hd5_data("out/triple_Slit_problem9_data.h5")

datas = [data_noSlit, data_singleSlit, data_doubleSlit, data_tripleSlit]
out_dir = "animations/"
names = ["no_slit", "single_slit", "double_slit", "triple_slit"]
for data, name in zip(datas, names):
    u = data["u"]


    # Create figure
    fig = plt.figure(figsize=(9,9))
    ax = plt.gca()
    anim = generate_animation(ax, fig, data)
    anim.save(out_dir + name + ".mp4", writer="ffmpeg", bitrate=10000, fps=15)  # The fps (frames per second) sets the animation speed
    plt.show()
