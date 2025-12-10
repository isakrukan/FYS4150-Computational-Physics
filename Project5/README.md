# Project 5
![hippo](https://github.uio.no/icrukan/FYS4150/blob/master/Project5/animations/double_slit.gif)

## TODO
- Finish report
- Finish this README (e.g. *Python scripts* section at the botto,)

This repository contains the code and documentation for Project 4 in the course *Computational Physics (FYS4150)* at the University of Oslo.

## About the project
This project uses the Crank-Nicolson scheme to solve the Schrödinger equation for a particle passing through a barrier with single-, double- and triple-slits.
Results include predictions for the resulting interference patterns.

## Structure
- **`code/`** All code used to generate results for this project.
- - **`code/include`** Header files for the project.
- - **`code/src`** All source code files for the project.
- - **`code/scripts`** Python scripts for plotting & analyzing results.
- - **`code/build`** Build files.
- - **`code/tests`** Tests for our code.
- **`figs/`** All figures used in main.tex
- **`docs/`** Documentation of the code struture generated using doxygen [doxygen](https://www.doxygen.nl/)


# Code Implementation
To build and run the code, start by navigating to the **`code/`** directory. 

## Requirements
### Main program
To compile and run the main program, the most basic dependencies are:
- Armadillo (C++ linear algebra library)
- HDF5 (Used to store data)

Additionally, compiling using the `makefile` requires 
- g++
- CMake


On Debian-based systems, for instance, these can be installed by running the following in your terminal: 
```bash
sudo apt-get install g++ cmake libarmadillo-dev libhdf5-dev
```

### Plotting scrips
For the python scripts, the basic dependencies are:

- Python 3.x
- NumPy
- Matplotlib
- h5py
- SciencePlots

## Usage
### Compiling and running entire project using make
The entire project may be compiled and run using
```bash
make project 
```

### Running the main program manually
1. Compile using:
    ```bash
    make all
    ```
2. The main program may then be run using the following optional arguments: 
    ```
    Usage:
        ./build/main [options]

    Options:
          [ Wall/slit parameters ]
          --single_slit,                  Include single slit (true/false)
          --double_slit,                  Include double slit (true/false)
          --triple_slit,                  Include triple slit (true/false)

          --wc,                           Centre position of wall in x-direction
          --wt,                           Wall thickness in x-direction
          --a,                            Distance between slits
          --d,                            Height of slits in y-direction
          --v0,                           Potential height of slit wall

          [ Wavepacket parameters ]
          --sigma_x,     -sx              Standard deviation in x
          --sigma_y,     -sy              Standard deviation in y
          --center_x,    -cx              Center position in x
          --center_y,    -cy              Center position in y
          --momentum_x,  -px              Momentum in x
          --momentum_y,  -py              Momentum in y

          [ Simulation parameters ]
          --dt,                           Time step size
          --L,                            Length of the domain
          --h,                            Height of the domain
          --T,                            Total simulation time
          --identifier,                   Unique identifier for the simulation

          --help                 Show this help message;
    ```



### Python scripts

TODO

## Authors 
- Magnus Axelsen
- Isak Cecil Onsager Rukan