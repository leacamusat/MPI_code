#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"

/**
 * @brief fonction permettant d'afficher un vecteur 
 * 
 * @param vec 
 * @param size 
 */
void print_vec(int * vec, int size){
    for (int i = 0; i < size; i++) {
        if (i==0){
            printf("(");
        }
        printf("%d ", vec[i]);
    }
    printf(")\n");
}

/**
 * @brief Bubble sort compléxité en Θ(n2).
 * 
 * @param arr 
 * @param len 
 */
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void bubble_sort(int *arr, int len){
    qsort(arr, len, sizeof(int), cmpfunc);
}


/**
 * @brief remplissage d'un vecteur avec des size valeurs randoms entre 0 et max comme valeurs 
 * 
 * @param vec
 * @param size 
 * @param max 
 */
void fill_vec(int* vec, int size, int max){
    srand(time(NULL));
    for (int i=0; i<size; i++){
        vec[i]=(rand() % max+1);
    }
}

/**
 * @brief tri par insertion
 * 
 * @param vec1 
 * @param vec2 
 * @param size1 
 * @param size2 
 * @return int* 
 */
int *fusion(int *vec1, int *vec2, int size1, int size2){
    int *res = malloc((size1 + size2) * sizeof(int));
    if(res == NULL) {
        die_errno(ALLOCATION_MEMORY_MESSAGE_ERROR);
    }
    int l = 0;
    int j = 0;    

    /**
     * @brief On vérifie que chaque vecteur a bien la même taille
     * 
     */
    if (size1 == size2){
        for(int i=0; i<(size1+size2); i++){
            if (((vec1[l]>vec2[j]) || (l==size2)) && (j<size1)){
                res[i] = vec2[j];
                j += 1;
            } else {
                res[i] = vec1[l];
                l += 1;
            }
        }
    } else {
        for(int i=0; i<(size1+size2); i++){
            if (((vec1[l]>vec2[j]) || (l==size1)) && (j<size2)){
                res[i] = vec2[j];
                j += 1;
            } else {
                res[i] = vec1[l];
                l += 1;
            }
        }
    }

    return res;
}


/**
 * @brief écrit le résultat dans un .csv
 * 
 * @param rank 
 * @param merge 
 * @param size 
 * @param N 
 * @param start_time 
 */
void calculate_result(int rank, int *merge, int n_proc, int tabsize, double start_time, char *filename){
    double total_time = MPI_Wtime() - start_time;
    double recdata;

    /**
     * @brief somme du temps d'exécution de tout les processus
     * 
     */
    if(MPI_Reduce(&total_time, &recdata, 1, MPI_DOUBLE, MPI_MAX, 0,MPI_COMM_WORLD) < 0){
        die_errno(MPI_REDUCE_MESSAGE_ERROR);
    }

    /**
     * @brief le processus 0 récupère le temps max d'exécution 
     *        de tous les processus
     * 
     */
    if (rank == ROOT){
        FILE * f;
        f = fopen(filename, "a");
        if (f == NULL) {
            die_errno(FOPEN_ERROR_MESSAGE);
        }

        if(fprintf(f,"%d,%f,%d\n", n_proc, recdata, tabsize) < 0){
            die_errno(FPRINT_ERROR_MESSAGE);
        }

        fclose(f);
    }
}
