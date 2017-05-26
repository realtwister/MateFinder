#include "DFS.h"
#ifdef DEBUG
#define concat(first, second) first second
#define LOG(fmt,...)    \
  printf(concat("LOG: ",fmt), ##__VA_ARGS__)
#else
#define LOG(x...)
#endif

/**
 * Calculate what the best outcome is for a single node.
 * First of all, it is checked whether the current position is a mate or a draw. If this is the case, this is reported to the calling function.
 * Next, all the moves that are possible in the position are investigated one by one.
 * For every move, it is returned to this function whether it results in a win, loss, draw, or an undecided position, and how many moves it takes.
 * This function, then, chooses the best option among all of these moves, and returns that to the calling function.
 * In turbo mode, at odd depths of search (so when the opponent is to move), not necessarily the best move is returned, but just a move that avoids being checkmated, whenever possible.
 * @param[in] board The board to consider
 * @param[in] depth The current depth
 * @return This function returns a DFSresult object containing the best state possible of the previous node the depth at which the state occurred and the moves leading to that state from this node.
 */
template<bool T>
DFSresult DFS::best_outcome(Board board, unsigned int depth){
  //check is current board is mate and return the state if true
  if(board.isMate()){
    return {2, depth, std::stack<move>()};
  }
  // check the current board is draw and return the state if true.
  if(board.isDraw()) return {0,depth, std::stack<move>()};
  // check if the current maximal depth is reached and return the state if true
  if(depth >= this->curDepth) return {1, depth, std::stack<move>()};

  // create the initial best result (-3 is not a real state but its worse than anything else.)
  DFSresult best = {-3, 0, std::stack<move>()};
  DFSresult res;
  // get all the moves of the current board and loop over them.
  moveArray moves = board.getMoves();
  for(int i = 0; i < moves.size(); i++){
    if(T)
      if(this->curDepth == this->maxDepth)
        std::cout << "Progress: branch " << (i+1) << "/" << moves.size() << "." << std::endl;
    // get the result of the children boards due to the current move
    res = best_outcome<false>(board.cloneAndExecMove(moves[i]), depth+1);
    // if the resulting state is better than the current best state change the best state.
    if(res.state > best.state){
      best=res;
      best.moves.push(moves[i]);
      if(this->turbo && depth%2==1 && best.state > -2)
	      break;
    }
    // else if the states are equal check if we can have a faster(slower) enemy mate (ally mate or draw).
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
    // if we are at the root node and we found a forced mate change the search depth to the worst case depth of the forced mate.
    if(depth == 0 && best.state == 2 && best.depth < this-> curDepth){
      LOG("change curDepth %d \n", best.depth);
      this-> curDepth = best.depth;
    }
  }
  // flip the mate state resulting from the children nodes to portray the mate state of this node.
  if(best.state == 2){
    best.state = -2;
  }
  else if(best.state == -2){
    best.state = 2;
  }
  return best;
}

/**
 * DFS or depth first search constructor.
 * @param[in] _start Starting board pointer
 * @param[in] _maxDepth Maximal depth to search.
 * @param[in] _turbo Turbo mode.
 */
DFS::DFS(Board* _start, unsigned int _maxDepth, bool _turbo)
{
  start = _start;
  curDepth = 1;
  maxDepth = _maxDepth;
  turbo = _turbo;
}

/**
 * Do the actual search. The search is conducted as follows. First of all, all the possible moves are investigated with depth 1.
 * If this does not yield a decisive result, then the depth at which is searched is increased by 2. This is done until the maximum depth is reached.
 * Increasing the depth like this does not increase the total time complexity of the program,
 * and is therefore perfectly suitable for obtaining preliminary results without having to conduct the entire search, while not giving rise to unacceptable amounts of overhead.
 * Only at the maximum depth level, progress reports are being shown.
 * @return This function returns a DFSresult object containing the best state possible from the start board the worstcase depth at which the state occurred and the moves leading to that state from the position.
 */
DFSresult DFS::search(){
  //set initial curDepth
  this->curDepth = 1;
  DFSresult res = {1,0,std::stack<move>()};
  //keep looking while there is no definitive answer and while the maximum depth is not reached.
  while(res.state == 1 && this->curDepth <= this->maxDepth)
  {
    //search on odd depths as these are the depths where enemy mates occur.
    res = this->best_outcome<true>(*(this->start), 0);
    this->curDepth += 2;
    if (this->curDepth > this->maxDepth && this->curDepth < this->maxDepth + 2) {this->curDepth = this->maxDepth;}
  }
  return res;
}
