#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "tri_fusion.h"
#include "../common.h"

/**
 * @brief fonction permettant de mettre 2 à la puissance i en utilisant les décalage binaire 
 * 
 * @param i 
 * @return int 
 */
int pow2(int i){
    return 1<<i;
}

int chunk_size(int rank, int N, int P){
  return (N%P > rank) ? (N/P) + 1 : N/P; 
}

int sizemerge( int* sendcounts, int i, int rank_snd){
    int sum=0;
    for (int j=rank_snd;j<pow2(i)+rank_snd;j++){
        sum+=sendcounts[j];
    }
    return sum;
}

/**
 * @brief ceci est une version du tri fusion où le nombre de processus est une puissance de 2, 
 *        et la taille de la liste est divisible par le nombre de processus.
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char** argv) {
    int N = N_ARRAY; //nb of numbers by array 
    int rank; //workers
    int size; //nb of clusters 
    int *sendbuf = NULL; //buffer d'envoi (à créer pour le root)
    
    if(MPI_Init(&argc, &argv) < 0){
        die_errno(MPI_INIT_MESSAGE_ERROR);
    }
 
    double start_time = MPI_Wtime();

    if(MPI_Comm_size(MPI_COMM_WORLD, &size) < 0){
        die_errno(MPI_COMM_SIZE_MESSAGE_ERROR);
    }

    if(MPI_Comm_rank(MPI_COMM_WORLD, &rank) < 0){
        die_errno(MPI_COMM_RANK_MESSAGE_ERROR);
    }

    //gestion des nombres impairs de tailles de tableau/processus
    int tabsize = N*size;
    if (argc > 1){
        tabsize = atoi(argv[1]);
    }

    /**
     * @brief buffer d'envoi (à créer pour le root)
     *        que l'on le rempli de valeur aléatoires
     * 
     */
    if (rank == ROOT){
        sendbuf = malloc(tabsize * sizeof(int));
        if(sendbuf == NULL){
            die_errno(ALLOCATION_MEMORY_MESSAGE_ERROR);
        }
        fill_vec(sendbuf, tabsize, MAX);
    }
    //préparation des arguments scatterv connus pas tous les processus.
    int *sendcounts = NULL; 
    int *displs = NULL; 
    sendcounts = malloc(size * sizeof(int));
    if(sendcounts == NULL){
        die_errno(ALLOCATION_MEMORY_MESSAGE_ERROR);
    }
    displs = malloc(size * sizeof(int));
    if(displs == NULL){
        die_errno(ALLOCATION_MEMORY_MESSAGE_ERROR);
    }
    displs[0] = 0; //on commence toujours par zéro
    
    for (int i=0;i<size;i++){
        sendcounts[i] = chunk_size(i,tabsize,size);
        if(i != size-1){
            displs[i+1] = displs[i] + sendcounts[i];
        }            
    }

    /**
     * @brief buffer de réception (à créer pour chaque processus)
     * 
     */
    int *recbuf = malloc( sendcounts[rank] * sizeof(int));
    if(recbuf == NULL){
        die_errno(ALLOCATION_MEMORY_MESSAGE_ERROR);
    }
    /**
     * @brief envoi d'une sous liste à chaque processus
     * 
    */
    if(MPI_Scatterv(sendbuf, sendcounts, displs, MPI_INT, recbuf, sendcounts[rank], MPI_INT, 0, MPI_COMM_WORLD) < 0){
        die_errno(MPI_SCATTER_MESSAGE_ERROR);
    }

    free(sendbuf);
    free(displs);

    /**
     * @brief tri local de chaque sous tableau pour chaque processus
     * 
     */
    bubble_sort(recbuf, sendcounts[rank]);
  
    int i = 0;
    int step = size;
    int *voisin = NULL;
    int *merge = NULL;
    //-----------------------------------------------
    int has_send_one_time = 0;
    int must_send;
    int must_receive;

    /**
     * @brief temps initial (en secondes)
     * 
     */
    
    while (!has_send_one_time) {
        must_send = 0;
        must_receive = 0;

        /**
         * @brief check pair / impaire
         *        si c'est 0 % étape en cours
         *        si c'est impair on reçoit
         * 
         */
        //processus en puissances de deux
        if (!(rank & (pow2(i+1) - 1))){
            must_receive=1;
        } else {
            if (has_send_one_time == 0){
                must_send = 1;
                has_send_one_time = 1;
            }
        }

        /**
         * @brief envoi pour chaque process
         * 
         */
        if (must_send){
            // 
            if (i == 0){
                if(MPI_Send(recbuf,sendcounts[rank], MPI_INT, (rank-pow2(i)), 0, MPI_COMM_WORLD) < 0){
                    die_errno(MPI_SEND_MESSAGE_ERROR);
                }
            } else {
                if(MPI_Send(merge, sizemerge(sendcounts,i,rank) , MPI_INT, (rank-pow2(i)), 0, MPI_COMM_WORLD) < 0){
                    die_errno(MPI_SEND_MESSAGE_ERROR);
                }
            }
        }

        /**
         * @brief reception pour chaque processus
         * 
         */
        if (must_receive){
            voisin=malloc(sizeof(int)*sizemerge(sendcounts,i,rank+pow2(i)));
            if(voisin == NULL){
                die_errno(ALLOCATION_MEMORY_MESSAGE_ERROR); 
            }
        
            if(MPI_Recv(voisin, sizemerge(sendcounts,i,rank+pow2(i)), MPI_INT, rank+pow2(i), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) < 0) {
                die_errno(MPI_RECV_MESSAGE_ERROR);
            }

            if (i == 0) {
              //tri par insertion de deux listes triées
                merge = fusion(recbuf, voisin, sendcounts[rank], sizemerge(sendcounts,i,rank+pow2(i)));
                free(voisin);
            }
            else {
                merge = fusion(merge, voisin, sizemerge(sendcounts,i,rank), sizemerge(sendcounts,i,rank+pow2(i)));//par fusion
                free(voisin);
            }
        }
        
        i+=1;
        /**
         * @brief step = nbr de fusion à effectuer (au début taille de tableau max.)
         * 
         */
        step = step>>1;
        if (step == 1){
            has_send_one_time=1;
        }
    }

    free(sendcounts);
    free(recbuf);

    /**
     * @brief on ajoute les résultats dans le .cvs
     *        à chaque itération
     * 
     */
    char *filename = "result_tri_fusion.csv";
    calculate_result(rank, merge, size, tabsize, start_time, filename);
    
    free(merge);

    if(MPI_Finalize() < 0){
        die_errno(MPI_FINALIZE_MESSAGE_ERROR);
    }
}
