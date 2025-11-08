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
    
    # a) 
    headers = ["n_up", "M", "E", "Degeneracy"]
    contents = [(state["n_up"], state["M"], state["E"], state["deg"]) for state in states]

    table = tabulate(contents, headers=headers, tablefmt="rounded_grid")
    print("Problem 1:\n")
    print(table)