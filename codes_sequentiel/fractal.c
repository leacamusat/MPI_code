/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/
//gcc -Wall fractal.c test_lea.c lea_png.c libattopng.c -o fractal -lm


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lea_png.h"
#include "create_matrix.h"
#include <complex.h>

int M=2;

//création d'une matrice de rows lignes et columns colonnes
matrix_t create_matrix_t(int rows, int columns){
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

//création de la fonction  (équivalent) : donne un vecteur de n valeurs répartis de manière homogène entre strat et end
double* linspace(int start, int end,int n ){
    double *lin = malloc(n* sizeof(double));

    double step = (end - start) / (double)(n - 1);

     for (int i = 0; i < n; i++) {
         lin[i] = start + ((double)i * step);
         
     }
    
    return lin;
} 



//calcul du nombre d'itérations pour sortir de l'orbite 
int nbiter(double _Complex z,double _Complex c){
    int iter=0;
    //printf("%f \n", cabs(z));
    while (cabs(z)<=M && iter<8000){
        
        iter+=1;
        double _Complex temp =z * z + c;
        z=temp;
        
    	//printf("module %f", module_complex(z));
    }
    return iter;
}


//remplissage de la matrice avec le nombre de mailles n désiré
matrix_t maillage(int start, int end,int n, int m,  double _Complex c ){
    double* linn=linspace(start, end, n);
    double* linm=linspace(start, end, m);
    matrix_t mat=create_matrix_t(n,m);
    for (int i=0; i<(int)(n/2); i++){
        for (int j=0;j<m; j++){
            double _Complex z;
            double re=linn[i];
            double im=linm[j];
            z=re+im*_Complex_I;
            int value=(int)log2(1+nbiter(z,c));
            mat.data[i][j]=value;
            mat.data[n-i-1][m-j-1]=value;
        } 
    }
    
    free(linn);
    free(linm);
    return mat;
}


//remplissage de la matrice avec le nombre de mailles n désiré
/*matrix_t maillage(int start, int end,int n, int m,  complex_t c ){
    double* lin=linspace(start, end, n);
    matrix_t mat=create_matrix_t(n,m);
    
    for (int i=0; i<n; i++){
        for (int j=i; j<m; j++){
            complex_t z;
            z.re=lin[i];
            z.im=lin[j];
            int value=(int)log2(1+nbiter(z,c));
            mat.data[i][j]=value;
            mat.data[n-i-1][n-j-1]=value;
        }
    }
    
    
    return mat;
}*/

//affichage de la matrice 
void affichage_matrix_t(matrix_t mat){
    int rows=mat.n;
    int columns=mat.m;
    
    for (int i=0; i<rows; i++){
        
        for (int j=0; j<columns; j++){
            
            printf("%d  ", mat.data[i][j]);
        }
        printf("\n");
    }

}



char buffer[50];
        
int main()
{
    double THETA=0.0;
    
    matrix_t mat;
    int n=25;
    //0.7885
    for (int i=0; i<n; i++){
    	double * vec=linspace(0, 2*3.14,n);
    	THETA=vec[i];
    	
        double _Complex C= 0.7885*cos(THETA) + 0.7885*sin(THETA)*_Complex_I;
    	//C.re=0.285+(0.01*i);
    	//C.im=0.01+(0.01*i);
        sprintf(buffer, "./video/essaivideo_fractal_%d.png", i);   
    	mat=maillage(-2,2,1056,1056 , C);
    //affichage_matrix_t(mat);
    	save_png(buffer,RGB, &mat);
    	}

    free_mat(mat);
    return 0;
}

