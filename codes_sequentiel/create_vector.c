/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "create_vector.h"

//fonction d'affichage d'un vecteur
void print_vec(vector_t v1){

    printf("(");
    for (int i=0; i<v1.n-1; i++){
        printf("%d  ", v1.vec[i]);
    }
    printf("%d", v1.vec[v1.n-1]);
    printf(") \n");
}
    
vector_t create_vector(int size){
    vector_t v;
    v.vec=malloc(size*sizeof(int)) ; // allocation mémoire pour stocker size int 
    v.n=size;
    for (int i=0; i<size; i++){
        //*(v+i)=0;
        v.vec[i]=0;
    }
    return v;
}

//fonction de remplissage automatique d'uun vecteur avec des valeurs générées aléatoirement 
vector_t fill(vector_t v1, int max){
    for (int i=0; i<v1.n; i++){
        
        v1.vec[i]=(rand()%max+1);
    }
    return v1;
}

//fonction de désallocation mémoire 
void destroy_vector(vector_t v){
    free(v.vec);
}

//fonction somme de deux vecteurs 
vector_t sum_vectors(vector_t v1, vector_t v2){
    vector_t v3=create_vector(v1.n);
    
    for (int i=0; i<v1.n; i++){
        v3.vec[i]=v1.vec[i]+v2.vec[i];
            
    }
   
    return v3;
}

//fonction produit scalaire  
int dot_vector(vector_t v1, vector_t v2){
    int product=0;
    
    for (int i=0; i<v1.n; i++){
        product+=v1.vec[i]*v2.vec[i];
            
    }
    
    
    return product;
}

//multicplication par un scalaire 
vector_t multiplication_scalar(vector_t v1,int scalar){
    
    
    vector_t v3=create_vector(v1.n);
    for (int i=0; i<v1.n; i++){
        v3.vec[i]=v1.vec[i]*scalar;
            
    }
    
    
    return v3;
}

//norme 1 d'un vecteur 
int norm_vector(vector_t v1){
    int norm1=0;
    for (int i=0; i<v1.n; i++){
        norm1+=abs(v1.vec[i]);
    }
    
    return norm1;
}


    

/*int main()
{
    int TAILLE=5;
    vector_t a=create_vector(TAILLE);
    vector_t b=create_vector(TAILLE);
    
    b=fill(b, 9);
    a=fill(a, 5);
    vector_t c;
    
    int MULT=2;
    c=multiplication_scalar(a, MULT);
    printf("\n La résultat de la multiplication de ");
    print_vec(a);
    printf(" par %d vaut :  ", MULT);
    print_vec(c);
    
    int norme=norm_vector(a);
    printf("\n La norme 1 de ");
    print_vec(a);
    printf("  vaut %d.", norme);
    printf("\n Le produit scalaire de  ");
    print_vec(a);
    printf(" * ");
    print_vec(b);
    printf("  vaut %d.", dot_vector(a, b));
    
    
    
    
    
    //soustraction de a par b 
    vector_t d;
    printf(" \n La différence ");
    print_vec(a);
    printf(" - ");
    print_vec(b);
    printf(" vaut ");
    d=sum_vectors(a, multiplication_scalar(b, -1));
    print_vec(d);
    destroy_vector(a);
    destroy_vector(b);
    destroy_vector(c);
    destroy_vector(d);
    return 0;
}*/

