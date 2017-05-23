#include "DFS.h"
#define DEBUG
#ifdef DEBUG
#define concat(first, second) first second
#define LOG(fmt,...)    \
  printf(concat("LOG: ",fmt), ##__VA_ARGS__)
#else
#define LOG(x...)
#endif

DFS::DFS(Board _start, unsigned int _maxDepth){
  start = _start;
  maxDepth = _maxDepth;
}

int DFS::search(){
  return 1;
}
