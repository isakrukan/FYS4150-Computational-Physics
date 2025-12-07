from utils import * 



data_noSlit = read_hd5_data("out/default_noSlit_data.h5")
data_Slit = read_hd5_data("out/default_Slit_data.h5")

u_noSlit = data_noSlit["u"]
u_Slit = data_Slit["u"]

t = data_noSlit["t"] # Should be the same size as data_Slit["t"]

p_noSlit = [np.abs(1 - np.linalg.norm(un)) for un in u_noSlit]
p_Slit = [np.abs(1 - np.linalg.norm(un)) for un in u_Slit]

latex_fonts()
t *= 1e3  # s -> ms

fig, ax = plt.subplots()

ax.plot(t, p_noSlit, label="No slit")
ax.plot(t, p_Slit, label="Double slit")

ax.set_xlabel(r"time $[\si{ms}]$")
ax.set_ylabel(r"deviation from total probability")  # or r"$1-p(t)$"
ax.set_yscale("log")
ax.legend()

plt.tight_layout(pad=0.5)
plt.savefig("../results/problem_7/problem_7.pdf")
plt.show()
