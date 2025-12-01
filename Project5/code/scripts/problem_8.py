from utils import * 


data = read_hd5_data("out/double_Slit_problem8_data.h5")
t = data["t"]
u = data["u"]


times = [0, 0.001, 0.002]

for t0 in times:

    ax, fig = plot_colormap(data, t0, lambda x: np.abs(x)**2, z_label=r"$|u(x,y,t)|^2$")

    plt.show()

    ax, fig = plot_colormap(data, t0, lambda x: np.real(x), z_label=r"Re$(u)$")

    plt.show()

    ax, fig = plot_colormap(data, t0, lambda x: np.real(x), z_label=r"Im$(u)$")

    plt.show()