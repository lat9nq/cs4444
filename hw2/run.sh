#!/bin/bash

#SBATCH --job-name="Parallel Blendering"

#SBATCH --get-user-env

#SBATCH --exclusive
#SBATCH -c 10
#SBATCH -t 1:00:00
#SBATCH -w granger[6]
#SBATCH --output="job_10.out"
#SBATCH --error="job_10.err"

./pblend Star_Collapse.blend -s 1 -e 250 -f 24 -c 10 -b explosion/star_coll_%04d.png -j

