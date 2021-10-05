# Investigating MPI process and OpenMP thread affinity

The demo code [affinity_test.c](../demos/affinity/affinity_test.c) performs a dummy calculation
and prints out the affinity mask for each MPI process and OpenMP thread. Code can be compiled
with the included [Makefile](../demos/affinity/Makefile). Run the code with different OpenMP thread pinning settings
(OMP_PLACES, OMP_PROC_BIND) via the batch job system in Puhti and in Mahti. Try also cases where
you have less OpenMP threads than requested with `--cpus-per-task`.

You may try also printing the affinity mask with OpenMP environment variables (OMP_DISPLAY_AFFINITY)

In CSC's Slurm configuration it is quite difficult oversubscribe the cores, but if you manage to
do so, you should see significant increase in the time of the dummy calculation.
