#!/bin/bash

#SBATCH -w slurm[2]
#SBATCH -t 1:00:00
#SBATCH -n 1

BLENDER="/usr/bin/blender"

FILE=$1
START=$2
END=$3
CHILDREN=$4

DIFF=`echo "($END - $START + 1) / $CHILDREN" | bc`

for ((i=0; i<$CHILDREN; i++)); do
	S=`echo "$i * $DIFF + $START" | bc`
	E=`echo "$S + $DIFF - 1" | bc`
	srun $CHILDREN $BLENDER -b $FILE -s $S -e $E -t 1 -o ./out/\#\#.png -F PNG -a&
done;


