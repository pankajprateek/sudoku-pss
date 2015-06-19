#ifndef STRUCTURE_H
#define STRUCTURE_H
unsigned const int N =3;

//We fill the puzzle with numbers 0,1,2....N*N-1


typedef unsigned short int num;

struct cluster{
  bool domain[N*N];      //domain[0]==true means the cell is in the queue; if(domain[i]==true) then i is in the domain of this cell, where 0<=i<N*N
  num value;            //value is the current number assigned to this cell; 0<=value<N*N
  bool given;           //if(given==true), then this cell was already filled up in the original puzzle
  bool queued;		//if(queued==true) means the cell is presently in the queue Q (Refer stochastic.cpp)
};

typedef struct cluster cell;

struct point{		//Serves as the element of the queue Q (Refer stochastic.cpp)
  num x;
  num y;
};

typedef struct point qentry;


#endif
