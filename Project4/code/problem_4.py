from problem_1 import states
from tabulate import tabulate
import numpy as np

N = 4
J   = 1.0
k_B = 1.0
T   = J / k_B

data_predicted = np.zeros(5)  
Z = 0

for state in states:
    M   = state["M"]
    E   = state["E"]
    deg = state["deg"]
    weight = deg * np.exp(-E*J/(k_B*T))

    data_predicted[0] = weight * E 
    data_predicted[1] = weight * E**2
    data_predicted[2] = weight * M
    data_predicted[3] = weight * M**2
    data_predicted[4] = weight * np.abs(M)

    Z += weight

data_predicted /= Z

eps_avg     = data_predicted[0] / N
eps_sqr_avg = data_predicted[1] / N**2
eps_var     = eps_sqr_avg - eps_avg**2

m_avg     = data_predicted[2] / N
m_avg_abs = data_predicted[4] / N
m_sqr_avg = data_predicted[3] / N**2
m_var     = m_sqr_avg - m_avg**2

C_V = (eps_sqr_avg - eps_avg**2) * (J**2) / (k_B * T**2)
chi = (m_sqr_avg - m_avg**2) * (J**2) / (k_B * T)

header = ["Observable", "Value"]
col_1 = ["Mean(ε)", "Var(ε)", "Mean(m)", "Mean(|m|)", "Var(m)", "C", "chi"]
col_2 = data_predicted.tolist() + [C_V, chi]

table = tabulate(zip(col_1, col_2), headers=header, tablefmt="rounded_grid")
print("Problem 1b:\n")
print(table)


    
