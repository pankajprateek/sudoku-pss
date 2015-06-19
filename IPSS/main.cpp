#include<iostream>
#include<stdio.h>
#include "model.h"
using namespace std;

void initialise(cell matrix[][N*N]);
void readpuzzle(num puzzle[][N*N],int count);
void preprocess(num puzzle[][N*N], cell matrix[][N*N]);
void printpuzzle(num puzzle[][N*N]);
void printmatrix(cell matrix[][N*N]);
void stochastic(cell matrix[][N*N],num puzzle[N*N][N*N]);
void processSingletonDomains(cell matrix[][N*N]);
int compare(num puzzle[][N*N], cell matrix[][N*N]);
void filterdomains(int x, int y, num value, cell matrix[][N*N]);

int main()
{
  num puzzle[N*N][N*N];
  cell matrix[N*N][N*N];

  unsigned int count=0;
  unsigned int nooffiles;
  if(N==4) {
    count=49;
//     while(count<50)
//     {
      initialise(matrix);
      readpuzzle(puzzle, count);    
      preprocess(puzzle, matrix);
      processSingletonDomains(matrix);
      //printpuzzle(puzzle);
      //printmatrixwithdomains(matrix);
      stochastic(matrix,puzzle);		//Solves the puzzle and writes the solution into matrix
      if(compare(puzzle, matrix)) printf("%d SOLVED\n", count);
      else printf("%d NOT SOLVED\n", count);
      return 0;
    }


  if(N==2) nooffiles=10000; else if(N==3) nooffiles=500; //else if(N==4) nooffiles=50;
  FILE *fp=fopen("result","a");		//We append the result to this file named "result"
  while(count<nooffiles)		//Read all puzzles, solve them and check
  {
    
    initialise(matrix);
    readpuzzle(puzzle, count);    
    preprocess(puzzle, matrix);
    processSingletonDomains(matrix);
    //printpuzzle(puzzle);
    
    stochastic(matrix,puzzle);		//Solves the puzzle and writes the solution into matrix
    
    bool flag=compare(puzzle, matrix);	//returns whether the puzzle has been solved correctly
    
    if(flag)		//If solved correctly
    {
      printf("%d SOLVED\n", count);
      printmatrix(matrix);
      fprintf(fp,"%d SOLVED\n", count);
    }
    else		//otherwise
    {
      printf("%d NOT SOLVED\n", count);
      fprintf(fp,"%d NOT SOLVED\n", count);
    }
    count++;
  }
  
}





void processSingletonDomains(cell matrix[][N*N]){
  bool changed=true;
  int nDomain=0; //No of values in the domain
  num i,j,k;
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
}
