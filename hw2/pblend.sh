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
SRUN="srun -c1 -n1 -t1:00:00 --mem-per-cpu=2"

FILE=$1
START=$2
END=$3
CHILDREN=$4
JUMP=$5

DIFF=`echo "($END - $START + 1) / $CHILDREN" | bc`

if [ $JUMP != "1" ]
then
	for ((i=0; i<$CHILDREN; i++)); do
		S=`echo "$i * $DIFF + $START" | bc`
		E=`echo "$S + $DIFF - 1" | bc`
		BLEND="$BLENDER -t 1 -b $FILE -s $S -e $E -a"
		echo $BLEND
		$SRUN --output=job_`printf %03d $i`.out $BLEND &
	done;
else
	for ((i=0; i<$CHILDREN; i++)); do
		S=`expr $i + 1`
		E=`echo "$END - ( $CHILDREN - $i ) + 1" | bc`
		BLEND="$BLENDER -t 1 -b $FILE -s $S -j $CHILDREN -e $E -a"
		echo $BLEND
		$SRUN --output=job_`printf %03d $i`.out $BLEND &
	done;
fi;


