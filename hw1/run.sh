#!/bin/bash

#SBATCH --job-name="Test of Slurm?"

#SBATCH --get-user-env

#SBATCH -N 3
#SBATCH -t 0:00:05
#SBATCH -w granger[3]
##SBATCH -p sbatch

#SBATCH --error="my_job.err"
#SBATCH --output="my_job.out"

srun -n1 $1 a.in 0.5

