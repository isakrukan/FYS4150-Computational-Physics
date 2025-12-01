import h5py
import numpy as np

def read_hd5_data(filename_and_path:str) -> np.ndarray|np.ndarray:
    """
    Reads the data stored at "filename_and_path" and returns t and u.
    """
    tmp = {}
    with h5py.File(filename_and_path, "r") as f:

        # Parameters:
        tmp["M"] = int(f["M"][0][0])
        tmp["N"] = int(f["N"][0][0])
        tmp["h"] = float(f["h"][0][0])
        tmp["dt"] = float(f["dt"][0][0])
        tmp["v0"] = float(f["v0"][0][0])

        # Arrays:
        t = np.array(f["t"][0])
        u = np.array(f["u"][...])

        # Converting from (x,y) to x+iy:
        real_id, imag_id = u.dtype.names
        u = u[real_id] + 1j * u[imag_id] # shape (N, grid_points)

        u = u.reshape(-1, tmp["M"] , tmp["M"] )    # shape (N, M, M)

        # Storing:
        tmp["t"] = t
        tmp["u"] = u      

        

    return tmp
