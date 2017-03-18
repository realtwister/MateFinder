#include <iostream>
#include <ctime>

#define LOOPS 1000000
namespace Piece {
enum Piece : char {
  whiteKing   = 'K',
  whiteQueen  = 'Q',
  whiteRook   = 'R',
  whiteBishop = 'B',
  whiteKnight = 'N',
  whitePawn   = 'P',
  blackKing   = 'k',
  blackQueen  = 'q',
  blackRook   = 'r',
  blackBishop = 'b',
  blackKnight = 'n',
  blackPawn   = 'p'
};
}


int main(int argc, char const *argv[]) {
  std::clock_t start;
  double duration_a = 0; double duration_b = 0;

  for (int k = 0; k < 1000; k++) {
    start = std::clock();

    for (int n = 0; n < LOOPS; n++) {
      Piece::Piece board[8][8];

      for (int i = 0; i < 8; i++) {
        board[0][i] = Piece::whiteQueen;
        board[1][i] = Piece::whitePawn;
      }
    }
    duration_a += (std::clock() - start) / (double)CLOCKS_PER_SEC;

    start = std::clock();

    for (int n = 0; n < LOOPS; n++) {
      char board[8][8];

      for (int i = 0; i < 8; i++) {
        board[0][i] = 'K';
        board[1][i] = 'P';
      }
    }
    duration_b += (std::clock() - start) / (double)CLOCKS_PER_SEC;
  }

  std::cout << "forLoop with Piece: " << duration_a << '\n';
  std::cout << "forLoop with char: " << duration_b << '\n';

  return 0;
}
