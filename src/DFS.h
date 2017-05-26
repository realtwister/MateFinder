#ifndef DFS_LIBRARY_DEFINED
#define DFS_LIBRARY_DEFINED

#include "Board.h"
#include <stack>

/**
 * The DFSresult structure contains the result of the Depth First Search.
 * It stores whether the given position is a win, a loss, a draw, or undecided, and it contains the move sequence the search routine considered best.
 */
struct DFSresult
{
  /**
   * This is the return state of the search routine. It is encoded as follows.
   * Value | Description
   * ----- | -----------
   * -2    | The position is winning for the player that currently is to move, provided that he plays correctly.
   * 0     | The position is a draw.
   * 1     | The position is still undecided. There is no forced mate possible within the number of moves that the search algorithm used as its maximum depth.
   * 2     | The position is losing for the player that currently is to move, provided that the opponent plays correctly.
   */
  int state;
  
  /**
   * The member variable depth stores the length of the resulting stack, so it equals the number of half-moves that the stack contains.
   */
  unsigned int depth;
  
  /**
   * The member variable moves stores the best move sequence that the search algorithm came up with. It uses the std::stack implementation.
   */
  std::stack<move> moves;
};

/**
 * The DFS class performs the Depth First Search through all the possible positions. It allows two possible search modes.
 * One is the standard mode of search. It searches through the entire tree of possible game continuations with a given depth, and returns the best move sequence of all these continuations.
 * The other is referred to as the turbo mode. This mode is aimed at finding checkmates for the player that initially is to move.
 * Using this mode, the algorithm will correctly find forced mate sequences, if they exist, but in other cases it might not identify the best game continuation for the opponent.
 */
class DFS
{
  //Member variables
  unsigned int maxDepth;  //!< The maximum number of half-moves to be executed from the initial position.
  unsigned int curDepth;  //!< The number of half-moves at which the current branches that are under investigation are being pruned.
  Board* start;           //!< A pointer to the Board object storing the initial position.
  bool turbo;             //!< A boolean variable storing whether the search should be conducted using turbo mode, affecting the behavior of the best_outcome method.
  
  //Private methods
  template<bool T>
  DFSresult best_outcome(Board, unsigned int);  //Method used internally for recursively finding the best continuations.

public:
  //Constructors
  DFS(Board* _start, unsigned int _maxDepth, bool _turbo);  //!< The constructor.
  DFS(Board* _start): DFS(_start, 5, false){}               //!< Default constructor, using the default values.

  //Public methods
  DFSresult search(); //The method that actually searches for the best game continuation.
};
#endif
