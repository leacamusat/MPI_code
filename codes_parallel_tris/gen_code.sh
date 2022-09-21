#!/bin/bash

nb_nodes=$1 
tab_size=$2
iterations=$3

workteam="master"

for ((k=1;k<$tab_size;k++))
do

for ((i=1;i<$nb_nodes;i++))
do
    workteam=$workteam",worker"$i

done

for ((j=1;j<=$iterations;j++))
do
#	mpirun --host $workteam -np $nb_nodes para $iterations
    echo "mpirun --host "$workteam" -np "$nb_nodes" tri_fusion "$iterations
done
