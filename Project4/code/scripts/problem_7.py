import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
plt.style.use('ggplot')

def amdahl_model(N, f):
    return (1-f) / N + f

data = np.loadtxt("out/times_openmp.txt")

threads = np.arange(1, data.shape[0] + 1)
data_avg = np.mean(data, axis=1)
data_var = np.var(data, axis=1)
data_err = np.sqrt(data_var / data.shape[1])

time_serial = data_avg[0]

data_avg /= time_serial
data_var /= time_serial**2
data_err /= time_serial


popt, pcov = curve_fit(amdahl_model, threads, data_avg)
f = popt
print("\n--- Problem 7 ---")
print("Fitted function for times:")
print(f" T(N) = (1-f)/N + f")
print(f" f={f[0]:.4f} ± {np.sqrt(pcov[0,0]):.4f}\n")
print(f" Maximum speedup:")
print(f" S_max = 1/f = {1/f[0]:.2f}\n")


plt.figure(figsize=(6,4), tight_layout=True)
plt.plot(threads, data_avg, marker='o', linestyle='--', label="Measured data")
plt.fill_between(threads, data_avg - data_err, data_avg + data_err, alpha=0.4)
plt.plot(threads, amdahl_model(threads, *popt), color='k', alpha=0.5, label=f"$T(N) = (1-f)/N + f$")

plt.ylim(0,1.05)
plt.xlabel("No. of threads")
plt.ylabel(r"Time  [$T_\mathrm{serial}$]")
plt.title("Timing tests for OpenMP implementation")
plt.legend()
plt.savefig("out/timing_tests.pdf")