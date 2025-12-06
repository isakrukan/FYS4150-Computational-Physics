from utils import * 


data_single = read_hd5_data("out/single_Slit_problem9_data.h5")
data_double = read_hd5_data("out/double_Slit_problem8_data.h5")
data_triple = read_hd5_data("out/triple_Slit_problem9_data.h5")
data_names = ["single", "double", "triple"]

latex_fonts()

datas = [data_single, data_double, data_triple]
for data, data_name in zip(datas, data_names):
    t = data["t"]
    u = data["u"]
    M = data["M"]

    t0 = 0.002; x0 = 0.8
    x = np.linspace(0,1,M)
    y = np.linspace(0,1,M)

    index_t = np.argmin(np.abs(t - t0))
    index_x = np.argmin(np.abs(x - x0))


    # Normalizing to get p(y|x=x0; t=t0):
    p = np.abs(u[index_t][:,index_x])**2
    p /= np.sum(p)

    plt.plot(y, p)
    plt.xlabel("y")
    plt.ylabel(r"$|u(x=0.8, y, t)|^2$")
    plt.legend()
    plt.savefig(f"../figs/problem_9/probability_x0p8_{data_name}.pdf")
    plt.show()



