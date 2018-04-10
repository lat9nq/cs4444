#!/bin/bash

#SBATCH -w granger[4]
#SBATCH -c 40
#SBATCH --job-name="Heated plate"
#SBATCH -t 1:00:00
#SBATCH --output="job.out"
#SBATCH --error="job.err"

./heated_plate 10000 10000 10000 40
