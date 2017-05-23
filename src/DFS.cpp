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
  // TODO: Check for  curDepth;
  if(depth >= this->curDepth) return {1, depth, std::stack<move>()};

  DFSresult best = {-3, 0, std::stack<move>()};
  DFSresult res;
  for(int i = 0; i  < moves.size(); i++){
    res = best_outcome(board.cloneAndExecMove(moves[i]), depth+1);
    if(res.state > best.state){
      best=res;
      best.moves.push(moves[i]);
      if(depth%2==1 && best.state > -2){
        break;
      }
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
    if(depth == 0 && best.state == 2 && best.depth < this-> curDepth){
      LOG("change curDepth %d \n", best.depth);
      this-> curDepth = best.depth;
    }
  }
  if(best.state == 2){
    best.state = -2;
  }
  else if(best.state == -2){
    best.state = 2;
  }
  return best;
}

DFS::DFS(Board _start, unsigned int _curDepth){
  start = _start;
  curDepth = _curDepth;
}

int DFS::search(){
  this->curDepth = 2;
  DFSresult res = {1,0,std::stack<move>()};
  while(res.state == 1 && this->curDepth < this->maxDepth){
    this->curDepth -=1;
    this->curDepth *=2;
    this->curDepth +=1;
    std::cout<< "curDepth: "<< this->curDepth <<std::endl;
    res = this->best_outcome(this->start, 0);
  }
  printf("state: %d in %u \n", res.state, res.depth);
  bool blackToMove = this->start.blackToMove();
  while(!res.moves.empty()){
    std::cout << (blackToMove? "black: " : "white: ");
    ((move) res.moves.top()).printMove(blackToMove);
    res.moves.pop();
    blackToMove = !blackToMove;
  }
  return res.state;
}
