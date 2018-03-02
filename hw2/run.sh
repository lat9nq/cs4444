#!/bin/bash

#SBATCH --job-name="Parallel Blendering"

#SBATCH --get-user-env

#SBATCH -c 10
#SBATCH -t 1:00:00
#SBATCH -w granger[3]
#SBATCH --output="job.out"
#SBATCH --error="job.err"

./pblend BikePump.blend -s 1 -e 250 -f 24 -c 10

