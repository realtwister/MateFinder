#ifndef PIECE_HEADER_DEFINED
#define PIECE_HEADER_DEFINED

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
  blackPawn   = 'p',
  none        = ' '
};

static const Piece whitePieces[6] =
{ whiteKing, whiteQueen, whiteRook, whiteBishop, whiteKnight, whitePawn };

static const Piece blackPieces[6] =
{ blackKing, blackQueen, blackRook, blackBishop, blackKnight, blackPawn };
}

#endif
