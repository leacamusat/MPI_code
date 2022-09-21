#include <mpi.h>
#include <stdio.h>
#include <complex.h>
#include "create_matrix.h"
#include "lea_png.h"
#include <time.h>    // time()
#include <stdlib.h>


int M=2;

//création d'une matrice de rows lignes et columns colonnes
int** create_image_int(int rows, int columns){
    printf("%d, %d, %ld \n", rows, columns, sizeof(int*));
    int ** mat=malloc(rows*sizeof(int*)) ; // allocation mémoire pour stocker size adress
    
    int* vec_tot=malloc(columns*rows*sizeof(int));
    srand(time(0)); //Randomise seed initialisation
    for (int i=0; i<rows; i++){
        mat[i]=vec_tot+i*columns;
        for (int j=0; j<columns; j++){
            mat[i][j]=rand() % 2;
        }
    }
    return mat;
}

//reshape de l'image d'origine et symétrie
int** reform_image(int * vec, int nb_rows,  int nb_columns){
    
    int** mat =create_image_int(nb_rows, nb_columns);
    int index=0;
    for (int i=0; i<nb_rows; i++){
        for (int j=0;j<nb_columns; j++){
            
            mat[i][j]=vec[index];
            printf("%d vec[index] \n ", vec[index]);
            index+=1;
        } 
    }
    
    return mat;
}

void concatenate(int * concat, int * tokenprec, int * recbuf, int * tokensucc,  int nb_rows_by_proc, int nb_columns, int ite){
  int temp=0;
  for (int j=0; j<nb_columns; j++){
    concat[temp]=tokenprec[j];
    temp+=1;
  }

  for (int i=0; i<nb_rows_by_proc*nb_columns; i++){
      if (ite==0){
        concat[temp]=recbuf[i];
      }
      temp+=1;
  }
  

  for (int j=0; j<nb_columns; j++){
    concat[temp]=tokensucc[j];
    temp+=1;
  }

  
}


void cellulaire(int *vec_entree, int *vec_sortie, int m, int nb_rows_by_proc){
    
    int jleft;
    int ileft;
    int iright;
    int jright;
    printf("reform image \n");
    int ** mat_entree=reform_image(vec_entree, nb_rows_by_proc+2, m);
    printf("reform image \n");
    int n=nb_rows_by_proc+2;
    int temp=0;
    int value;
    printf("%d, \n ", n-3);
    for (int i=0; i<n; i++){
   
        ileft=(n+i-1)%n;
        iright=(i+1)%n;
   	    printf("boucle GOL\n");
        for (int j=0;j<m; j++){
        
            
            jleft=(m+j-1)%m;
            jright=(j+1)%m;
            
            /*
            int tl = ..., tc =..., tr = ...,
            	cl = ..., cr = ...,
            	bl = ..., bc = ..., br = ...;
            int value = tl ^ tc ^ tr ^ cl ^ cr ^ bl ^ bc ^ br;*/
            
            
            value =(mat_entree[i][jleft]+mat_entree[i][jright]+mat_entree[ileft][jright]+mat_entree[ileft][j]+mat_entree[ileft][jleft]+mat_entree[iright][j]+mat_entree[iright][jright]+mat_entree[iright][jleft]);//la parité de tous les voisins
            
            
            
            vec_sortie[temp]=value%2;     
              

            printf("vec sortie %d \n", vec_sortie[temp]);
            temp+=1;

    }
    
    
    
  }
  printf("fin de GOL \n ");
  free(mat_entree);
}

void game_of_life(int *vec_entree, int *vec_sortie, int m, int nb_rows_by_proc){
    
    int jleft;
    int ileft;
    int iright;
    int jright;
    printf("reform image \n");
    int ** mat_entree=reform_image(vec_entree, nb_rows_by_proc+2, m);
    printf("reform image \n");
    int n=nb_rows_by_proc+2;
    int temp=0;
    int value;
    printf("%d, \n ", n-3);
    for (int i=0; i<n; i++){
   
        ileft=(n+i-1)%n;
        iright=(i+1)%n;
   	    printf("boucle GOL\n");
        for (int j=0;j<m; j++){
        
            
            jleft=(m+j-1)%m;
            jright=(j+1)%m;
            
            /*
            int tl = ..., tc =..., tr = ...,
            	cl = ..., cr = ...,
            	bl = ..., bc = ..., br = ...;
            int value = tl ^ tc ^ tr ^ cl ^ cr ^ bl ^ bc ^ br;*/
            
            
            value =(mat_entree[i][jleft]+mat_entree[i][jright]+mat_entree[ileft][jright]+mat_entree[ileft][j]+mat_entree[ileft][jleft]+mat_entree[iright][j]+mat_entree[iright][jright]+mat_entree[iright][jleft]);//la parité de tous les voisins
            
            
            printf("%d || %d value \n ", mat_entree[i][j], value);
            if (mat_entree[i][j]==1){
              if (value ==2 || value==3 ){
            	 vec_sortie[temp]=1;     
              } 
              else {
                vec_sortie[temp]=0; 
              }
            }
            else {
              if (value==3){
                printf("Ok");
            	  vec_sortie[temp]=1; 
              }
              else {
                vec_sortie[temp]=0; 
              }
            }

            

            printf("vec sortie %d \n", vec_sortie[temp]);
            temp+=1;

    }
    
    
    
  }
  printf("fin de GOL \n ");
  free(mat_entree);
}





int main(int argc, char** argv){
  MPI_Init(&argc, &argv);
  int n=32;
  int m=32;
  
  int root=0;//processus central qui envoie et récupère toutes les données 
  int *recbuf;
  int size, rank;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  printf("nombre de processus %d \n", size);
  int ** matrix;
  if (rank==root){
    matrix = create_image_int(n,m);
    
    for (int i=0; i<n; i++){
      for (int j=0; j<m; j++){
        matrix[i][j]=0;

      }
        
    }
    int ind=n/2-1;
    matrix[ind][ind]=1;
    matrix[ind][ind+1]=1;
    matrix[ind+1][ind]=1;
    matrix[ind+1][ind+1]=1;
    matrix[ind+2][ind+2]=1;
    matrix[ind+2][ind+3]=1;
    matrix[ind+3][ind+3]=1;
    matrix[ind+3][ind+2]=1;
    matrix_t image0;

    image0.n=n; 
    image0.m=m;
    image0.data=matrix;  
    save_png("./video/essaivideo_cellular_oye_0.png",GREY, &image0);
  }

  int nprocs=size;
  int nb_coeff=n/nprocs*m;

  int nb_ite=200;
  //1) vectorisation de la matrice et envoides données nécessaires aux voisins gauche et droite de chaque processus
  int * sendbuf;

  for (int ite =0; ite<nb_ite; ite++){
    printf("************************new ite*************\n");
    if (ite==0){
      if (rank==0){
        sendbuf=matrix[0];
      }
      recbuf=malloc(nb_coeff*sizeof(int ));//buffer de réception (à créer pour chaque processus)

      printf("\n fin du scatter");
      MPI_Scatter(sendbuf, nb_coeff, MPI_INT, recbuf, nb_coeff, MPI_INT, root, MPI_COMM_WORLD);
      }

      //ring pour faire des échanges (chaque processus envoi sa preéière ligne au precent et sa dernière au suivant 
      int prec=(nprocs+rank-1)%nprocs; //numéro du rank suivant
      
      int succ=(rank+1)%nprocs;//numéro du rank précédent

      int * proc_last_to_next_send=&recbuf[(n/nprocs-1)*m];
      //malloc(m*sizeof(int)); 
      int * proc_first_to_previous_send=&recbuf[0];
      
      //malloc(m*sizeof(int));// on récupère la première ligne pour le processus suivant
      int * tokensucc=malloc(m*sizeof(int)); 
      int * tokenprec=malloc(m*sizeof(int));
      
      if (rank%2==0){
        MPI_Sendrecv(proc_last_to_next_send, m, MPI_INT, succ, 0, tokensucc, m, MPI_INT, succ, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
       
        MPI_Sendrecv(proc_first_to_previous_send, m, MPI_INT,prec, 0,tokenprec, m, MPI_INT,prec, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
      }

      else {
        MPI_Sendrecv(proc_first_to_previous_send, m, MPI_INT,prec, 0,tokenprec, m, MPI_INT,prec, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
        MPI_Sendrecv(proc_last_to_next_send, m, MPI_INT, succ, 0, tokensucc, m, MPI_INT, succ, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
        
      }
      
      int * sendbufint;
      int * concat;
      int * sendbuf;
      if (ite%2==0){
        if (ite==0){  
          concat=malloc (m*(2+n/nprocs)*sizeof(int));
          sendbuf=malloc (m*(2+n/nprocs)*sizeof(int));
        }
        
        printf(" fin \n "); 
        concatenate(concat, tokenprec,recbuf,tokensucc, n/nprocs, m, ite);
        printf("**************************************************************************");
        
        game_of_life(concat, sendbuf, m, n/nprocs);
        printf("^^^^^^^^^^^^^^^^^^^^^^^^^^ ");
        
        sendbufint=&sendbuf[m];
        }
      

      else {
        printf("concactenate ok");
        concatenate(sendbuf, tokenprec,recbuf,tokensucc, n/nprocs, m, ite);
        printf("concactenate ok");
        game_of_life(sendbuf, concat, m, n/nprocs);
        

        printf("GOL ok");
        sendbufint=&concat[m];
      }
      
      
      printf("fin \n");
      
      
      
      
      
      int * recbufint;
      if (root==0){
        recbufint=malloc(n*m*sizeof(int ));
        
      }
      
      printf("on est ici 1 \n");
      MPI_Gather(sendbufint, nb_coeff, MPI_INT, recbufint, 
        nb_coeff, MPI_INT, root, MPI_COMM_WORLD);
      
      //envoyer à chaque processus les données
      
      printf("on est ici \n");
      
      
      if (rank==root){
        printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% \n ");
        for (int i=0; i<(m)*n; i++){
          printf("-- %d ", recbufint[i] );
        }
        int **mat=reform_image(recbufint,n, m);
        matrix_t matrix;
        matrix.n=n;
        matrix.m=m;
        matrix.data=mat;
        char buffer[50];
        sprintf(buffer, "./video/essaivideo_cellular_oye_%d.png", ite+1);   
        save_png(buffer,GREY, &matrix);
        sendbuf=recbufint;
      }
        
    
  }
  if (rank==root){
    free(sendbuf);
  }
  MPI_Finalize();  
}


