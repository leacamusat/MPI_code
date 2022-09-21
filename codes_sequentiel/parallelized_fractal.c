#include <mpi.h> 
#include <stdio.h>
#include <complex.h>
#include <stdlib.h>
#include <math.h>
#include "create_matrix.h"
#include "lea_png.h"

int M=2;

//création d'une matrice de rows lignes et columns colonnes
double _Complex** create_image_complex(int rows, int columns){
    double _Complex** mat;
    mat=malloc(rows*sizeof(double _Complex*)) ; // allocation mémoire pour stocker size adress
    double _Complex* vec_tot=malloc(columns*rows*sizeof(double _Complex));
    
    for (int i=0; i<rows; i++){
        mat[i]=vec_tot+i*columns;
        for (int j=0; j<columns; j++){
            mat[i][j]=0.0+0.0*_Complex_I;

        }
    }
    return mat;
}


//création d'une matrice de rows lignes et columns colonnes
int** create_image_int(int rows, int columns){
    int** mat;
    mat=malloc(rows*sizeof(int*)) ; // allocation mémoire pour stocker size adress
    
    int* vec_tot=malloc(columns*rows*sizeof(int));
    
    for (int i=0; i<rows; i++){
        mat[i]=vec_tot+i*columns;
        for (int j=0; j<columns; j++){
            mat[i][j]=0;
        }
    }
    return mat;
}

//création de la fonction  (équivalent) : donne un vecteur de n valeurs répartis de manière homogène entre strat et end
double* linspace(int start, int end,int n ){
    printf("%ld \n", n* sizeof(double));
    double *lin = malloc(n* sizeof(double));
    printf("hola \n");
    printf("%p %d \n", lin, n);
    double step = (end - start) / (double)(n - 1);

     for (int i = 0; i < n; i++) {
         lin[i] = start + ((double)i * step);
         
     }
    
    return lin;
} 



//calcul du nombre d'itérations pour sortir de l'orbite 
int nbiter(double _Complex z,double _Complex c){
    int iter=0;
    while (cabs(z)<=M && iter<5000){
        
        iter+=1;
        double _Complex temp =z * z + c;
        z=temp;
        
    	//printf("module %f", module_complex(z));
    }
    return iter;
}


//remplissage de la matrice avec le nombre de mailles n désiré
int* maillage(double _Complex c, double _Complex* vec, int nb_coeff, int n ){
    
    int * iterate_vector= malloc(nb_coeff* sizeof(int));
    printf("malloc %p", iterate_vector);
    for (int i=0; i< nb_coeff; i++){
            int value=(int)log2(1+nbiter(vec[i],c));
            printf(" orbite %d \n", (int)log2(1+nbiter(vec[i],c)) );
            printf(" iteration %d \n", i); 
            iterate_vector[i]=value;
        }

    printf("returning by maillage \n");
    return iterate_vector;
}


//remplissage de la matrice avec le nombre de mailles n désiré
double _Complex** maillage_ini(int start, int end,int n, int m){
    double* linn=linspace(start, end, n);
    printf("%p \n", linn);
    double* linm=linspace(start, end, m);
    double _Complex** mat=create_image_complex(n/2,m);
    printf("%p \n ", mat);
    for (int i=0; i<n/2; i++){
        double re=linn[i];
        for (int j=0;j<m; j++){
            double _Complex z;
            
            double im=linm[j];
            z=re+im*_Complex_I;
            mat[i][j]=z;
            printf("%d %d \n", i, j);
        } 
    }
    
    free(linn);
    free(linm);
    return mat;
}

//reshape de l'image d'origine et symétrie
int** reform_image(int * vec,int nb_rows,  int nb_columns){
    int** mat =create_image_int( nb_rows, nb_columns);
    int index=0;
    for (int i=0; i<nb_rows/2; i++){
        for (int j=0;j<nb_columns; j++){
            index+=1;
            mat[i][j]=vec[index];
            mat[nb_rows-i-1][nb_columns-j-1]=vec[index];//symétrie
        } 
    }
    
    
    return mat;
}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  double THETA;
  int rang;
  int niter=1000;
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rang);
  printf("rxdtzutcgvhbtr \n zvghjbrtzgvhj %d \n", size);

  

  
  int n=256;
  int m=256;
  int nprocs=size;
  printf("%d", nprocs);
  int nb_coeff=(n/nprocs)*m;
  int root=0;//processus central qui envoie et récupère toutes les données
  double _Complex * sendbuf;
  double * vec=linspace(0, 2*3.14,niter);
  int k=0;
  for (int k=0; k<niter; k++){
      
    THETA=vec[k];  
    
    double _Complex C= 0.7885*cos(THETA) + 0.7885*sin(THETA)*_Complex_I;
  /*1) envoi de la matrixe maillée qui contient 
  des nombres complexes en distribué*/
    printf("a \n");
    double _Complex * recbuf=malloc(nb_coeff/2*sizeof(double _Complex ));//buffer de réception (éà créer pour chaque processus)
    printf("hola 1 \n");
    int start=-2;
    int end=2;
    printf("b \n");
    
    if (rang==root){
      printf(" BONJOUR \n");
      double _Complex ** mat=maillage_ini(start, end,n, m);
      printf(" root \n");
      sendbuf=malloc(nprocs*nb_coeff/2*sizeof(double _Complex ));//buffer d'envoi (à créer pour le root)
      printf("hola 2 \n");
      int k=0;
      for (int i=0; i<n/2; i++) {
        for (int j=0; j<m; j++){
          sendbuf[k]=mat[i][j];//remplissage du buffer d'envoi
          k+=1;
        }
      }
    }

    MPI_Scatter(sendbuf, nb_coeff/2, MPI_C_DOUBLE_COMPLEX, recbuf, nb_coeff/2, MPI_C_DOUBLE_COMPLEX, root, MPI_COMM_WORLD);
    //envoyer à chaque processus les données
    printf(" scatter \n");
    /*2) calcul du nombre d'itérations pour sortir de l'orbite
    par chaque processeur et envoi au root du vecteur total */
    
   
    int * sendbufint=maillage(C, recbuf,nb_coeff/2,nb_coeff/2 );
    printf("hola1 \n");
    int * recbufint;
    recbufint=malloc(nprocs*(nb_coeff/2)*sizeof(int ));
    printf("hola 3 \n");
    if (rang==0){
      printf("root, je ne rentre pas la \n");
      //recbufint=malloc(nprocs*nb_coeff/2*sizeof(int ));
    }


    MPI_Gather(sendbufint, nb_coeff/2, MPI_INT, recbufint, 
      nb_coeff/2, MPI_INT, root, MPI_COMM_WORLD);
    
    free (recbuf);
    printf("%d rang %d size \n", rang, size);
    if (rang==root){
      free(sendbuf);
      int **mat=reform_image(recbufint,n, m);
      matrix_t matrix;
      matrix.n=n;
      matrix.m=m;
      matrix.data=mat;
      char buffer[50];
      sprintf(buffer, "./video/essaivideo_fractal_%d.png", k);   
      save_png(buffer,RGB, &matrix);
      free(recbufint);

      }
    free(sendbufint);
  }
  free(vec);
  printf("size %d \n", size);
  MPI_Finalize();
}