# Project 4: Computational Physics
This repository contains the code and documentation for Project 4 in the FYS3150 - Computational Physics course at the University of Oslo.

# TO DO
- Finish this README
- Finish report

## About the project
TODO


## Folders
- **`code/`** Contains all code used to generate results for this project.
- **`figs/`** Contains all figures used in main.tex
- **`docs/`** Contains documentation of the code struture generated using doxygen [doxygen](https://www.doxygen.nl/)


# Code Implementation
To build and run the code, enter the **`code/`** directory, structured as follows
- **`src/`**: Contains all source code files for the project.
- **`include/`** Contains relevant header files for the project.
- **`out/`**: Save location for data from main program.
- **`scripts/`**: Contains Python scripts for plotting & analyzing results 

## Requirements
### Main program
To compile and run the main program, the most basic dependencies are:
- Armadillo (C++ linear algebra library)
- OpenMP

Additionally, compiling using the `makefile` requires 
- g++
- CMake


On Debian-based systems, for instance, these can be installed by running the following in your terminal: 
```bash
sudo apt-get install g++ cmake libarmadillo-dev libomp-dev
```

### Plotting scrips
For the python scripts, the basic dependencies are:

- Python 3.x
- NumPy
- Matplotlib
- SciPy
- tabulate

These can be installed by running
```bash
pip install -r requirements.txt
```
inside the **`code/`** directory.

## Usage
### Compiling and running entire project using make
The entire project may be compiled and run using
```bash
make project 
```
Note that this will only run a down-scaled version of the simulations in problem 8, so plots may not be very accurate. 

For reproducing the exact data used in the report, see (and run) the included Bash script `run_repeat_trials.sh`.

### Running the main program manually
1. Compile using:
    ```bash
    make all
    ```
    This will make both a serial version `main` and a parallelized version `main_openmp` of the main program.

2. The main program may then be run using the following optional arguments: 
    ```
    Usage:
        ./build/main [options]          (Serial)
        ./build/main_openmp [options]   (Parallelized)

    Options:
        --lattice_size, -L              size of the lattice (default: 2)
        --coupling,     -J              coupling constant (default: 1.0)
        --temperature,  -T              temperature (default: 1.0)
        --ordered                       start with ordered spins (default: false)

        --run, -r                       run MCMC simulation (default: false)
        --steps_per_cycle, -n_steps     number of steps per cycle (default: 1000)
        --total_cycles,    -n_cycles    number of total cycles (default: 100)
        --burn_in_cycles,  -n_burn      number of burn-in cycles (default: 0)
        --seed                          random seed (default: 1234)

        --temperature_scan,  -T_scan    perform temperature scan (default: false)
        --temperature_min,   -T_min     minimum temperature for scan (default: 2.1)
        --temperature_max,   -T_max     maximum temperature for scan (default: 2.4)
        --temperature_steps, -T_steps   number of temperature steps (default: 30)

        --outdir,                       output directory (default: out/)
        --name,                         run name (default: autogenerates based on parameters)
        --help, -h                      display this help message
    ```



### Python scripts

TODO

## Authors 
- Magnus Axelsen
- Isak Cecil Onsager Rukan
