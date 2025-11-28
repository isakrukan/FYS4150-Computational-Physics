import numpy as np
import matplotlib.pyplot as plt
import h5py




dataset = h5py.File('time_evolution.h5', 'r')

print(dataset)
print(dataset.keys())

for key in dataset.keys():
    print(f"{key}: {dataset[key].shape}")

print(np.sum(np.abs(dataset['real'])))

# ^ all zeros

