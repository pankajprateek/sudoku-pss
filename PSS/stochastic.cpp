#include<iostream>
#include "model.h"
#include<ctime>
#include<queue>
#include<stack>
#include<cstdlib>
#include<stdio.h>
using namespace std;

void printmatrix(cell matrix[][N*N]);
void printmatrixwithdomains(cell matrix[][N*N]);
void filterdomains(int x, int y, num value, cell matrix[][N*N]);
queue<point> Q;	//queue to hold the cells to be repaired
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
  num values[N*N], conflicts[N*N], domain=0;
  for(i=0;i<N*N;i++)		//conflicts[i] is the amount of conflict associated with val=values[i]
				//domain is the no of elements in values[] & conflicts[]
  {
    if(matrix[row][col].domain[i]==1){
      values[domain]=i;
      conflicts[domain]=evalinitialconflicts(row, col, i, matrix);
      domain++;
    }

  }
  if(domain==0 || matrix[row][col].given==1) return matrix[row][col].value;
  unsigned int min=conflicts[0];	//min is the least conflict encountered for this cell
  bool min_index[N*N];		//min_index[i]==true means that i is among the values with minimumm conflicts
  for(i=0;i<domain;i++)
    if(conflicts[i]<min) min=conflicts[i];
  
  unsigned int count=0;		//count is the number of values with minimum conflict
  //printf("Minimum Conflicting Values for (%d,%d): ",row,col);
  for(i=0;i<domain;i++)
  {
    if(conflicts[i]==min){
      //printf("%d ",values[i]);
      min_index[i]=1;
      count++;
    }
    else
      min_index[i]=0;
  }
  //printf("\n");
  
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


void initialfill(cell matrix[][N*N]){
  num nunfilledrowcells[N*N], toFill=0, nunfilledrows=0;
  num i,j;
  bool rowUnfilled=0;
  for(i=0; i<N*N; i++){
    nunfilledrowcells[i]=0;
    rowUnfilled=0;
    for(j=0; j<N*N; j++){
      if(matrix[i][j].given==0){
	cellsfilled[i][j]=0;
	nunfilledrowcells[i]++;
	toFill++;
	rowUnfilled=1;
      }
      else cellsfilled[i][j]=1;
    }
    if(rowUnfilled==1) nunfilledrows++;
  }
  num randomrow, count=0, selectedrow, randomcolumn, selectedcolumn;
  while(toFill>0){
    randomrow=rand()%nunfilledrows;
    count=0;
    for(i=0; i<N*N; i++){
      if(nunfilledrowcells[i]>0){
	if(count==randomrow){
	  selectedrow=i;
	  break;
	}
	count++;
    }
    }
    randomcolumn=rand()%nunfilledrowcells[selectedrow];
    count=0;
    for(i=0; i<N*N; i++){
      if(cellsfilled[selectedrow][i]==0){
	if(count==randomcolumn){
	  selectedcolumn=i;
	  break;
	}
	count++;
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
    matrix[selectedrow][selectedcolumn].queued=1;	//We mark (i,j) cell as queued	
  }
}



long long int rwt[N*N][N*N][N*N][N*N];
long long int cwt[N*N][N*N][N*N][N*N];
long long int bwt[N*N][N*N][N*N][N*N];
//*wt[n][i][j][v] is the weight of constraint between ith and jth cell in nth {row, column, block} for the value v
unsigned const int printIntervals=10;
unsigned int iterations=0;
unsigned int evalConflicts(int row,int col, int val, cell matrix[][N*N]){	//returns sum of conflicts for (x,y) cell
  int i,j;
  unsigned int conflicts=0;
  for(i=0;i<N*N;i++)
  {
    if((matrix[row][i].value==val) && (i!=col))
      conflicts+=rwt[row][i<col ?i:col][i>col ?i:col][val];
    if((matrix[i][col].value==val) && (i!=row))
      conflicts+=cwt[col][i<row ?i:row][i>row ?i:row][val];
  }
  for(i=((int)(row/N))*N;i<((int)(row/N))*N+N;i++)
  {
    for(j=((int)(col/N))*N;j<(int)(col/N)*N+N;j++)
    {
      if((val==matrix[i][j].value) && !((i==row) && (j==col)))
      {
	int blockno=((int)(i/N))*N+(int)(j/N);
	int ijno=((int)(i%N))*N+(int)(j%N);
	int rowcolno=((int)(row%N))*N+(int)(col%N);
	conflicts+=bwt[blockno][ijno<rowcolno ?ijno:rowcolno][ijno>rowcolno?ijno:rowcolno][val];
      }
    }
  }
  return conflicts;
}

unsigned int minconflictingvalue(int row,int col,cluster matrix[][N*N])
//Returns least-conflicting value (other than the current one) of (row,col) cell
{
  num i;
  unsigned int values[N*N], conflicts[N*N], domain=0;
  for(i=0;i<N*N;i++)		//conflicts[i] is the amount of conflict associated with val=values[i]
				//domain is the no of elements in values[] & conflicts[]
  {
    if(matrix[row][col].domain[i]==1 && i!=matrix[row][col].value){
      values[domain]=i;
      conflicts[domain]=evalConflicts(row, col, i, matrix);
      domain++;
    }

  }
  if(domain==0 || matrix[row][col].given==1) return matrix[row][col].value;
  unsigned int min=conflicts[0];	//min is the least conflict encountered for this cell
  bool min_index[N*N];		//min_index[i]==true means that i is among the values with minimumm conflicts
  for(i=0;i<domain;i++)
    if(conflicts[i]<min) min=conflicts[i];
  
  unsigned int count=0;		//count is the number of values with minimum conflict
  //printf("Minimum Conflicting Values for (%d,%d): ",row,col);
  for(i=0;i<domain;i++)
  {
    if(conflicts[i]==min){
      //printf("%d ",values[i]);
      min_index[i]=1;
      count++;
    }
    else
      min_index[i]=0;
  }
  //printf("\n");
  
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
////////////////////////////////////////////////////////////////////////
/*---------------------------STOCHASTIC-------------------------------*/
////////////////////////////////////////////////////////////////////////
void stochastic(cell matrix[][N*N],num puzzle[N*N][N*N])
{
  num i,j,k,l;
  for(i=0;i<N*N;i++)
    for(j=0;j<N*N;j++)
      for(k=0;k<N*N;k++)
	for(l=0;l<N*N;l++)
	  rwt[i][j][k][l]=cwt[i][j][k][l]=bwt[i][j][k][l]=1;	//Initialise all the weights to 1

  srand((unsigned)time(0));
  
  bool changed=true;
  int nDomain=0; //No of values in the domain
  while(changed){
    changed=false;
    for(i=0;i<N*N;i++)
    {
      for(j=0;j<N*N;j++)
      {
	if(matrix[i][j].given==0){
	  nDomain=0;
	  for(k=0;k<N*N;k++)
	    if(matrix[i][j].domain[k]==1)
	      nDomain++;
	  if(nDomain==1){
	    for(k=0;k<N*N;k++)
	      if(matrix[i][j].domain[k]==1)
	      {
		matrix[i][j].domain[k]=0;
		matrix[i][j].value=k;
		matrix[i][j].given=1;
		filterdomains(i,j,k,matrix);
		changed=1;
		break;
	      }
	  }
	}
      }
    }
  }
  
  initialfill(matrix);
  iterations=0;
  //char ch;
  while(!Q.empty()){
    //scanf("%c", &ch);
    iterations++;
    //printmatrix(matrix);
    //if(iterations%printIntervals==0){
      //printf("Iterations:%d\n", iterations);
      //printmatrix(matrix);
    //}
    qentry head=Q.front();	//Take up the front cell
    Q.pop();
    unsigned int row=head.x, col=head.y;
//     if(matrix[row][col].queued==0) printf("Something's wrong!\n");
    matrix[row][col].queued=0;			//Unmark the head as queued
    //printf("Queue popped at: %d %d Value: %d\n", row, col, matrix[row][col].value);


    //printf("%d", matrix[row][col].queued);
    unsigned int conflicts=evalConflicts(row,col,matrix[row][col].value,matrix);	//Evaluate its conflicts
    //printf("Conflicts: %d\n", conflicts);
    if(conflicts>0)
    {
      
      //printf("Minimum Conflicting Value:%d\n",min);
      num prevValue = matrix[row][col].value;
      num min;
      if(matrix[row][col].given==0){
	min=minconflictingvalue(row,col,matrix);	//Calculate the minimum conflicting value
	matrix[row][col].value=min;				//Assign the cell its minimum conflicting value
      }
      else min=prevValue;
      for(i=0;i<N*N;i++)
      {
	if((matrix[row][i].value==min) && (i!=col) /*&& (matrix[row][i].given==0)*/ /*&& (matrix[row][i].queued==0)*/){
	  qentry toBeQueued;
	  toBeQueued.x=row;
	  toBeQueued.y=i;
	  Q.push(toBeQueued);				//queue the same-row cells having values equal to this minimum value
	  matrix[row][i].queued=1;
	  //printf("Queued:(%d,%d) ",row,i);
	}
	if((matrix[i][col].value==min) && (i!=row) /*&& (matrix[i][col].given==0)*/ /*&& (matrix[i][col].queued==0)*/){
	  qentry toBeQueued;
	  toBeQueued.x=i;
	  toBeQueued.y=col;
	  Q.push(toBeQueued);				//queue the same-column cells having values equal to this minimum value
	  matrix[i][col].queued=1;
	  //printf("(%d,%d) ",i,col);
	}
	
	//we increment the weight of the constraint (corresponding to the previous value of this cell)
	if((matrix[row][i].value==prevValue) && (i!=col)) rwt[row][i<col?i:col][i>col?i:col][prevValue]++;	//for same-row cells
	if((matrix[i][col].value==prevValue) && (i!=row)) cwt[col][i<row?i:row][i>row?i:row][prevValue]++;	//for same-column cells
	   
      }
      for(i=((int)(row/N))*N;i<((int)(row/N))*N+N;i++)
      {
	for(j=((int)(col/N))*N;j<((int)(col/N))*N+N;j++)
	{
	  if((min==matrix[i][j].value) && !((i==row) && (j==col)) /*&& (matrix[i][j].given==0) *//*&& (matrix[i][j].queued==0)*/)
	  {
	    qentry toBeQueued;
	    toBeQueued.x=i;
	    toBeQueued.y=j;
	    Q.push(toBeQueued);				//queue the same-block cells having values equal to this minimum value
	    matrix[i][j].queued=1;
	    //printf("(%d,%d)\n",i,j);
	  }
	  if((matrix[i][j].value==prevValue) && !((i==row)&&(j==col))){
	    int blockno=((int)(i/N))*N+(int)(j/N);
	    int ijno=((int)(i%N))*N+(int)(j%N);
	    int rowcolno=((int)(row%N))*N+(int)(col%N);
	    bwt[blockno][ijno<rowcolno? ijno: rowcolno][ijno>rowcolno?ijno:rowcolno][prevValue]++;
	  }
	}
      }
    }
  }
  printf("%d\n", iterations);
}