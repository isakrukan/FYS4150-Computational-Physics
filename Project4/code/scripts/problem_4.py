from problem_1 import states
from tabulate import tabulate
import matplotlib.pyplot as plt
import numpy as np
import utils

utils.latex_fonts()


DIR = "../figs/"

N = 4
J   = 1.0
k_B = 1.0
T   = J / k_B

## Analytic predictions:
def E_mean_analytic(T, k_B, E0):
    x = E0 / (T*k_B)
    return -E0 * np.sinh(x)/(np.cosh(x) + 3)

def E2_mean_analytic(T, k_B, E0):
    x = E0 / (T*k_B)
    return E0**2 * np.cosh(x)/(np.cosh(x) + 3)

def M_abs_mean_analytic(T, k_B, E0):
    x = E0 / (T*k_B)
    return 2*(np.exp(x) + 2) / (np.cosh(x) + 3)

def M2_mean_analytic(T, k_B, E0):
    x = E0 / (T*k_B)
    return 8*(np.exp(x) + 1) / (np.cosh(x) + 3)

def C_V_analytic(T, k_B, E0):
    x = E0 / (T*k_B)
    return E0**2/(k_B*T**2) * (3*np.cosh(x) + 1) / (np.cosh(x) + 3)**2

def chi_analytic(T, k_B, E0):
    x = E0 / (T*k_B)
    return 8/(k_B*T) * (np.exp(x) + 1) / (np.cosh(x) + 3)

E0 = 8*J
E_mean = E_mean_analytic(T, k_B, E0)
E2_mean = E2_mean_analytic(T, k_B, E0)
M_abs = M_abs_mean_analytic(T, k_B, E0)
M2 =  M2_mean_analytic(T, k_B, E0)

eps = E_mean / N 
eps2 = E2_mean / N**2 
m_abs = M_abs / N 
m2 = M2 / N**2 

C_V = C_V_analytic(T, k_B, E0)
chi = chi_analytic(T, k_B, E0)

# --- Load numerical data ---
data = np.loadtxt("out/MCMC-unordered-L2-T1p0/raw_data.csv", delimiter=",")[1:]

cycles  = data[:,0]
eps_num = data[:,1] / N
m_num   = data[:,2] / N

C_num   = np.var(eps_num) * N / (k_B * T**2)
chi_num = np.var(m_num) * N / (k_B * T)


E_mean_num      = np.cumsum(data[:,1]) / cycles
M_mean_num      = np.cumsum(np.abs(data[:,2])) / cycles

eps_mean_num    = np.cumsum(eps_num) / cycles
m_mean_num      = np.cumsum(m_num) / cycles
m_abs_mean_num  = np.cumsum(np.abs(m_num)) / cycles

eps2_mean_num   = np.cumsum(eps_num**2) / cycles
m2_mean_num     = np.cumsum(m_num**2) / cycles

C_cumulated     = (eps2_mean_num - eps_mean_num**2) * N / (k_B * T**2)
chi_cumulated   = (m2_mean_num - m_mean_num**2) * N / (k_B * T)

# --- Problem 4b: compare analytical and numerical ---

header = ["Analytical", "Numerical"]
headers = ["Mean(eps)", "Mean(|m|)", "C_V/N", "X/N"]
# predicted = [data_predicted[0], data_predicted[2], C, X] 
predicted = [eps, m_abs, C_V/N, chi/N] 
estimated = [np.mean(eps), np.mean(np.abs(m_num)), C_num, chi_num] 

table = tabulate(zip(headers, predicted, estimated), headers=header, tablefmt="rounded_grid")
print("\n--- Problem 4b ---\n")
print(table)

# Computing relative errors:
predicted = [E_mean, M_abs, C_V/N, chi/N] 
estimated = [E_mean_num, M_mean_num, C_cumulated, chi_cumulated] 
relative_errors = [np.abs((pred - est)/pred) for pred, est in zip(predicted, estimated)]

# Plot of relative errors:
plt.figure(figsize=(6,4), constrained_layout=True)
labels = [r"$\Delta \langle E\rangle$", r"$\Delta \langle |M|\rangle$", r"$\Delta C_V/N$", r"$\Delta \chi/N$"]
for i in range(len(relative_errors)):
    plt.plot(cycles, relative_errors[i], label=labels[i])

plt.yscale("log")
plt.xscale("log")
plt.xlim([1, cycles[-1]])
# plt.ylim([0,1.5])
plt.xlabel("No. of MC cycles")
plt.ylabel("Relative errors")
plt.legend()
plt.savefig(DIR + "errors_2x2.pdf")

# --- Problem 4c: plot equilibration of <eps> ---
cycles  = data[:,0]
eps_num = data[:,1] / N
m_num   = data[:,2] / N

eps_avg_num = np.cumsum(eps_num) / (np.arange(len(eps_num)) + 1)

plt.figure(figsize=(6,4), constrained_layout=True)
plt.semilogx(cycles, eps_avg_num, label=r"Numerical $<\epsilon>$")
plt.axhline(eps, color='C1', linestyle='--', label=r"Predicted $<\epsilon>$")
plt.xlim([1, cycles[-1]])
plt.xlabel("No. of MC cycles")
plt.ylabel("Energy per spin")
plt.legend()
plt.savefig(DIR + "ncycles_2x2.pdf")
# plt.show()