/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "lea_png.h"
#include "create_matrix.h"



matrix_t create_matrix(int rows, int columns){
    matrix_t mat;
    mat.data=malloc(rows*sizeof(int*)) ; // allocation mémoire pour stocker size adress
    mat.n=rows;
    mat.m=columns;
    int* vec_tot=malloc(columns*rows*sizeof(int));
    
    for (int i=0; i<rows; i++){
        mat.data[i]=vec_tot+i*columns;
        for (int j=0; j<columns; j++){
            mat.data[i][j]=0;
        }
    }
        
        
    
    
    return mat;
}


//fonction de remplissage automatique d'uun vecteur avec des valeurs générées aléatoirement 
matrix_t fill(matrix_t m, int max){
    int rows=m.n;
    int columns=m.m;
    for (int i=0; i<rows; i++){
        for (int j=0; j<columns; j++){
            m.data[i][j]=(rand()%max+1);;
        }
    }
    return m;
}

void free_mat(matrix_t mat){
    free(mat.data[0]);
    free(mat.data);
}
    

/*int main()
{
    int TAILLE=5;
    matrix_t tab=create_matrix(TAILLE, TAILLE);
    
    
    
    
    return 0;
}*/
