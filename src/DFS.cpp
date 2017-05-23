#include "DFS.h"
#define DEBUG
#ifdef DEBUG
#define concat(first, second) first second
#define LOG(fmt,...)    \
  printf(concat("LOG: ",fmt), ##__VA_ARGS__)
#else
#define LOG(x...)
#endif
/*nodeState<ALLY> DFS::allyMove(Board& curBoard, unsigned int depth, unsigned int maxLen){
  if(depth >= maxLen || depth >= maxDepth) return {undecided, depth};
  if(curBoard.getMoves().size() <= 0) return {curBoard.isMate()? enemyWin: draw,depth};
  Board childBoard;
  nodeState<ALLY> bestState = {none,0};
  nodeState<ALLY> curState= {none,0};
  for(move curMove: curBoard.getMoves()){
    childBoard = curBoard;
    childBoard.execMove(curMove);
    curState = allyMove(childBoard,depth+1,maxLen);
    if(curState>bestState) bestState=curState;
  }
  std::cout<<curBoard.getMoves().size()<<std::endl;
  return allyMove(childBoard, depth+1, maxLen);
}*/

inline Board* getNext(Board& curBoard, move curMove){
  LOG("Executing move: %c\n", (char)curMove.num);
  /* FOR Framework*/
  Board* childBoard = curMove.board;
  /* FOR BOARD.H
  childBoard = curBoard;
  childBoard.execMove(curMove);*/
  return childBoard;
}

nodeState<ALLY> DFS::allyMove(Board& curBoard, unsigned int depth, unsigned int maxLen){
  if(depth >= maxDepth || depth >= maxLen){
    LOG("Prune by depth \n");
    return {undecided, depth};
  }
  if(curBoard.getMoves().size() == 0){
  LOG("Prune by result \n");
    return { curBoard.isMate() ? enemyWin : draw,depth};
  }
  curBoard.print(0);
  Board *childBoard;
  nodeState<ENEMY> bestState = {none,0};
  nodeState<ENEMY> curState= {none,0};
  depth++;
  LOG("Reached depth: %d\n", depth);
  for(move curMove:curBoard.getMoves()){
    LOG("Ally to move\n");
    childBoard = getNext(curBoard,curMove);
    #ifdef DEBUG
    childBoard->print(0);
    #endif
    curState = enemyMove(*childBoard, depth, maxLen);
    if(curState>bestState){
       bestState=curState;
      if(curState.state == allyWin){
        maxLen = curState.depth;
      }
    }
  }
  return nodeState<ALLY>({bestState.state, bestState.depth});
}

nodeState<ENEMY> DFS::enemyMove(Board& curBoard, unsigned int depth, unsigned int maxLen){
  if(depth >= maxDepth || depth >= maxLen){
    LOG("Prune by depth\n");
    return {undecided, depth};
  }
  if(curBoard.getMoves().size() == 0){
    LOG("Prune by result\n");
    return { curBoard.isMate() ? allyWin : draw,depth};
  }

  Board *childBoard;
  nodeState<ALLY> bestState = {none,0};
  nodeState<ALLY> curState= {none,0};
  for(move curMove:curBoard.getMoves()){
    LOG("Enemy to move\n");
    childBoard = getNext(curBoard,curMove);
    curState = allyMove(*childBoard, depth, maxLen);
    if(curState>bestState){
      bestState=curState;
      if(curState.state > allyWin){
        //std::cout<<"break due to "<< curState.state<< std::endl;
        break;
      }
    }
  }
  return nodeState<ENEMY>({bestState.state, bestState.depth});
}

int DFS::findMate(){
  nodeState<ALLY> result = allyMove(*start, 0, -1);
  switch(result.state){
    case allyWin:
      std::cout<< "Win";
      break;
    case enemyWin:
      std::cout<< "Lose";
      break;
    case undecided:
      std::cout<< "Undecided";
      break;
    case draw:
      std::cout<< "Draw";
      break;
    default:
      std::cout<< "UWOT";
      break;
  }
  return result.depth;
}


DFS::DFS(Board *_start, unsigned int _maxDepth){
  //maxLen = -1;
  start = _start;
  maxDepth = _maxDepth;
  moves = std::vector<move>();
}
