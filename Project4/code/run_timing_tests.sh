#!/bin/bash

# ---
# 
# This is a script used to run timing tests for OpenMP parallelization
# in problem 7. It runs 5 trials of a temperature scan with 1e4 MC cycles
# and lattice size L=5. The number of threads is varied from 1 to 16.
# Ouput is saved to out/times_openmp.txt.
#
# ---

# Parameters
n_cycles=10000
n_burn=$((n_cycles/10))
T_steps=128
L=5

# Set number of available threads for OpenMP (adjust as needed)
NUM_TRIALS=5
MAX_NUM_THREADS=16

# Prepare output file, clear contents if it exists
mkdir -p out
OUTFILE="out/times_openmp.txt"
> "$OUTFILE"

echo "Running OpenMP timing tests..."

# Loop over number of threads
for NUM_CORES in $(seq 1 $MAX_NUM_THREADS); do

    # Set number of threads
    export OMP_NUM_THREADS=$NUM_CORES
    echo -ne "\rCurrent: ${NUM_CORES} threads..."  

    # Prepare to collect times
    TIMES=""

    # Run trials
    for trial in $(seq 1 $NUM_TRIALS); do

        OUTPUT=$(build/main_openmp \
            -T_scan \
            -T_steps "$T_steps" \
            -L "$L" \
            -n_burn "$n_burn" \
            -n_cycles "$n_cycles" \
            --seed 42 \
            --save_data false)

        # Extract time from output
        TIMES="${TIMES} $(echo "$OUTPUT" | awk '{print $5}')"
    done

    # Save trial times to new line in out/times_openmp.txt
    echo -n "$TIMES" >> "out/times_openmp.txt"
    echo "" >> "out/times_openmp.txt"

done
    