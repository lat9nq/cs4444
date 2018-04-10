#!/bin/bash

#SBATCH --job-name="Parallel Blendering"

#SBATCH --get-user-env

#SBATCH -c 40
#SBATCH -t 1:00:00
#SBATCH -w granger[5]
#SBATCH --output="job_b.out"
#SBATCH --error="job_b.err"

./pblend Star_Collapse.blend -s 126 -e 250 -f 24 -c 40 -b explosion/star_coll_%04d.png

