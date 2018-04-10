#!/bin/bash

#SBATCH --job-name="Parallel Blendering"

#SBATCH --get-user-env

#SBATCH -c 40
#SBATCH -t 1:00:00
#SBATCH -w granger[4]
#SBATCH --output="job_a.out"
#SBATCH --error="job_a.err"

./pblend Star_Collapse.blend -j -s 1 -e 125 -f 24 -c 40 -b explosion/star_coll_%04d.png

