#!/bin/bash
for i in 2 4 8 16 # 
do
	for j in {1..10} # 
	do
	#	mpirun --host $workteam -np $nb_nodes para $iterations
	#	make run N_PROC=$nb_nodes OVER=-oversubscribe TAILLE=1500000
		make run N_PROC=$i TAILLE=32000000
	done	
done
