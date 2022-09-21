CC=mpicc
OUTPUT=tri_fusion
CFLAGS=-O3 -Wall -Wextra -pedantic -g
#LDFLAGS=-fsanitize=address -fsanitize=leak -fsanitize=undefined 
LDFLAGS=
TAILLE=11000
N_PROC=4

all: $(OUTPUT)

$(OUTPUT): tri_fusion.o common.o
	$(CC) tri_fusion.o ../common.o -o $(OUTPUT) $(LDFLAGS)

tri_fusion.o: tri_fusion.c tri_fusion.h 
	$(CC) $(CFLAGS) -c tri_fusion.c -o tri_fusion.o 

common.o: ../common.c ../common.h
	$(CC) $(CFLAGS) -c ../common.c -o ../common.o


clean:
	rm -f *.o $(OUTPUT)


rebuild: clean all

run: $(OUTPUT)
	mpirun --host master:4,worker1:4,worker2:4,worker3:4 --use-hwthread-cpus -np $(N_PROC)  $(OUTPUT) $(TAILLE)

run_local: $(OUTPUT)
	mpirun --use-hwthread-cpus -np $(N_PROC)  $(OUTPUT) $(TAILLE)
