import h5py
import numpy as np

def read_hd5_data(filename_and_path:str) -> np.ndarray|np.ndarray:
    """
    Reads the data stored at "filename_and_path" and returns t and u.
    """
    with h5py.File(filename_and_path, "r") as f:
        
        for keys in f:
            print(keys)
        u0 = f["u_0"]
        print(u0)
        t = np.array(f["t"])

        # Automatically find all u_i datasets
        u = []
        for key in f.keys():
            if key.startswith("u_"):
                u.append(np.array(f[key]))

    
    u = np.array(u)

    return t,u
