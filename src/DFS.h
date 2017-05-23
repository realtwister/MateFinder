#ifndef DFS_LIBRARY_DEFINED
#define DFS_LIBRARY_DEFINED
#include <stack>
#include "Framework.cpp"
enum NODE_STATE:char{
  none=0,
  allyWin=1,
  undecided=2,
  draw=3,
  enemyWin=4
};

enum NODETYPES{
  ENEMY,
  ALLY,
};

template<enum NODETYPES>
struct nodeState;

template<>
struct nodeState<ENEMY>{
  NODE_STATE state;
  unsigned int depth;

  bool operator > (const nodeState<ENEMY> &other)
  {
    if(other.state == none){
      return true;
    }
      if(state == other.state){
        if(state != enemyWin){
          return depth < other.depth;
        }
        return depth > other.depth;
      }

      switch(state){
        case allyWin:
          return true;
        case enemyWin:
          return false;
        case undecided:
          return other.state == enemyWin || other.state == draw;
        case draw:
          return other.state == enemyWin;
        default:
        return false;
      }
  }
};

template<>
struct nodeState<ALLY>{
  NODE_STATE state;
  unsigned int depth;
  bool operator > (const nodeState<ALLY> &other)
  {
    if(other.state == none){
      return true;
    }
    if(state == other.state){
      if(state != allyWin){
        return depth < other.depth;
      }
      return depth > other.depth;
    }

    switch(state){
      case allyWin:
        return false;
      case enemyWin:
        return true;
      case undecided:
        return other.state == allyWin || other.state == draw ;
      case draw:
        return other.state == allyWin;
      default:
      return false;
    }
  }
};


class DFS{
  #ifdef DEBUG
public:
  #endif
  unsigned int maxDepth;
  std::vector<move> moves;
  Board* start;
  std::stack<move> curMoves;

  nodeState<ALLY> allyMove(Board& curBoard, unsigned int depth, unsigned int maxLen);
  nodeState<ENEMY> enemyMove(Board& curBoard, unsigned int depth, unsigned int maxLen);
public:
  //constructor
  DFS(Board* _start, unsigned int _maxDepth);
  DFS(Board* _start): DFS(_start, 10){}

  //getter
  //int getLen(){return maxLen;}
  std::vector<move> getMoves(){return moves;}
  int getMaxDepth(){return maxDepth;}

  //setters
  void setMaxDepth(int _maxDepth){maxDepth=_maxDepth;}
  void setStart(Board* _start){start = _start;}

  //calculate
  int findMate(); //find the mate
};
#endif
