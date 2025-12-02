from utils import * 


data = read_hd5_data("out/double_Slit_problem8_data.h5")
t = data["t"]
u = data["u"]
M = data["M"]

t0 = 0.002; x0 = 0.8
x = np.linspace(0,1,M)
y = np.linspace(0,1,M)

index_t = np.argmin(np.abs(t - t0))
index_x = np.argmin(np.abs(x - x0))


latex_fonts()
# Normalizing to get p(y|x=x0; t=t0):
p = np.abs(u[index_t][index_x,:])**2
p /= np.sum(p)

plt.plot(y, p)
plt.xlabel("y")
plt.ylabel(r"$|u(x=0.8, y, t)|^2$")
plt.legend()
plt.show()
