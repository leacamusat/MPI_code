#!/bin/bash

for i in 2 4 8 16 # 
do
		for j in {1..10} # 
		do
			#	mpirun --host $workteam -np $nb_nodes para $iterations
			#	make run N_PROC=$nb_nodes OVER=-oversubscribe TAILLE=1500000
			#	echo "tri_fusion, nbr de proc $i,  iteration $j"
				make run N_PROC=$i TAILLE=74000000
			done
		done
