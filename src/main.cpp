#include <ctime>
#include <iostream>
#include "Board.h"
#define innerN 100
#define outerN 10000000

static square knightMoves[8]={
  {1,2},
  {-1,2},
  {2,1},
  {2,-1},
  {1,-2},
  {-1,-2},
  {-2,1},
  {-2,-1}
};

unsigned char knightLoop(){
  signed char newx,newy;
  for(int i=0; i<innerN; i++)
    for (signed char hordir = -1; hordir <= 1; hordir += 2)
      for (signed char verdir = -1; verdir <= 1; verdir += 2)
        for (signed char absx = 1; absx <= 2; absx++)
        {
          newy = verdir * (3 - absx);
          newx = hordir * absx;
        }
  return newx;
}

unsigned char knightArray(){
  square mv;
  for(int i=0; i<innerN; i++)
  for(int i=0; i<8; i++){
    mv= knightMoves[i];
  }
  return mv.x;
}



int main(int argc, char const *argv[]) {
  std::clock_t start;
  double durloop=0;
  double durarray=0;
  unsigned char x;
  signed char newx,newy;
  square mv;
  for(int i =0; i<outerN; i++){

    start = std::clock();
    for(int i=0; i<innerN; i++)
      for (signed char hordir = -1; hordir <= 1; hordir += 2)
        for (signed char verdir = -1; verdir <= 1; verdir += 2)
          for (signed char absx = 1; absx <= 2; absx++)
          {
            newy = verdir * (3 - absx);
            newx = hordir * absx;
          }
    durloop+= ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    if(i%10000000==0) std::cout<<newx;
      start = std::clock();
      for(int i=0; i<innerN; i++)
      for(int i=0; i<8; i++){
        mv= knightMoves[i];
      }
    durarray+= ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    if(i%10000000==0) std::cout<<mv.x;
  }

  std::cout<<"loop: "<< durloop<<" array:"<<durarray<<'\n';
}
