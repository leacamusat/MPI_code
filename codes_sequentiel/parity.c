//gcc -Wall fractal.c test_lea.c lea_png.c libattopng.c -o fractal -lm


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lea_png.h"
#include "create_matrix.h"

void cellulaire(matrix_t mat_entree, matrix_t mat_sortie){
    int n=mat_entree.n;
    int m=mat_entree.m;
    int jleft;
    int ileft;
    int iright;
    int jright;
    
    for (int i=0; i<n; i++){
   
        ileft=(n+i-1)%n;
        iright=(n+i+1)%n;
   	
        for (int j=0;j<m; j++){
        
            
            jleft=(m+j-1)%m;
            jright=(m+j+1)%m;
            
            /*
            int tl = ..., tc =..., tr = ...,
            	cl = ..., cr = ...,
            	bl = ..., bc = ..., br = ...;
            int value = tl ^ tc ^ tr ^ cl ^ cr ^ bl ^ bc ^ br;*/
            
            
            int value =(mat_entree.data[i][jleft]+mat_entree.data[i][jright]+mat_entree.data[ileft][jright]+mat_entree.data[ileft][j]+mat_entree.data[ileft][jleft]+mat_entree.data[iright][j]+mat_entree.data[iright][jright]+mat_entree.data[iright][jleft])%2;//la parité de tous les voisins
            mat_sortie.data[i][j]=value;     
        } 
    }
    
    
    
}

void game_of_life(matrix_t mat_entree, matrix_t mat_sortie){
    int n=mat_entree.n;
    int m=mat_entree.m;
    int jleft;
    int ileft;
    int iright;
    int jright;
    
    for (int i=0; i<n; i++){
   
        ileft=(n+i-1)%n;
        iright=(n+i+1)%n;
   	
        for (int j=0;j<m; j++){
        
            
            jleft=(m+j-1)%m;
            jright=(m+j+1)%m;
            
            /*
            int tl = ..., tc =..., tr = ...,
            	cl = ..., cr = ...,
            	bl = ..., bc = ..., br = ...;
            int value = tl ^ tc ^ tr ^ cl ^ cr ^ bl ^ bc ^ br;*/
            
            
            int value =(mat_entree.data[i][jleft]+mat_entree.data[i][jright]+mat_entree.data[ileft][jright]+mat_entree.data[ileft][j]+mat_entree.data[ileft][jleft]+mat_entree.data[iright][j]+mat_entree.data[iright][jright]+mat_entree.data[iright][jleft]);//la parité de tous les voisins
            
            
            printf("%d", value);
            if (value<2 || (value>3 && (mat_entree.data[i][j]==1))){
               
            	mat_sortie.data[i][j]=0;     
        } 
            if (value==2||(value>3 && mat_entree.data[i][j]==0)) {
                printf("Ok");
            	mat_sortie.data[i][j]=1; 
            }
    }
    
    
    
}
}


int main()
{
    
    matrix_t mat=create_matrix(100,100);
     for (int i=0;i<=55; i++){
        for (int j=0;j<=55; j++){
    	    mat.data[i][j]=1;
    	    }
    	    }
    
    
    //affichage_matrix_t(mat);
    save_png("./video/matpost_game_0.png",GREY, &mat);
    
    char buffer[50];
    
    int m=100;
    /*matrix_t mat=create_matrix(100,100);
    matrix_t matpost;
    for (int j=1;j<=m; j++){
        sprintf(buffer, "./video/matpost_%d.png", j);
        if j%2=0{
            save_png(buffer,GREY, &cellulaire(mat, matpost));
            
	else :
            
    	    save_png(buffer,GREY, &cellulaire(matpost, mat));
    	}*/
    	
     
    matrix_t matpost=create_matrix(100,100);
    for (int j=1;j<=m; j++){
    	
        sprintf(buffer, "./video/matpost_game_%d.png", j);
        if (j%2==1){
            game_of_life(mat, matpost);
            save_png(buffer,GREY, &matpost);
            }
            
	else {
            game_of_life(matpost, mat);
    	    save_png(buffer,GREY, &mat );
    	    }
    }
    free_mat(mat);
    free_mat(matpost);
    return 0;
}
