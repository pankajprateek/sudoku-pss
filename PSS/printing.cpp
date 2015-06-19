#include<iostream>
#include<stdio.h>
#include "model.h"

using namespace std;

typedef unsigned short int num;
void printpuzzle(num puzzle[][N*N])
{
  int i,j;
  printf("\nPrinting the puzzle\n\n");
  for(i=0;i<N*N;i++)
  {
    for(j=0;j<N*N;j++)
    {
      printf("%d\t", puzzle[i][j]);	//We fill the puzzle with numbers 0,1,2....N*N-1
    }
    printf("\n");
  }
  printf("\n");
}

void printmatrix(cell matrix[][N*N]){
  int i,j;
  printf("\nPrinting the matrix\n");
  for(i=0; i<N*N; i++){
    for(j=0; j<N*N; j++)
      printf("%d\t", matrix[i][j].value);
    printf("\n");
  }
}

void printmatrixwithdomains(cell matrix[][N*N]){
  int i,j,k;
  printf("\nPrinting the matrix\n");
  for(i=0; i<N*N; i++){
    for(j=0; j<N*N; j++){
      printf("%d|", matrix[i][j].value);
      for(k=0; k<N*N; k++)
	printf("%d", matrix[i][j].domain[k]);
      printf("\t");
    }
    printf("\n");
  }
}