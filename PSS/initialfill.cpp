#include<iostream>
#include "model.h"
#include<ctime>
#include<queue>
#include<cstdlib>
#include<stdio.h>
using namespace std;



bool cellsfilled[N*N][N*N];

unsigned int evalinitialconflicts(int row,int col, int val, cell matrix[][N*N])	//returns sum of conflicts for (x,y) cell
{
  int i,j;
  unsigned int conflicts=0;
  for(i=0;i<N*N;i++)
  {
    if((cellsfilled[row][i]==1) && (matrix[row][i].value==val) && (i!=col))
      conflicts++;
    if((cellsfilled[i][col]==1) && (matrix[i][col].value==val) && (i!=row))
      conflicts++;
  }
  for(i=((int)(row/N))*N;i<((int)(row/N))*N+N;i++)
    for(j=((int)(col/N))*N;j<(int)(col/N)*N+N;j++)
      if((cellsfilled[i][j]==1) && (val==matrix[i][j].value) && !((i==row) && (j==col)))
	conflicts++;
  return conflicts;
}

unsigned int mininitialconflictingvalue(int row,int col,cell matrix[][N*N])
//Returns least-conflicting value (other than the current one) of (row,col) cell
{
  num i;
  unsigned int values[N*N], conflicts[N*N], domain=0;
  for(i=0;i<N*N;i++)		//conflicts[i] is the amount of conflict associated with val=values[i]
				//domain is the no of elements in values[] & conflicts[]
  {
    if(matrix[row][col].domain[i]==1 && i!=matrix[row][col].value){
      values[domain]=i;
      conflicts[domain]=evalinitialconflicts(row, col, i, matrix);
      domain++;
    }

  }
  if(domain==0) return matrix[row][col].value;
  unsigned int min=conflicts[0];	//min is the least conflict encountered for this cell
  bool min_index[N*N];		//min_index[i]==true means that i is among the values with minimumm conflicts
  for(i=0;i<domain;i++)
    if(conflicts[i]<min) min=conflicts[i];
  
  unsigned int count=0;		//count is the number of values with minimum conflict
  printf("Minimum Conflicting Values for (%d,%d): ",row,col);
  for(i=0;i<domain;i++)
  {
    if(conflicts[i]==min){
      printf("%d ",values[i]);
      min_index[i]=1;
      count++;
    }
    else
      min_index[i]=0;
  }
  printf("\n");
  
  int choice=rand()%count;	//we randomly choose amongst the least-conflicting value
  int c=0;
  for(i=0;i<domain;i++)
  {
    
    if(min_index[i]==1){
      if(c==choice)		//We return the "choice"th value, where choice is a random no. between 0 and count
      return values[i];
      c+=1;
    }
  }
}


//void initialfill(cell matrix[][N*N], queue<point> Q)
template< typename T > void initialfill( cell matrix[][N*N],T &Q )
{
  
  unsigned int nunfilledrowcells[N*N], toFill=0, nunfilledrows=0;
  unsigned int i,j;
  bool flag=0;
  for(i=0; i<N*N; i++){
    nunfilledrowcells[i]=0;
    flag=0;
    for(j=0; j<N*N; j++){
      if(matrix[i][j].given==0){
	cellsfilled[i][j]=0;
	nunfilledrowcells[i]++;
	toFill++;
	flag=1;
      }
      else cellsfilled[i][j]=1;
    }
    if(flag==1) nunfilledrows++;
  }
  unsigned int randomrow, count=0, selectedrow, randomcolumn, selectedcolumn;
  while(toFill>0){
    randomrow=rand()%nunfilledrows;
    count=0;
    for(i=0; i<N*N; i++){
      if(nunfilledrowcells[i]>0){
	count++;
	if(count==randomrow){
	  selectedrow=i;
	  break;
      }
    }
    }
    randomcolumn=rand()%nunfilledrowcells[selectedrow];
    count=0;
    for(i=0; i<N*N; i++){
      if(cellsfilled[selectedrow][i]==0){
	count++;
	if(count==randomcolumn){
	  selectedcolumn=i;
	  break;
	}
      }
    }
    matrix[selectedrow][selectedcolumn].value=mininitialconflictingvalue(selectedrow, selectedcolumn, matrix);
    cellsfilled[selectedrow][selectedcolumn]=1;
    nunfilledrowcells[selectedrow]--;
    if(nunfilledrowcells[selectedrow]==0) nunfilledrows--;
    toFill--;
    qentry toBeQueued;
    toBeQueued.x=selectedrow;
    toBeQueued.y=selectedcolumn;
    Q.push(toBeQueued);
    matrix[i][j].queued=1;	//We mark (i,j) cell as queued	
  }
  unsigned int j;
  for(i=0; i<N*N; i++){
    for(j=0; j<N*N; j++)
      printf("%d\t", matrix[i][j].value);
    printf("\n");
  }
}
  