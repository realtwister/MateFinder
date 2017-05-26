#ifndef MOVE_HEADER_DEFINED
#define MOVE_HEADER_DEFINED

#include <iostream>
#include "Square.h"
#include "Piece.h"

/**
 * The move struct stores one move. It stores both the start and end squares in a compact way, to reduce memory copying overheads.
 */
struct move
{ 
  /**
   * The square from which the piece starts its move.
   */
  square<void> start;
  
  /**
   * The square where the piece lands after the move.
   */
  square<void> end;
  
  /**
   * The piece a pawn promotes to if it reaches the other side of the board, using FEN notation. The piece is always stored in white.
   */
  Piece::Piece promoteTo;

  /**
   * This function displays the move in the terminal.
   */
  void printMove(bool blackToMove)
  {
    std::cout << (char)(start.x + 'a') << ((blackToMove ? 7 - start.y : start.y) + 1) << "-" << (char)(end.x + 'a') << ((blackToMove ? 7 - end.y : end.y) + 1);
    if (end.y == (blackToMove ? 0 : 7)) std::cout << (char)promoteTo;
  }
};

#endif
