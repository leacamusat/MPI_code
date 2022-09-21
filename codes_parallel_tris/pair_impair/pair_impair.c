#include <mpi.h>
#include <stdio.h>
#include <complex.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../common.h"
#include "pair_impair.h"

/**
 * @brief est-ce qu'on tri à gauche ou 
 *        est-ce qu'on tri à droite
 * 
 * @param recbuf 
 * @param size_recbuf 
 * @param tempbuf 
 * @param size_tempbuf 
 * @param left 
 */
void tri_pair_impair(int * recbuf, int size_recbuf, int * tempbuf, int size_tempbuf, int left){
    int * vec_tot = fusion(recbuf, tempbuf, size_recbuf, size_tempbuf); //tri par insertion des deux listes 
    if(left) {//si on est le processus à gauche de l'échange, on conserve les plus petits 
        for (int i = 0; i < size_recbuf; i++) {
            recbuf[i]=vec_tot[i]; //on conserve la partie de gauche du vecteur (les plus petits)
        }  
    } else {
        for (int i = 0; i < size_recbuf; i++) {//si on est le processus à droite de l'échange, on conserve les plus grands (pour conserver l'ordre)
            recbuf[i]=vec_tot[i+size_tempbuf];//on conserve la partie de droite du vecteur (les plus grands)
        }
    }
    free(vec_tot);
}

int main(int argc, char** argv){
    int N; //nb of numbers by array
    int rank;
    int size;//nb of clusters 
    int tabsize;

    if(MPI_Init(&argc, &argv) < 0){
        die_errno(MPI_INIT_MESSAGE_ERROR);
    }
  
    if(MPI_Comm_size(MPI_COMM_WORLD, &size) < 0){
        die_errno(MPI_COMM_SIZE_MESSAGE_ERROR);
    }

    if(MPI_Comm_rank(MPI_COMM_WORLD, &rank) < 0){
        die_errno(MPI_COMM_RANK_MESSAGE_ERROR);
    }
  
    if (argc > 1){
        tabsize = atoi(argv[1]);
        N = tabsize / size;
    }

    //buffer de réception (éà créer pour chaque processus)
    int *recbuf = malloc(N * sizeof(int));
    if(recbuf == NULL){
        die_errno(ALLOCATION_MEMORY_MESSAGE_ERROR);
    }

    int *sendbuf=NULL;
    if (rank == ROOT){
        //buffer d'envoi (à créer pour le root) 
        sendbuf = malloc(tabsize * sizeof(int));
        if(sendbuf == NULL){
            die_errno(ALLOCATION_MEMORY_MESSAGE_ERROR);
        }

        fill_vec(sendbuf, tabsize, MAX);

    }

    /**
     * @brief envoi d'une sous liste à chaque processus
     * 
     */
    if(MPI_Scatter(sendbuf, N, MPI_INT, recbuf, N, MPI_INT, 0, MPI_COMM_WORLD) < 0){
        die_errno(MPI_SCATTER_MESSAGE_ERROR);
    }
    
    //tout le monde a trié sa liste en local
    bubble_sort(recbuf, N);
    
    int *tempbuf = NULL;
    tempbuf = malloc(N * sizeof(int));   
    if(tempbuf == NULL){
        die_errno(ALLOCATION_MEMORY_MESSAGE_ERROR);
    }

    /**
     * @brief temps initial (en secondes)
     * 
     */
    double start_time = MPI_Wtime();

    //boucle sur tous les processus (à chaque tour de boucle on fait la phase paire ET la phase impaire)
    for (int i = 0; i < size; i++) {

        /**
         * @brief phase impaire 
         * 
         */
        
        //vers la droite 
        if ((rank%2==0) && ((rank+1)<size)) { //si on est pair et qu'on a un voisin de droite avec qui échanger, on envoie et reçoit à droite 
            if(MPI_Sendrecv(recbuf, N, MPI_INT, rank+1, 0,tempbuf, N, MPI_INT,rank+1,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) < 0){
                die_errno(MPI_SENDRECV_MESSAGE_ERROR);
            }
            tri_pair_impair(recbuf, N, tempbuf, N,1);
        }

        //vers la gauche 
        if (rank % 2 == 1){ //si on est impair dans tous les cas on reçoit un tableau et on envoie à gauche
            if(MPI_Sendrecv(recbuf, N, MPI_INT, rank-1, 0, tempbuf, N, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) < 0){
                die_errno(MPI_SENDRECV_MESSAGE_ERROR);
            }
            tri_pair_impair(recbuf, N, tempbuf, N,0);
        }
    
        /**
         * @brief phase paire
         * 
         */

        //vers la droite
        if ((rank%2==1) && ((rank)<(size-1))){//si on est impair et qu'on a un voisin à droite on envoie et on recoit à droite 

            if(MPI_Sendrecv(recbuf, N, MPI_INT, rank+1, 0, tempbuf, N, MPI_INT, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) < 0){
                die_errno(MPI_SENDRECV_MESSAGE_ERROR);
            }

            tri_pair_impair(recbuf, N, tempbuf, N,1);
        }
        
        //vers la gauche 
        if ((rank%2==0) && (rank!=0)){//si on est pair et pas de rang 0 on recit et envoie avec son voisin de gauche 

            if(MPI_Sendrecv(recbuf, N, MPI_INT, rank-1, 0, tempbuf, N, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) < 0){
                die_errno(MPI_SENDRECV_MESSAGE_ERROR);
            }

            tri_pair_impair(recbuf, N, tempbuf, N,0);
        }
        
    }

    free(tempbuf);

    //on récupère tous les sous tableaux triés
    if(MPI_Gather(recbuf, N, MPI_INT, sendbuf, N, MPI_INT, 0, MPI_COMM_WORLD) < 0) {
        die_errno(MPI_GATHER_MESSAGE_ERROR);
    }

    /**
     * @brief on ajoute les résultats dans le .cvs
     *        à chaque itération
     * 
     */
    char *filename = "result_pair_impair.csv";
    calculate_result(rank, recbuf, size, tabsize, start_time, filename);

    if (!rank){
        free(sendbuf);
    }

    free(recbuf);

    if(MPI_Finalize() < 0){
        die_errno(MPI_FINALIZE_MESSAGE_ERROR);
    }
}
