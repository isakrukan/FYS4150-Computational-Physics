#!/bin/bash

# See: https://www.uio.no/english/services/it/research/hpc/lightweighthpc/

# Parameters
n_cycles=100000
n_burn=$((n_cycles/10))
n_steps=10000
T_steps=100

# Set number of available threads for OpenMP
# Here: Use 32 to not exceed 1/4 of available cores on bioint01
export OMP_NUM_THREADS=32


# Run repeat simulations
for repeat_idx in {1..10}; do
    for L in 40 60 80 100; do
        build/main_openmp \
            -T_scan \
            -T_steps "$T_steps" \
            -L "$L" \
            -n_burn "$n_burn" \
            -n_cycles "$n_cycles" \
            -n_steps "$n_steps" \
            --seed $((42*repeat_idx)) \
            --name "scan_${repeat_idx}" \
            --outdir "out/temperature_scans/repeat_runs/results-L${L}"
    done
done



# ---------------------------------------------------------
# Notes: (Running on bioint01 using 32 threads)
# ---------------------------------------------------------
#
# OLD:
# Results using previous build w/ arma matrix for grid:
#
#       1e6 cycles -> ~ 40.0 min per L
#
# CURRENT:
# For less autocorrelation, try running fewer cycles but
# more repeats:
#
#       1e5 cycles -> ~ 2.0-3.0 min per L
#                  -> ~ 1.5-2.0 hrs for 10 repeats
#
# ---------------------------------------------------------