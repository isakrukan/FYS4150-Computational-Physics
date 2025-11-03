from problem_1 import states
import sympy as sp

T = sp.symbols('T', positive=True, real=True)
J = sp.symbols('J', real=True)
k = sp.symbols('k', positive=True, real=True)

Z = 0
M_avg  = 0
M_avg_abs  = 0
M2_avg = 0
E_avg  = 0
E2_avg = 0

for state in states:
    M    = state["M"]
    E    = state["E"]
    deg  = state["deg"]
    n_up = state["n_up"]
    weight = deg * sp.exp(-E*J/(k*T))
    

    Z         += weight
    M_avg     += weight * M
    M_avg_abs += weight * sp.Abs(M)
    M2_avg    += weight * M**2
    E_avg     += weight * E
    E2_avg    += weight * E**2

M_avg  = M_avg / Z
M_avg_abs  = M_avg_abs / Z
M2_avg = M2_avg / Z
E_avg  = E_avg / Z
E2_avg = E2_avg / Z
    
N = 4
    
def print_numeric(T_val, J_val, k_val):

    Z_val = Z.evalf(subs={T:T_val, J:J_val, k:k_val})

    m_avg  = M_avg.evalf(subs={T:T_val, J:J_val, k:k_val}) / N
    m_avg_abs  = M_avg_abs.evalf(subs={T:T_val, J:J_val, k:k_val}) / N
    m2_avg = M2_avg.evalf(subs={T:T_val, J:J_val, k:k_val}) / N**2
    eps_avg  = E_avg.evalf(subs={T:T_val, J:J_val, k:k_val}) / N
    eps2_avg = E2_avg.evalf(subs={T:T_val, J:J_val, k:k_val}) / N**2

    m_var = m2_avg - m_avg**2
    eps_var = eps2_avg - eps_avg**2

    C   = N * eps_var / (k_val * T_val**2)
    chi = N * m_var   / (k_val * T_val)

    print(f"\nFor T={T_val}, J={J_val}, k={k_val}:")
    #print(f"Z = {Z_val}")
    print(f"<ε> = {eps_avg}")
    print(f"<ε>_var = {eps_var}")
    print(f"<m> = {m_avg}")
    print(f"<|m|> = {m_avg_abs}")
    print(f"<m>_var = {m_var}")
    print(f"C = {C}")
    print(f"chi = {chi}")
    

k_val = 1.0
J_val = 1.0
T_val = J_val / k_val

    
print_numeric(T_val, J_val, k_val)
