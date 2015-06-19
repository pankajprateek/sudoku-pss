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
queue<point> Q, BigQ;	//queue to hold the cells to be repaired
unsigned int evalConflicts(int row, int col, int val, cell matrix[][N*N]);
unsigned int evalinitialConflicts(int row, int col, int val, cell matrix[][N*N]);
unsigned int mininitialconflictingvalue(int row,int col,cell matrix[][N*N]);
unsigned int minconflictingvalue(int row,int col,cluster matrix[][N*N]);
void initialqueueing(cell matrix[][N*N]);
long long int rwt[N*N][N*N][N*N][N*N];
long long int cwt[N*N][N*N][N*N][N*N];
long long int bwt[N*N][N*N][N*N][N*N];
//*wt[n][i][j][v] is the weight of constraint between ith and jth cell in nth {row, column, block} for the value v
unsigned const int printIntervals=10;
unsigned int iterations=0;
bool cellsfilled[N*N][N*N];

void stochastic(cell matrix[][N*N],num puzzle[N*N][N*N])
{
  num i,j,k,l;
    for(i=0;i<N*N;i++)
    for(j=0;j<N*N;j++)
      for(k=0;k<N*N;k++)
	for(l=0;l<N*N;l++)
	  rwt[i][j][k][l]=cwt[i][j][k][l]=bwt[i][j][k][l]=1;	//Initialise all the weights to 1

  srand((unsigned)time(0));
  initialqueueing(matrix);
  iterations=0;
  while(!BigQ.empty()){
    qentry ihead=BigQ.front();
    BigQ.pop();
    num irow=ihead.x, icol=ihead.y,i,j;
    
    num ival = matrix[irow][icol].value=mininitialconflictingvalue(irow,icol,matrix);
    //printf("Big Q popped at (%d,%d)=%d\n", irow, icol, ival);
//     if(matrix[irow][icol].given==1) continue;
    matrix[irow][icol].assigned=true;
    for(i=0; i<N*N; i++){
      if(matrix[irow][i].value==ival && matrix[irow][i].assigned==true && i!=icol){
	qentry toBeQueued;
	toBeQueued.x=irow;
	toBeQueued.y=i;
	Q.push(toBeQueued);
	matrix[irow][i].queued=true;
      }
      if(matrix[i][icol].value==ival && matrix[i][icol].assigned==true && i!=irow){
	qentry toBeQueued;
	toBeQueued.x=i;
	toBeQueued.y=icol;
	Q.push(toBeQueued);
	matrix[i][icol].queued=true;
      }
    }
    for(i=((int)(irow/N))*N;i<((int)(irow/N))*N+N;i++){
      for(j=((int)(icol/N))*N;j<((int)(icol/N))*N+N;j++){
	if(matrix[i][j].value==ival && matrix[i][j].assigned==true && !(i==irow && j==icol)){
	  qentry toBeQueued;
	  toBeQueued.x=i;
	  toBeQueued.y=j;
	  Q.push(toBeQueued);
	  matrix[i][j].queued=true;
	  }
      }
    }
    Q.push(ihead);
    while(!Q.empty()){
      iterations++;
      //if(iterations%printIntervals==0){
	//printf("Iterations:%d\n", iterations);
	//printmatrix(matrix);
      //}
      qentry head=Q.front();	//Take up the front cell
      Q.pop();
      unsigned int row=head.x, col=head.y;
      if(matrix[row][col].assigned==0) printf("Something's wrong\n");
      matrix[row][col].queued=0;			//Unmark the head as queued
      //printf("\nQueue popped at: %d %d Value: %d Assigned:%d Given:%d\n", row, col, matrix[row][col].value, matrix[row][col].assigned, matrix[row][col].given);
      if(matrix[row][col].given==1){
	//printf("The cell was \"given\"\n");
	continue;
      }     
      unsigned int conflicts=evalConflicts(row,col,matrix[row][col].value,matrix);	//Evaluate its conflicts
      //printf("Conflicts: %d\n", conflicts);
      if(conflicts>0)
      {
	num prevValue = matrix[row][col].value;
	num min=minconflictingvalue(row,col,matrix);	//Calculate the minimum conflicting value
	matrix[row][col].value=min;				//Assign the cell its minimum conflicting values
	//printf("Minimum Conflicting Value:%d\n",min);	  
	for(i=0;i<N*N;i++)
	{
	  if((matrix[row][i].value==min) && (i!=col)/* && (matrix[row][i].given==0) */&& matrix[row][i].assigned==true && (matrix[row][i].queued==0)){
	    qentry toBeQueued;
	    toBeQueued.x=row;
	    toBeQueued.y=i;
	    Q.push(toBeQueued);			//queue the same-row cells having values equal to this minimum value
	    matrix[row][i].queued=1;
	    //printf("Same-row Queued:(%d,%d) ",row,i);
	  }
	  if((matrix[i][col].value==min) && (i!=row)/* && (matrix[i][col].given==0) */&& matrix[i][col].assigned==true  && (matrix[i][col].queued==0)){
	    qentry toBeQueued;
	    toBeQueued.x=i;
	    toBeQueued.y=col;
	    Q.push(toBeQueued);				//queue the same-column cells having values equal to this minimum value
	    matrix[i][col].queued=1;
	    //printf("Same-cloumn queued:(%d,%d) ",i,col);
	  }
	  
	  //we increment the weight of the constraint (corresponding to the previous value of this cell)
	  if((matrix[row][i].value==prevValue) && (i!=col) && matrix[row][i].assigned==1) rwt[row][i<col?i:col][i>col?i:col][prevValue]++;	//for same-row cells
	  if((matrix[i][col].value==prevValue) && (i!=row) && matrix[i][col].assigned==1) cwt[col][i<row?i:row][i>row?i:row][prevValue]++;	//for same-column cells
	    
	}
	for(i=((int)(row/N))*N;i<((int)(row/N))*N+N;i++)
	{
	  for(j=((int)(col/N))*N;j<((int)(col/N))*N+N;j++)
	  {
	    if((min==matrix[i][j].value) && !((i==row) && (j==col)) /*&& (matrix[i][j].given==0)*/ && matrix[i][j].assigned==true  && (matrix[i][j].queued==0))
	    {
	      qentry toBeQueued;
	      toBeQueued.x=i;
	      toBeQueued.y=j;
	      Q.push(toBeQueued);				//queue the same-block cells having values equal to this minimum value
	      matrix[i][j].queued=1;
	      //printf("Same-block queued:(%d,%d)\n",i,j);
	    }
	    if((matrix[i][j].value==prevValue) && !((i==row)&&(j==col)) && matrix[i][j].assigned==1){
	      int blockno=((int)(i/N))*N+(int)(j/N);
	      int ijno=((int)(i%N))*N+(int)(j%N);
	      int rowcolno=((int)(row%N))*N+(int)(col%N);
	      bwt[blockno][ijno<rowcolno? ijno: rowcolno][ijno>rowcolno?ijno:rowcolno][prevValue]++;
	    }
	  }
	}
      }
    }
  }
  printmatrix(matrix);
  printf("%d\n", iterations);
  
}

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
  if(matrix[row][col].given==1) return matrix[row][col].value;
  for(i=0;i<N*N;i++)		//conflicts[i] is the amount of conflict associated with val=values[i]
				//domain is the no of elements in values[] & conflicts[]
  {
    if(matrix[row][col].domain[i]==1 /*&& i!=matrix[row][col].value*/){
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


void initialqueueing(cell matrix[][N*N]){
  num nUnqueuedRowCells[N*N], toQueue=0, nUnqueuedRows=0;
  num i,j;
  bool rowUnqueued=0;
  for(i=0; i<N*N; i++){
    nUnqueuedRowCells[i]=0;
    rowUnqueued=0;
    for(j=0; j<N*N; j++){
      if(matrix[i][j].given==0){
	nUnqueuedRowCells[i]++;
	toQueue++;
	rowUnqueued=1;
	cellsfilled[i][j]=0;
      }
      else cellsfilled[i][j]=1;
      matrix[i][j].queued=false;
      matrix[i][j].assigned=false;
    }
    if(rowUnqueued==1) nUnqueuedRows++;
  }
  //printf("To big Queue: %d\n", toQueue);
  num randomrow, count=0, selectedrow, randomcolumn, selectedcolumn;
  while(toQueue>0){
    randomrow=rand()%nUnqueuedRows;
    count=0;
    for(i=0; i<N*N; i++){
      if(nUnqueuedRowCells[i]>0){
	if(count==randomrow){
	  selectedrow=i;
	  break;
	}
	count++;
    }
    }
    randomcolumn=rand()%nUnqueuedRowCells[selectedrow];
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
//     matrix[selectedrow][selectedcolumn].assigned=true;
    cellsfilled[selectedrow][selectedcolumn]=1;
    nUnqueuedRowCells[selectedrow]--;
    if(nUnqueuedRowCells[selectedrow]==0) nUnqueuedRows--;
    toQueue--;
    qentry toBeQueued;
    toBeQueued.x=selectedrow;
    toBeQueued.y=selectedcolumn;
    BigQ.push(toBeQueued);
  }
}

unsigned int evalConflicts(int row,int col, int val, cell matrix[][N*N]){	//returns sum of conflicts for (x,y) cell
  int i,j;
  unsigned int conflicts=0;
  for(i=0;i<N*N;i++)
  {
    if((matrix[row][i].value==val) && (i!=col) && matrix[row][i].assigned==1)
      conflicts+=rwt[row][i<col ?i:col][i>col ?i:col][val];
    if((matrix[i][col].value==val) && (i!=row) && matrix[i][col].assigned==1)
      conflicts+=cwt[col][i<row ?i:row][i>row ?i:row][val];
  }
  for(i=((int)(row/N))*N;i<((int)(row/N))*N+N;i++)
  {
    for(j=((int)(col/N))*N;j<(int)(col/N)*N+N;j++)
    {
      if((val==matrix[i][j].value) && !((i==row) && (j==col)) && matrix[i][j].assigned==1)
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
  if(matrix[row][col].given==1){
    //printf("Given cell found for (%d,%d) in minconflictingvalue\n", row,col);
    return matrix[row][col].value;
  }
  for(i=0;i<N*N;i++)		//conflicts[i] is the amount of conflict associated with val=values[i]
				//domain is the no of elements in values[] & conflicts[]
  {
    if(matrix[row][col].domain[i]==1 && i!=matrix[row][col].value){
      values[domain]=i;
      conflicts[domain]=evalConflicts(row, col, i, matrix);
      domain++;
    }

  }
  if(domain==0){
    return matrix[row][col].value;
    //printf("Domain=0 for (%d,%d) in minconflictingvalue\n", row,col);
  }
  unsigned int min=conflicts[0];	//min is the least conflict encountered for this cell
  bool min_index[N*N];		//min_index[i]==true means that i is among the values with minimumm conflicts
  for(i=0;i<domain;i++)
    if(conflicts[i]<min) min=conflicts[i];
  
  unsigned int count=0;		//count is the number of values with minimum conflict
  //printf("(Values,Conflicts) ");
  //for(i=0; i<domain; i++) printf("(%d,%d) ", values[i], conflicts[i]);
  //printf("Minimum Conflicting Values for (%d,%d): ",row,col);
  //printf("\n");
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