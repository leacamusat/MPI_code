//gcc -Wall fractal.c test_lea.c lea_png.c libattopng.c -o fractal -lm


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "create_vector.h"


int mustExit=0;

void exchange(int i, vector_t v){
    int temp=v.vec[i];
    v.vec[i]=v.vec[i+1];
    v.vec[i+1]=temp;
}




void sort_pair_impair(vector_t vec1){
    do {
        mustExit=1;
        for (int i=0; i<vec1.n-1; i+=2){
            if (vec1.vec[i]>vec1.vec[i+1]) {
                exchange(i, vec1);
                mustExit=0;
                }
                }
        for (int i=1; i<vec1.n-1; i+=2){
            if (vec1.vec[i]>vec1.vec[i+1]) {
                exchange(i, vec1);
                mustExit=0;
                }
                }
    } while( mustExit == 0 ); 
}



int main()
{
    vector_t vec=create_vector(9);
    vec=fill(vec, 15);
    print_vec(vec);
    sort_pair_impair(vec);
    printf("\n post sort \n");
    print_vec(vec);
    printf("\n");
    return 0;
    
}
