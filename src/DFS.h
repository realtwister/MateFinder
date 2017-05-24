#ifndef DFS_LIBRARY_DEFINED
#define DFS_LIBRARY_DEFINED
#include "Board.h"
#include <stack>

struct DFSresult
{
  int state;
  unsigned int depth;
  std::stack<move> moves;
};

class DFS{
  #ifdef DEBUG
public:
  #endif
  unsigned int maxDepth;
  unsigned int curDepth;
  Board* start;

  DFSresult best_outcome(Board, unsigned int);

public:
  //constructor
  DFS(Board* _start, unsigned int _maxDepth);
  DFS(Board* _start): DFS(_start, 10){}

  int search();
};
#endif
