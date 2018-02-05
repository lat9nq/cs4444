#!/bin/bash

#SBATCH --job-name="Test of Slurm?"

#SBATCH --get-user-env

#SBATCH -N 1
#SBATCH -t 0:00:05
##SBATCH -p sbatch

#SBATCH --error="my_job.err"
#SBATCH --output="my_job.out"

srun -n1 optimized a.in
cat my_job.out

