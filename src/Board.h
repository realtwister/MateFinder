namespace BoardExceptions {}

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
  none        = 0
};

const Piece whitePieces[6] =
{ whiteKing, whiteQueen, whiteRook, whiteBishop, whiteKnight, whitePawn };

const Piece blackPieces[6] =
{ blackKing, blackQueen, blackRook, blackBishop, blackKnight, blackPawn };
}

struct square
{
  signed char x;
  signed char y;
};

struct move
{
  square       start;
  square       end;
  Piece::Piece promoteTo; // what to promote to if promotion is possible
};

struct moveArray
{
  int   len;
  move *moves;
};

struct Check
{
  unsigned char len;
  char          heatMap[8];
};

class Board {
private:

  // Properties
  Piece::Piece board[8][8]; // The board of pieces in FEN notation
  enum STATE_FLAGS : char {
    whiteToMoveMask          = 0x01,
    whiteCastleKingsideMask  = 0x02,
    whiteCastleQueensideMask = 0x04,
    blackCastleKingsideMask  = 0x08,
    blackCastleQueensideMask = 0x10,
    CheckMask                = 0x20,
  };
  char state;     // Environment flags

  char enPassant; // the x coordinate of the enPassent move
  moveArray legalMoves;

  // IO functions to read FEN notation
  int fromStr(const char *str);         // read the FEN notation
                                        // from a string
  int fromFile(const char *fileName);   // read the FEN notation
                                        // from a file

  // Function and helper functions to calculate the legal moves
  void  calcMoves();                    // Calculate legal moves
  Check getCheck(const square kingPos); // Get the details about a possible
                                        // check at kingPos
  bool  firstPiece(const Check *result,
                   square       curPos,
                   square       dir,
                   char         friendlies); // Investigate the possibility of
                                             // attacks from dir to curPos
                                             // (Recursive) with heatmap.

  bool isAttacked(const square piecePos);    // Check whether the square at
                                             // piecePos is attacked
  bool hasAttacker(square curPos,
                   square dir);              // Invesitgate the possibility of
                                             // attacks from dir at curPos.

public:

  // Constructors
  Board() : Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
                  false) {}

  Board(const char *str) : Board(str,
                                 false) {}

  Board(const char *str,
        const bool  file); // read from FEN notation from file or
                           // string and calculate legal moves.

  // Getters
  char getSquare(const square pos) {
    return board[pos.x][pos.y];
  } // get the piece at pos

  bool isCheck();                   // return checkflag
  bool isMate();                    // check if current board is mate

  // Setters
  void execMove(move mv); // Execute mv.

  // Print function
  void printBoard();
};
