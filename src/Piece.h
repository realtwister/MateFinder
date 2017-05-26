#ifndef PIECE_HEADER_DEFINED
#define PIECE_HEADER_DEFINED

/**
 * Here, some convenient definitions regarding the pieces are made.
 */
namespace Piece {
/**
 * This enumerations connect every FEN character with a keyword.
 */
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

/**
 * Array of all the white pieces.
 */
static const Piece whitePieces[6] =
{ whiteKing, whiteQueen, whiteRook, whiteBishop, whiteKnight, whitePawn };

/**
 * Array of all the black pieces.
 */
static const Piece blackPieces[6] =
{ blackKing, blackQueen, blackRook, blackBishop, blackKnight, blackPawn };
}

#endif
