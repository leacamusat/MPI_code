CC=mpicc
OUTPUT=pair_impair
CFLAGS=-O3 -Wall -Wextra -pedantic -pedantic -g -std=gnu11
LDFLAGS=-fsanitize=address -fsanitize=leak -fsanitize=undefined
LDFLAGS=
TAILLE=16000000

all: $(OUTPUT)

$(OUTPUT): common.o pair_impair.o 
	$(CC) pair_impair.o ../common.o -o $(OUTPUT) $(LDFLAGS)

pair_impair.o: pair_impair.c pair_impair.h 
	$(CC) $(CFLAGS) -c pair_impair.c -o pair_impair.o 

common.o: ../common.c ../common.h 
	$(CC) $(CFLAGS) -c ../common.c -o ../common.o

clean:
	rm -f *.o ../*.o $(OUTPUT)


rebuild: clean all

run: $(OUTPUT)
	mpirun --host master:4,worker1:4,worker2:4,worker3:4 --use-hwthread-cpus -np $(N_PROC)  $(OUTPUT) $(TAILLE)

run_local: $(OUTPUT)
	mpirun --use-hwthread-cpus -np $(N_PROC)  $(OUTPUT) $(TAILLE)