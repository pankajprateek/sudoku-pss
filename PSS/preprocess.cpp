#include<iostream>
#include<stdio.h>
#include "model.h"
using namespace std;

void filterdomains(int x, int y, num value, cell matrix[][N*N])
//Removes "value" from the domains of same-row, same-column and same-block cells of (x,y) cell
{
  int i,j;
  for(i=0;i<N*N;i++)
  {
    
    if(i!=y) matrix[x][i].domain[value]=0;	//Removes values from the domains of same-row cells
    if(i!=x) matrix[i][y].domain[value]=0;	//Removes values from the domains of same-column cells
  }
  int x0=((int)(x/N))*N, y0=((int)(y/N))*N; //(x0,y0) is the top left cell of the block containing (x,y)
  for(i=x0; i<x0+N; i++)			//Removes values from the domains of the same-block cells
    for(j=y0; j<y0+N; j++)
      if(i!=x || j!=y) matrix[i][j].domain[value]=0;
}




void preprocess(num puzzle[][N*N], cell matrix[][N*N])	//Fills the numbers from puzzle onto the matrix
{
  int i,j;
  //printf("Preprocessing\n");
  for(i=0;i<N*N;i++)
  {
    for(j=0;j<N*N;j++)
    {
      if(puzzle[i][j]<N*N)		//If the value of this cell is given
      {
	//printf("Preprocessing for (%d,%d)=%d\n",i,j,puzzle[i][j]);
	matrix[i][j].value=puzzle[i][j];	//Fill the value in the correspoding cell in matrix
	matrix[i][j].given=1;			//Mark that cell as given
	filterdomains(i,j,puzzle[i][j],matrix);	//Remove the values from the cells in the same rox, column and block
      }
    }
  }
}