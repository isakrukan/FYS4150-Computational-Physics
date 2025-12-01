import h5py
import numpy as np

def read_hd5_data(filename_and_path:str) -> dict:
    """
    Reads the data stored at "filename_and_path" and returns dictionary containing M, N, h, dt, v0, t and u.
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
        t = np.array(f["t"][:]).reshape(-1)   # full 1D array of length N

        u = np.array(f["u"][:])

        u_raw = np.array(f["u"][...])      # shape (M*M, N)

        # Converting from (x,y) to x + iy:
        real_id, imag_id = u_raw.dtype.names
        u_complex = u_raw[real_id] + 1j * u_raw[imag_id]  

        u_complex = u_complex.T                            # (N, M*M)
        u = u_complex.reshape(-1, tmp["M"], tmp["M"])      # (N, M, M)
        
        # Storing:
        tmp["t"] = t
        tmp["u"] = u      

        

    return tmp