#include<iostream>
#include "model.h"

using namespace std;
//We fill the puzzle with numbers 0,1,2....N*N-1
void initialise(cell matrix[][N*N])	//Initialises matrix so that given==false and domain[i]==true for all cells
{
  int i,j,k;
  for(i=0;i<N*N;i++)
  {
    for(j=0;j<N*N;j++)
    {
      matrix[i][j].given=0;	//Assume that none of the cell values are given
      matrix[i][j].value=N*N;
      matrix[i][j].queued=0;
      for(k=0;k<N*N;k++)	
      {
	matrix[i][j].domain[k]=1;		//Assume largest possible domain for a start
      }
    }
  }
}