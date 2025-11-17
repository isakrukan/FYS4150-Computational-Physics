import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
plt.style.use('ggplot')

def inv_model(N, a, b):
    return a / N + b

data = np.loadtxt("out/times_openmp.txt")

threads = np.arange(1, data.shape[0] + 1)
data_avg = np.mean(data, axis=1)
data_var = np.var(data, axis=1)
data_err = np.sqrt(data_var / data.shape[1])

time_serial = data_avg[0]

data_avg /= time_serial
data_var /= time_serial**2
data_err /= time_serial


popt, pcov = curve_fit(inv_model, threads, data_avg)
a, b = popt
print("\n--- Problem 7 ---")
print("Fitted function for times:")
print(f" T(N) = a/N + b")
print(f" a={a} ± {np.sqrt(pcov[0,0])}")
print(f" b={b} ± {np.sqrt(pcov[1,1])}")

plt.figure(figsize=(6,4), tight_layout=True)
plt.plot(threads, data_avg, marker='o', linestyle='--', label="Measured data")
plt.fill_between(threads, data_avg - data_err, data_avg + data_err, alpha=0.4)
plt.plot(threads, inv_model(threads, *popt), color='k', alpha=0.5, label=f"$T(N) = a/N + b$")

plt.xlabel("No. of threads")
plt.ylabel(r"Time  [$T_\mathrm{serial}$]")
plt.title("Timing tests for OpenMP implementation")
plt.legend()
plt.savefig("out/timing_tests.pdf")