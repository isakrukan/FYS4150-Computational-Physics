#!/bin/bash

# ---
#
# This is a script used to run simulations for problem 8.
# It runs 10 repeat simulations w/ 1e6 MC cycles for each
# lattice size L ∈ [40, 60, 80, 100].
#
# Not reccomended to run locally due to long runtimes.
#
# Ex: I ran this on bioint01 from
# https://www.uio.no/english/services/it/research/hpc/lightweighthpc/
#
# ---


# Parameters
n_cycles=1000000
n_burn=$((n_cycles/10))
T_steps=100

# Set number of available threads for OpenMP
# Here: Use 32 to not exceed 1/4 of available cores on bioint01
export OMP_NUM_THREADS=32

# Create log directory & file
mkdir -p "logs"
log="logs/repeat_runs.log"


# Start log
echo "-----------------------------------------------------" >> "${log}" 2>&1
echo "[$(date +"%H:%M:%S")] Starting repeat simulations..." >> "${log}" 2>&1
echo "-----------------------------------------------------" >> "${log}" 2>&1

# Run repeat simulations
for repeat_idx in {1..10}; do
    for L in 40 60 80 100; do
        
    outdir="out/temperature_scans/repeat_runs/results-L${L}"
    mkdir -p "${outdir}"

        echo "[$(date +"%H:%M:%S")] Running L=${L}, repeat=${repeat_idx}" >> "${log}" 2>&1
        
        build/main_openmp \
            -T_scan \
            -T_steps "$T_steps" \
            -L "$L" \
            -n_burn "$n_burn" \
            -n_cycles "$n_cycles" \
            --seed $((42*repeat_idx)) \
            --name "scan_${repeat_idx}" \
            --outdir "${outdir}" \
            >> "${log}" 2>&1
    done
done

# End log
wait
echo "-----------------------------------------------------" >> "${log}" 2>&1
echo "[$(date +"%H:%M:%S")] All simulations completed." >> "${log}" 2>&1
echo "-----------------------------------------------------" >> "${log}" 2>&1
