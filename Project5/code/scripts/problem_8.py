from utils import * 


data = read_hd5_data("out/double_Slit_problem8_data.h5")
t = data["t"]
u = data["u"]

OUT_DIR = "../results/problem_8/"

times = [0, 0.001, 0.002]

latex_fonts()
for t0 in times:

    # Absolute value
    ax, fig = plot_colormap(data, t0, 
                            lambda x: np.abs(x)**2, 
                            z_label=r"$|u(x,y,t)|^2$", 
                            # title=f"t={t0*1e3:.1f} " + r"$\si{ms}$"
                            )

    ax.grid(visible=True, which='major', color='gray', linestyle='--', linewidth=0.5, alpha=0.5)
    plt.tight_layout(pad=0.5)
    plt.savefig(OUT_DIR + f"absolute_value{t0}.pdf")

    # Real value
    ax, fig = plot_colormap(data, t0, 
                            lambda x: np.real(x), 
                            z_label=r"Re$\{u(x,y,t)\}$", 
                            # title=f"t={t0*1e3:.1f} " + r"$\si{ms}$"
                            )

    ax.grid(visible=True, which='major', color='gray', linestyle='--', linewidth=0.5, alpha=0.5)
    plt.tight_layout(pad=0.5)
    plt.savefig(OUT_DIR + f"real_value{t0}.pdf")

    # Imaginary value
    ax, fig = plot_colormap(data, t0, 
                            lambda x: np.imag(x),
                            z_label=r"Im$\{u(x,y,t)\}$", 
                            # title=f"t={t0*1e3:.1f} " + r"$\si{ms}$"
                            )

    ax.grid(visible=True, which='major', color='gray', linestyle='--', linewidth=0.5, alpha=0.5)
    plt.tight_layout(pad=0.5)
    plt.savefig(OUT_DIR + f"imag_value{t0}.pdf")

plt.show()
