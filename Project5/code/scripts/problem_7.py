from utils import * 



data_noSlit = read_hd5_data("out/default_noSlitdata.h5")
data_Slit = read_hd5_data("out/default_Slitdata.h5")

u_noSlit = data_noSlit["u"]
u_Slit = data_Slit["u"]

t = data_noSlit["t"] # Should be the same size as data_Slit["t"]

p_noSlit = [np.abs(1 - np.linalg.norm(un)) for un in u_noSlit]
p_Slit = [np.abs(1 - np.linalg.norm(un)) for un in u_Slit]

plt.plot(t, p_noSlit, label="No slit")
plt.plot(t, p_noSlit, label="Double slit")
plt.yscale("log")
plt.legend()
plt.show()