# MPI_code
# ALGORITHMS
---

## tri fusion
cd codes_parallel_tris/tri_fusion

#compile

make

#run

make run N_PROC=<nbr_processus>

#clean repo (.o and executable repo deletion)

make clean

---

#pair impair

cd codes_parallel_tris/pair_impair

#compile

make

#run

make run N_PROC=<nbr_processus>

#clean repo (.o and executable repo deletion)

make clean

---

## automates parallel 

cd fractales_automate

make fractal_mpi

./fractal_mpi

## fractales parallel

cd fractales_automate

make GOL_mpi

./GOL_mpi
