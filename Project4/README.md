# Project 4: Computational Physics
This repository contains the code and documentation for Project 4 in the FYS3150 - Computational Physics course at the University of Oslo.

## About the project
This project investigates the Ising model and the Markov chain Monte Carlo (MCMC) method. We implement the MCMC method numerically and simulate the Ising model for various parameters. In particular, we estimate the critical temperature for our finite-sized lattice, and compare to Lars Onsager's exact [expression](https://journals.aps.org/pr/abstract/10.1103/PhysRev.65.117) for the infinite-sized lattice. Additionally, we explore how we can optimize our code both using i.a. parallelization.


## Folders
- **`code/`** All code used to generate results for this project.
- - **`code/include`** Header files for the project.
- - **`code/src`** All source code files for the project.
- - **`code/scripts`** Python scripts for plotting & analyzing results, relying primarely on data from **`code/out`**, created by various runs of **`code/main.cpp`** (see **`code/makefile`**).
- - **`code/build`** Build files.
- - **`code/out`** Output folder, for produced data.
- - **`code/log`** Log files for simulation durations.
- - **`code/tests`** Tests for our code.
- **`figs/`** All figures used in main.tex
- **`docs/`** Documentation of the code struture generated using doxygen [doxygen](https://www.doxygen.nl/)


# Code Implementation
To build and run the code, navigate to the **`code/`** directory. 

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
These scripts loads various data from the **`code/out`** directory. When mentioning data files below, these come from **`code/out`** unless mentioned otherwise, and `produced` figures are all stored to **`figs/`**.
- **`problem_1.py`** Prints all possible configurations of energy $E$, magnetization $M$ and number of spin-up locations, including degeneracy level.
- **`problem_4.py`** Loads the file **`MCMC-unordered-L2-T1p0/raw_data.csv`**, calculates analytic expressions for a few quantities of the $2\times 2$ Ising model, and produces two figures: **`errors_2x2.pdf`** displaying relative errors against Monte Carlo cycles, and **`ncycles_2x2.pdf`** showing the average energy per site against Monte Carlo cycles.
- **`problem_5.py`** Loads **`MCMC-X-L20-TY/raw_data.csv`**, where $X$ is either `ordered` or `unordered` and $Y= 1p0$ or $Y=2p4$. Creates figure **`equilibration.pdf`* showing average energy per site (included accumulated average) for various temperatures $T= 1p0$ and $T=2p4$, with an initial state either ordered or unordered.
- **`problem_6.py`** Loads same files as in **`problem_5.py`**. Produces the figure **`epsilon_distribution.pdf`** which shows the estimated energy distribution.
- **`problem_7.py`** Loads **`times_openmp.txt`**. Produces the figure **`timing_tests.pdf`** showing how the average amount of run time evolves as the number of threads increase. 
- **`problem_8.py`** Tries to load data from **`code/out/temperature_scans/repeat_runs/`** if these exists. If not, uses data from **`code/out/temperature_scans/tests/`**. Produces **`temp_scan.pdf`** which show how various observables evolve as functions of temperature, and **`temp_scan_error_.pdf`** (by default), which is the corresponding figrure displaying the errors.
- **`problem_9.py`** Tries to load in the same way as **`problem_8.py`**. Estimates the critical temperature for finite-sized lattice and extrapolates to the infinite-sized lattice case by linear regression.
-  **`utils.py`** Contains a couple of utility functions: **latex_fonts** for producing nice figures, **combine_runs** for combining observables from various simulations and **estimate_T_infinite** for estimating the critical temperature at infinite lattice size using linear regression.

## Authors 
- Magnus Axelsen
- Isak Cecil Onsager Rukan
