#ifndef MOVE_HEADER_DEFINED
#define MOVE_HEADER_DEFINED

#include <iostream>
#include "Square.h"
#include "Piece.h"

struct move
{
  square<void> start;
  square<void> end;
  Piece::Piece promoteTo; // what to promote to if promotion is possible

  void printMove(bool blackToMove)
  {
    std::cout << (char)(start.x + 'a') << ((blackToMove ? 7 - start.y : start.y) + 1) << "-" << (char)(end.x + 'a') << ((blackToMove ? 7 - end.y : end.y) + 1);
  }
};

#endif
