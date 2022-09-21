#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lea_png.h"
#include "create_vector.h"


//procédure qui permet de copier une partie d'un vecteur origvect de longueur len à partir
//de l'indice orig copié à la position dest du destvec
void copy(int orig, int dest, int len, vector_t destvec, vector_t origvect){
    for (int u=0; u<len; u++){
        destvec.vec[dest+u]=origvect.vec[orig+u];   
    } 
}

//fonction qui permet d'extraire un vecteur de taille len à partir de l'indice n
vector_t copy_f(int n, int len, vector_t v){
    vector_t fin=create_vector(len);
    int j=0;
    for (int u=n; u<len+n; u++){
        fin.vec[j]=v.vec[u];
        j+=1;
    }
    return fin;   
}

//entrée : deux tableaux triés A et B
//sortie : un tableau trié qui contient exactement les éléments des tableaux A et B
vector_t fusion(vector_t vec1, vector_t vec2){
    int length= vec1.n+vec2.n;
    printf("fusion \n");
    print_vec(vec1);
    print_vec(vec2);
    if (vec1.n==0) {
        printf("vec1 \n");
        return vec2;
    }
    if (vec2.n==0) {
        printf("vec2 \n");
        return vec1;
    }
    
    vector_t vec=create_vector(length);
    if (vec1.vec[0]<vec2.vec[0]) {
        printf("if \n");
            
        vec.vec[0]=vec1.vec[0];
        copy(0, 1,  length, vec,fusion(copy_f(1, vec1.n-1, vec1), vec2));
        
    } else {
         printf("else \n");
         vec.vec[0]=vec2.vec[0];
         
         copy(0,1,  length, vec,fusion(vec1, copy_f(1, vec2.n-1, vec2)));
    }
    
    print_vec(vec); 
    return vec;
}


//entrée : un tableau T
//sortie : une permutation triée de T
vector_t tri_fusion(vector_t vec){
     print_vec(vec);
     if (vec.n>1){
         int cut=vec.n/2;
         printf("boucle %d, %d  \n", cut, vec.n-cut );
         vector_t vec2=copy_f(0, cut, vec);
         vector_t vec3=copy_f(cut, vec.n-cut, vec);
         return fusion(tri_fusion(vec2), tri_fusion( vec3));
         }    
      else {
         return vec;
      }
}



int main()
{
    vector_t vec=create_vector(15);
    vector_t vec2=create_vector(5);
    vec=fill(vec, 15); //remplissage aléatoire du vecteur avec des éléments de 0 à 15
    vector_t vec3 = tri_fusion(vec);
    printf("\n post sort \n");
    print_vec(vec3);
    printf("\n");
    return 0;
    
}
