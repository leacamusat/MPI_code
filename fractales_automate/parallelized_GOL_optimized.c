#include <mpi.h>
#include <stdio.h>
#include <complex.h>
#include "create_matrix.h"
#include "lea_png.h"
#include <time.h> // time()
#include <stdlib.h>

#define NUM 0

void swap(int *concat, int parity, int chunk, int N, int M, int prev, int next)
{
  if (parity)
  {
    MPI_Send(concat + M * chunk, M * chunk, MPI_INT, prev, 0, MPI_COMM_WORLD);
    
    MPI_Recv(&concat[M * (N - chunk)], M * chunk, MPI_INT, next, 0,
             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("PROCESSUS PAIR %d : envoi de la seconde ligne de la matrice à %d et réception de %d \n", prev+1, prev, next);
    MPI_Send(&concat[M * (N - 2 * chunk)], M * chunk, MPI_INT, next,
             0, MPI_COMM_WORLD);
    
    MPI_Recv(&concat[0], M * chunk, MPI_INT, prev,
             0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("PROCESSUS PAIR %d : envoi avant dernière ligne de la matrice à %d et réception de %d \n", next-1, next, prev);

  }
  else
  {

    
    MPI_Recv(concat + M * (N - chunk), M * chunk, MPI_INT, next, 0,
             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    
    MPI_Send(&concat[M * chunk], M * chunk, MPI_INT, prev, 0, MPI_COMM_WORLD);
    printf("PROCESSUS IMPAIR %d : réception 2nde ligne de la matrice from %d et envoi à %d \n", next-1, next, prev);
    MPI_Recv(&concat[0], M * chunk, MPI_INT, prev,
             0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(&concat[M * (N - 2 * chunk)], M * chunk, MPI_INT, next,
             0, MPI_COMM_WORLD);
    printf("PROCESSUS IMPAIR %d : réception de l'avant dernière ligne de la matrice from %d et envoi à %d \n", next-1, prev, next);
  }
}

// création d'une matrice de rows lignes et columns colonnes
int **create_image_int(int rows, int columns)
{
  //printf("%d, %d, %ld \n", rows, columns, sizeof(int *));
  int **mat = malloc(rows * sizeof(int *)); // allocation mémoire pour stocker size adress

  int *vec_tot = malloc(columns * rows * sizeof(int));
  srand(time(0)); // Randomise seed initialisation
  for (int i = 0; i < rows; i++)
  {
    mat[i] = vec_tot + i * columns;
    for (int j = 0; j < columns; j++)
    {
      mat[i][j] = rand() % 2;
    }
  }
  return mat;
}

// reshape de l'image d'origine et symétrie
int **reform_image(int *vec, int nb_rows, int nb_columns)
{

  int **mat = create_image_int(nb_rows, nb_columns);
  int index = 0;
  for (int i = 0; i < nb_rows; i++)
  {
    for (int j = 0; j < nb_columns; j++)
    {

      mat[i][j] = vec[index];
      //printf("%d vec[index] \n ", vec[index]);
      index += 1;
    }
  }

  return mat;
}

void cellulaire(int *vec_entree, int *vec_sortie, int m, int nb_rows_by_proc)
{

  int jleft;
  int ileft;
  int iright;
  int jright;
  
  int **mat_entree = reform_image(vec_entree, nb_rows_by_proc + 2, m);
  
  int n = nb_rows_by_proc + 2;
  int temp = 0;
  int value;
  
  for (int i = 0; i < n; i++)
  {

    ileft = (n + i - 1) % n;
    iright = (i + 1) % n;
    
    for (int j = 0; j < m; j++)
    {

      jleft = (m + j - 1) % m;
      jright = (j + 1) % m;

      value = (mat_entree[i][jleft] + mat_entree[i][jright] + mat_entree[ileft][jright] + mat_entree[ileft][j] + mat_entree[ileft][jleft] + mat_entree[iright][j] + mat_entree[iright][jright] + mat_entree[iright][jleft]); // la parité de tous les voisins

      vec_sortie[temp] = value % 2;

      
      temp += 1;
    }
  }
  
  free(mat_entree);
}

void game_of_life(int *vec_entree, int *vec_sortie, int m, int nb_rows_by_proc)
{

  int jleft;
  int ileft;
  int iright;
  int jright;
  
  int **mat_entree = reform_image(vec_entree, nb_rows_by_proc + 2, m);
  
  int n = nb_rows_by_proc + 2;
  int temp = 0;
  int value;
  
  for (int i = 0; i < n; i++)
  {

    ileft = (n + i - 1) % n;
    iright = (i + 1) % n;
    
    for (int j = 0; j < m; j++)
    {

      jleft = (m + j - 1) % m;
      jright = (j + 1) % m;

      value = (mat_entree[i][jleft] + mat_entree[i][jright] + mat_entree[ileft][jright] + mat_entree[ileft][j] + mat_entree[ileft][jleft] + mat_entree[iright][j] + mat_entree[iright][jright] + mat_entree[iright][jleft]); // la parité de tous les voisins

      
      if (mat_entree[i][j] == 1)
      {
        if (value == 2 || value == 3)
        {
          vec_sortie[temp] = 1;
        }
        else
        {
          vec_sortie[temp] = 0;
        }
      }
      else
      {
        if (value == 3)
        {
          
          vec_sortie[temp] = 1;
        }
        else
        {
          vec_sortie[temp] = 0;
        }
      }

      
      temp += 1;
    }
  }
  free(mat_entree);
}

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);
  int root = 0; // processus central qui envoie et récupère toutes les données
  int n = 100;
  int m = 100;
  int size, rank;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int **start_mat;
  if (rank == root)
  {
    start_mat = create_image_int(n, m);

    for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < m; j++)
      {
        start_mat[i][j] = 0;
      }
    }
    int ind = n / 2 - 1;
    start_mat[ind][ind] = 1;
    start_mat[ind][ind + 1] = 1;
    start_mat[ind][ind + 2] = 1;
    start_mat[ind-2][ind +1] = 1;
    start_mat[ind-1][ind + 2] = 1;
    matrix_t image0;

    image0.n = n;
    image0.m = m;
    image0.data = start_mat;
    save_png("./video/essaivideo_cellular_oye_0.png", GREY, &image0);
  }

  int nb_coeff = (n / size) * m;

  int nb_ite = 50;
  // 1) vectorisation de la matrice et envoides données nécessaires aux voisins gauche et droite de chaque processus
  int *mat1, *mat2;
  int *recbufint;
  for (int ite = 0; ite < nb_ite; ite++)
  {
    printf("************************new ite*************\n");
    if (ite == 0)
    {
      int *sendbuf = NULL;
      if (rank == 0)
      {
        sendbuf = start_mat[0];
      }
      printf("allocation concat de %d\n", (n / size + 2) * m);
      mat1 = malloc((n / size + 2) * m * sizeof(int)); // buffer de réception (à créer pour chaque processus)

      
      MPI_Scatter(sendbuf, nb_coeff, MPI_INT, mat1 + m, nb_coeff, MPI_INT, root, MPI_COMM_WORLD);
      
      
    }
    
    //  ring pour faire des échanges (chaque processus envoi sa première ligne au précédent et sa dernière au suivant
    int prev = (size + rank - 1) % size; // numéro du rank précédent

    int next = (rank + 1) % size; // numéro du rank suivant

    int *mat_final;

    if (ite % 2 == 0)
    {
      if (ite == 0)
      {
        mat2 = malloc(m * (2 + n / size) * sizeof(int));
      }

      swap(mat1, rank % 2 == 0, 1, (n / size + 2), m, prev, next);

      if (NUM==0){
        game_of_life(mat1, mat2, m, n / size);
      }
      else {
        cellulaire(mat1, mat2, m, n / size);
      }

      mat_final = mat2 + m;
    }

    else
    {
      
      swap(mat2, rank % 2 == 0, 1, (n / size + 2), m, prev, next);
      
      if (NUM==0){
        game_of_life(mat2, mat1, m, n / size);
      }
      else{
        cellulaire(mat2, mat1, m, n / size);
      }

      mat_final = mat1 + m;
    }

   

    
    if (root == 0)
    {
      recbufint = malloc(n * m * sizeof(int));
    }

    printf("Gather de la matrice \n");
    MPI_Gather(mat_final, nb_coeff, MPI_INT, recbufint,
               nb_coeff, MPI_INT, root, MPI_COMM_WORLD);

    // envoyer à chaque processus les données

    

    if (rank == root)
    {
      
      int **mat = reform_image(recbufint, n, m);
      matrix_t matrix;
      matrix.n = n;
      matrix.m = m;
      matrix.data = mat;
      char buffer[50];
      sprintf(buffer, "./video/essaivideo_cellular_oye_%d.png", ite + 1);
      save_png(buffer, GREY, &matrix);
    }
  }
  
  if (rank == root)
  {
    free(mat1);
    free(mat2);
    free(recbufint); 
  }
  
  MPI_Finalize();
}
