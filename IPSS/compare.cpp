#include<iostream>
#include<stdio.h>
#include "model.h"
using namespace std;

bool compare(num puzzle[][N*N], cell matrix[][N*N])
{
  int i,j,k,flag,row,col;
  num array[N*N];
  //Checking if the already-given cells haven't been changed
  for(i=0; i<N*N; i++)
    for(j=0; j<N*N; j++)
      if(puzzle[i][j]<N*N && matrix[i][j].value!=puzzle[i][j]) return 0;
  // Comparing along Rows
  for(i=0;i<N*N;i++)
  {
    for(j=0;j<N*N;j++)
      array[j]=0;
    for(j=0;j<N*N;j++)
    {
      array[matrix[i][j].value]++;
    }
    flag=1;
    for(j=0;j<N*N;j++)
    {
      if(array[j]!=1)
	flag=0;
    }
    if(flag==0)
      return 0;
  }
  // Comparing along Columns
  for(i=0;i<N*N;i++)
  {
    for(j=0;j<N*N;j++)
      array[j]=0;
    for(j=0;j<N*N;j++)
    {
      array[matrix[i][j].value]++;
    }
    flag=1;
    for(j=0;j<N*N;j++)
    {
      if(array[j]!=1)
	flag=0;
    }
    if(flag==0)
      return 0;
  }
  // Comparing along blocks
  for(row=0;row<N*N;row+=N)
  {
    for(col=0;col<N*N;col+=N)
    {
      for(k=0;k<N*N;k++)
	array[k]=0;
      for(i=row;i<row+N;i++)
      {
	for(j=col;j<col+N;j++)
	{
	  array[matrix[i][j].value]++;
	}
      }
      flag=1;
      for(j=0;j<N*N;j++)
      {
	if(array[j]!=1)
	  flag=0;
      }
      if(flag==0)
	return 0;
    }
  }
  return 1;
}