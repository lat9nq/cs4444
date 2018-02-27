#!/bin/bash

#SBATCH -w slurm[2]
#SBATCH -t 1:00:00
#SBATCH -n 1
#SBATCH -c 1
#SBATCH --mem-per-cpu=2

#SBATCH --output="pblend.out"
#SBATCH --error="pblend.err"
#SBATCH --account=lat9nq

BLENDER="/usr/bin/blender"

FILE=$1
START=$2
END=$3
CHILDREN=$4

DIFF=`echo "($END - $START + 1) / $CHILDREN" | bc`

for ((i=0; i<$CHILDREN; i++)); do
	S=`echo "$i * $DIFF + $START" | bc`
	E=`echo "$S + $DIFF - 1" | bc`
	BLEND="$BLENDER -b $FILE -s $S -e $E -a -t 1"
	echo $BLEND
	srun -c1 -n1 -t1:00:00 --mem-per-cpu=2 --output=job_`printf %03d $i`.out $BLEND &
done;


