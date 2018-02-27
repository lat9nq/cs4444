#!/bin/bash

#SBATCH --job-name="Parallel Blendering"

#SBATCH --get-user-env

#SBATCH -t 1:00:00
#SBATCH -w granger[3]
#SBATCH -c 2
#SBATCH --output="job.out"
#SBATCH --error="job.err"

srun -n1 ./pblend.sh sc1_sh5_asteroid_hit.blend 1 60 2

