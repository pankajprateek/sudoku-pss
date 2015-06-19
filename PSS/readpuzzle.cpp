#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include "model.h"
#include<string.h>
using namespace std;

void readpuzzle(num puzzle[][N*N],int count)	//Reads puzzle files from the command line
{
  unsigned int i,j;
  unsigned int tmp;
  char* order = (char*)malloc(sizeof(char)*2);
  sprintf(order, "%d", N);
  char* puzzleno = (char*)malloc(sizeof(char)*6);
  sprintf(puzzleno,"%d", count);
  char * toBeRead = (char*)malloc(sizeof(char)*40);
  //Order 2 puzzles are in "../puzzles/sudoku2/" and numbered 0 to 9999
  //Order 3 puzzles are in "../puzzles/sudoku3/" and numbered 0 to 199
  strcpy(toBeRead, "../puzzles/sudoku");
  strcat(toBeRead, order);
  strcat(toBeRead, "/");
  strcat(toBeRead, puzzleno);
  printf("Opened %s\n", toBeRead);
  FILE* fp = fopen(toBeRead, "r");
  for(i=0;i<N*N;i++)
  {
    for(j=0;j<N*N;j++)
    {
      fscanf(fp,"%d",&tmp);
      if(tmp!=0){
	puzzle[i][j]=tmp-1;	//We fill the puzzle with numbers from 0,1,2.....,N*N-1
	//printf("puzzle[%d][%d]=%d\n",i,j,puzzle[i][j]);
      }
      else
	puzzle[i][j]=N*N;
    }
  }
  fclose(fp);
}