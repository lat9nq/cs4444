#!/bin/bash

#SBATCH --job-name="Parallel Blendering"

#SBATCH --get-user-env

#SBATCH -t 1:00:00
#SBATCH -w granger[3]
#SBATCH -c 2
#SBATCH --output="job.out"
#SBATCH --error="job.err"

srun -n1 ./pblend sc1_sh5_asteroid_hit.blend -c 2 -f 24 -s 1 -e 60

