
#include "libattopng.h"
#include "lea_png.h"




    
matrix_t create_matrix(int rows, int columns); 

//fonction de remplissage automatique d'uun vecteur avec des valeurs générées aléatoirement 
matrix_t fill(matrix_t m, int max);

void free_mat(matrix_t mat);
