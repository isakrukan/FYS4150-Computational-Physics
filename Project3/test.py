import numpy as np

def n_factorial(n):
    """Input = int(n), returns n factorial"""
    total = 1
    for i in range(1, n+1):
        total *= i
    return total

def eksponentialfunktion(n, x):
    """
    Estimates the exponentialfunction
    Input = int(n) and int(x) 
    n represents how many terms in the 
    series expansion we sum and x is the variable.

    """
    resultat = 0
    for k in range(0, n+1):
        resultat += x**n/n_factorial(k)
    return resultat

n_input = x_input = [0, 1, 2]

for n in n_input:
    print(n)
    for x in x_input:
        print(f"n = {n}, x = {x}  sum = {eksponentialfunktion(n, x)}")


print(eksponentialfunktion(15, 1))

result = eksponentialfunktion(0, 1)
print(n)
while abs(result-np.exp(1))>1e-5: # algorithm for finding n such as 
    result = eksponentialfunktion(n, 1)
    n = n+1
    Error = abs(result-np.exp(1))
    print(Error, n)
