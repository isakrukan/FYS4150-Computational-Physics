#-------------
# Problem 1
#-------------
    
states = [{"n_up": 0, "M" : -4, "E" : -8, "deg" : 1},
          {"n_up": 1, "M" : -2, "E" :  0, "deg" : 4},
          {"n_up": 2, "M" :  0, "E" :  0, "deg" : 4},
          {"n_up": 2, "M" :  0, "E" :  8, "deg" : 2},
          {"n_up": 3, "M" :  2, "E" :  0, "deg" : 4},
          {"n_up": 4, "M" :  4, "E" : -8, "deg" : 1}]

if __name__ == "__main__":
    from tabulate import tabulate
    import sympy as sp
    import matplotlib.pyplot as plt
    
    # a) 
    headers = ["n_up", "M", "E", "Degeneracy"]
    contents = [(state["n_up"], state["M"], state["E"], state["deg"]) for state in states]

    table = tabulate(contents, headers=headers, tablefmt="rounded_grid")
    print("Problem 1:\n")
    print(table)

    # b) 

    T = sp.symbols('T', positive=True, real=True)
    J = sp.symbols('J', real=True)
    k = sp.symbols('k_B', positive=True, real=True)


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

    C_V = (E2_avg - E_avg**2) / (k * T**2)
    chi = (M2_avg - M_avg**2) / (k * T)


    # simplify and format expressions for nicer printing
    Z   = r"Z & = " +  f"{sp.latex(sp.simplify(Z))}" + r"\\"
    M_avg = r"\langle M \rangle & = " +  f"{sp.latex(sp.simplify(M_avg))}" + r"\\"
    M_avg_abs = r"\langle |M| \rangle & = " +  f"{sp.latex(sp.simplify(M_avg_abs))}" + r"\\"
    M2_avg = r"\langle M^2 \rangle & = " +  f"{sp.latex(sp.simplify(M2_avg))}" + r"\\"
    E_avg = r"\langle E \rangle & = " +  f"{sp.latex(sp.simplify(E_avg))}" + r"\\"
    E2_avg = r"\langle E^2 \rangle & = " +  f"{sp.latex(sp.simplify(E2_avg))}" + r"\\"

    C_V = r"C_V & = " +  f"{sp.latex(sp.simplify(C_V))}" + r"\\"
    chi = r"\chi & = " +  f"{sp.latex(sp.simplify(chi))}" + r"\\"

    print("\nProblem 1 (b):")
    print(Z)
    print(E_avg)
    print(E2_avg)
    print(M_avg_abs)
    print(M2_avg)
    print(C_V)
    print(chi)
    # exit()
    
    print("\nSymbolic expressions for averages:")
    print(f"<M> = {M_avg}")
    print(f"<|M|> = {M_avg_abs}")
    print(f"<M^2> = {M2_avg}")
    print(f"<E> = {E_avg}")
    print(f"<E^2> = {E2_avg}")
    print(f"Z = {Z}")