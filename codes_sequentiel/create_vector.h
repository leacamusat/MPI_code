/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

//structure avec vecteur d'entier et taille
typedef struct vector {
    int n;
    int* vec;
} vector_t;

//fonction d'affichage d'un vecteur
void print_vec(vector_t v1);
    
vector_t create_vector(int size);

//fonction de remplissage automatique d'uun vecteur avec des valeurs générées aléatoirement 
vector_t fill(vector_t v1, int max);

//fonction de désallocation mémoire 
void destroy_vector(vector_t v);


//fonction somme de deux vecteurs 
vector_t sum_vectors(vector_t v1, vector_t v2);


//fonction produit scalaire  
int dot_vector(vector_t v1, vector_t v2);

//multicplication par un scalaire 
vector_t multiplication_scalar(vector_t v1,int scalar);

//norme 1 d'un vecteur 
int norm_vector(vector_t v1);


    

