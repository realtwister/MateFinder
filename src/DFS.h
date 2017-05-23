#ifndef DFS_LIBRARY_DEFINED
#define DFS_LIBRARY_DEFINED
#include "Board.h"

class DFS{
  #ifdef DEBUG
public:
  #endif
  unsigned int maxDepth;
  Board start;



public:
  //constructor
  DFS(Board _start, unsigned int _maxDepth);
  DFS(Board _start): DFS(_start, 10){}

  int search();
};
#endif
