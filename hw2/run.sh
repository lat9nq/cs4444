#!/bin/bash

#SBATCH --job-name="Parallel Blendering"

#SBATCH --get-user-env

#SBATCH -t 1:00:00
#SBATCH -w slurm2
#SBATCH -c 2
#SBATCH --output="job.out"
#SBATCH --error="job.err"

srun ./pblend sc1_sh5_asteroid_hit.blend -s 1 -e 60 -f 24 -c 2

