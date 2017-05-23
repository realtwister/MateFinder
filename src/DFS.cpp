#include "DFS.h"
#define DEBUG
#ifdef DEBUG
#define concat(first, second) first second
#define LOG(fmt,...)    \
  printf(concat("LOG: ",fmt), ##__VA_ARGS__)
#else
#define LOG(x...)
#endif

DFSresult DFS:: best_outcome(Board board, unsigned int depth){
  if(board.isMate()){
    return {2, depth, std::stack<move>()};
  }
  moveArray& moves = board.getMoves();
  if(moves.size() == 0) return {0,depth, std::stack<move>()};
  // TODO: Check for  maxDepth;
  if(depth >= this->maxDepth) return {1, depth, std::stack<move>()};

  DFSresult best = {-3, 0, std::stack<move>()};
  DFSresult res;
  for(int i = 0; i  < moves.size(); i++){
    res = best_outcome(board.cloneAndExecMove(moves[i]), depth+1);
    if(res.state > best.state){
      best=res;
      best.moves.push(moves[i]);
    }
    else if(res.state == best.state){
      switch(res.state){
        case 2:
          if(res.depth < best.depth){
            best = res;
            best.moves.push(moves[i]);
          }
          break;
        case -2:
        case 0:
          if(res.depth > best.depth) {
            best = res;
            best.moves.push(moves[i]);
          }
          break;
      }
    }
  }
  if(best.state == 2){
    best.state = -2;
  }
  else if(best.state == -2){
    if(depth % 2 == 1 && best.depth <  this->maxDepth){
      LOG("change maxDepth %d \n", best.depth);
      this-> maxDepth = best.depth;
    }
    best.state = 2;
  }
  return best;
}

DFS::DFS(Board _start, unsigned int _maxDepth){
  start = _start;
  maxDepth = _maxDepth;
}

int DFS::search(){
  DFSresult res = this->best_outcome(this->start, 0);
  printf("state: %d in %u \n", res.state, res.depth);
  return res.state;
}
