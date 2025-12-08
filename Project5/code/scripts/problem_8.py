from utils import * 


data = read_hd5_data("out/double_Slit_problem8_data.h5")
t = data["t"]
u = data["u"]

OUT_DIR = "../results/problem_8/"

times = [0, 0.001, 0.002]
xy_lims = [(0,1), (0.35, 0.65), (0.2, 0.8)]
xy_lims = [(0.1, 0.9)]*3
latex_fonts()
for t0, xy_lim in zip(times, xy_lims):

    # Absolute value
    ax_abs, fig_abs = plot_colormap(data, t0, 
                            lambda x: np.abs(x)**2, 
                            z_label=r"$|u(x,y,t)|^2$", 
                            # title=f"t={t0*1e3:.1f} " + r"$\si{ms}$"
                            )

    ax_abs.grid(visible=True, which='major', color='gray', linestyle='--', linewidth=0.5, alpha=0.5)
    ax_abs.set_xlim(xy_lim)
    ax_abs.set_ylim(xy_lim)
    # fig_abs.tight_layout(pad=0.5)
    fig_abs.savefig(OUT_DIR + f"absolute_value{t0}.pdf")
    figsize = tuple(fig_abs.get_size_inches())

    # Real value
    fig_real, ax_real = plt.subplots(figsize=figsize)
    ax, fig = plot_colormap(data, t0, 
                            lambda x: np.real(x), 
                            z_label=r"Re$(u(x,y,t))$", 
                            # title=f"t={t0*1e3:.1f} " + r"$\si{ms}$"
                            cmap="seismic",
                            ax=ax_real, fig=fig_real
                            )

    ax_real.grid(visible=True, which='major', color='gray', linestyle='--', linewidth=0.5, alpha=0.5)
    ax_real.set_xlim(xy_lim)
    ax_real.set_ylim(xy_lim)
    # fig_real.tight_layout(pad=0.5)
    fig_real.savefig(OUT_DIR + f"real_value{t0}.pdf")

    # Imaginary value
    fig_imag, ax_imag = plt.subplots(figsize=figsize)
    ax, fig = plot_colormap(data, t0, 
                            lambda x: np.imag(x),
                            z_label=r"Im$(u(x,y,t))$", 
                            # title=f"t={t0*1e3:.1f} " + r"$\si{ms}$"
                            cmap="seismic",
                            ax=ax_imag, fig=fig_imag
                            )

    ax_imag.grid(visible=True, which='major', color='gray', linestyle='--', linewidth=0.5, alpha=0.5)
    ax_imag.set_xlim(xy_lim)
    ax_imag.set_ylim(xy_lim)
    # fig_imag.tight_layout(pad=0.5)
    fig_imag.savefig(OUT_DIR + f"imag_value{t0}.pdf")

    # plt.show()
    # exit()

 # Absolute value ignoring a portion

x = np.linspace(0,1,data["M"])
away_from_the_barrier = x <= 0.5
data["u"][:,:,away_from_the_barrier] = 0
t0 = 0.002
ax, fig = plot_colormap(data, t0, 
                        lambda x: np.abs(x)**2, 
                        z_label=r"$|u(x,y,t)|^2$", 
                        # title=f"t={t0*1e3:.1f} " + r"$\si{ms}$"
                        )

ax.grid(visible=True, which='major', color='gray', linestyle='--', linewidth=0.5, alpha=0.5)
plt.tight_layout(pad=0.5)
plt.xlim(xy_lims[0])
plt.ylim(xy_lims[0])
plt.savefig(OUT_DIR + f"absolute_value{t0}_ignored.pdf")
# plt.show()