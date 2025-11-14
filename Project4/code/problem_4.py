from problem_1 import states
from tabulate import tabulate
import matplotlib.pyplot as plt
import numpy as np
plt.style.use('ggplot')

N = 4
J   = 1.0
k_B = 1.0
T   = J / k_B

# --- Compute predicted values ---
data_predicted = np.zeros(6)  
Z = 0

for state in states:
    M   = state["M"]
    E   = state["E"]
    deg = state["deg"]
    weight = deg * np.exp(-E*J/(k_B*T))

    data_predicted[0] += weight * E / N
    data_predicted[1] += weight * E**2 / N**2
    data_predicted[2] += weight * np.abs(M) / N
    data_predicted[3] += weight * M**2 / N**2

    Z += weight

data_predicted /= Z

# Compute heat capacity and susceptibility
eps_var = data_predicted[1] - data_predicted[0]**2
m_var   = data_predicted[3] - data_predicted[2]**2

C = eps_var * N / (k_B * T**2)
X = m_var   * N / (k_B * T)


# --- Load numerical data ---
data = np.loadtxt("out/MCMC-unordered-L2-T1p0/raw_data.csv", delimiter=",")[1:]

cycles  = data[:,0]
eps_num = data[:,1] / N
m_num   = data[:,2] / N

C_num = np.var(eps_num) * N / (k_B * T**2)
X_num = np.var(np.abs(m_num)) * N / (k_B * T)

# --- Problem 4b: compare analytical and numerical ---
header = ["Analytical", "Numerical"]
col_1 = ["Mean(ε)", "Mean(|m|)", "C_V/N", "X/N"]
col_2 = [data_predicted[0], data_predicted[2], C, X]
col_3 = [np.mean(eps_num), np.mean(np.abs(m_num)), C_num, X_num]

table = tabulate(zip(col_1, col_2, col_3), headers=header, tablefmt="rounded_grid")
print("\n--- Problem 4b ---\n")
print(table)


# --- Problem 4c: plot equilibration of <ε> ---
cycles  = data[:,0]
eps_num = data[:,1] / N
m_num   = data[:,2] / N

eps_avg_num = np.cumsum(eps_num) / (np.arange(len(eps_num)) + 1)

plt.figure(figsize=(6,4), constrained_layout=True)
plt.semilogx(cycles, eps_avg_num, label="Numerical <ε>")
plt.axhline(data_predicted[0], color='C1', linestyle='--', label="Predicted <ε>")
plt.xlim([1, cycles[-1]])
plt.xlabel("No. of MC cycles")
plt.ylabel("Energy per spin")
plt.legend()
plt.savefig("out/problem_4.pdf")
