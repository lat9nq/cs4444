#!/bin/bash

#SBATCH --job-name="Parallel Blendering"

#SBATCH --get-user-env

#SBATCH -c 40
#SBATCH -t 1:00:00
#SBATCH -w granger[4]
#SBATCH --output="job_a.out"
#SBATCH --error="job_a.err"

./pblend BikePump.blend -s 1 -e 125 -f 24 -c 40 -b pump/bike_pump_%04d.png

