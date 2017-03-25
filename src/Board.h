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
  none        = ' '
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

  square operator+(const square &other)const{
    square res;
    res.x = x+other.x;
    res.y = y+other.y;
    return res;
  }
};

struct move
{
  square       start;
  square       end;
  Piece::Piece promoteTo; // what to promote to if promotion is possible
};

struct moveArray
{
  unsigned int len;
  move        *moves;
};

struct check
{
  unsigned char len;
  char          heatMap[8];
};

class Board {
private:

  // Properties
  Piece::Piece board[8][8]; // The board of pieces in FEN notation
  enum stateFlags : char {
    checkMask          = 0x01,
    whiteCastleKingsideMask  = 0x02,
    whiteCastleQueensideMask = 0x04,
    blackCastleKingsideMask  = 0x08,
    blackCastleQueensideMask = 0x10,
    blackToMoveMask          = 0x20,
  };
  char state;     // Environment flags

  signed char enPassant; // the x coordinate of the enPassent move
  moveArray legalMoves;
  #ifdef DEBUG
public:
  #endif
  // IO functions to read FEN notation
  int fromStr(const char *str);         // read the FEN notation
                                        // from a string
  int fromFile(const char *fileName);   // read the FEN notation
                                        // from a file

  // Function and helper functions to calculate the legal moves
  void  calcMoves();                    // Calculate legal moves
  check getCheck(); // Get the details about a possible
                                        // check at kingPos
  bool  firstPiece(const check *result,
                   const square       curPos,
                   const square       dir,
                   const char         friendlies); // Investigate the possibility of
                                             // attacks from dir to curPos
                                             // (Recursive) with heatmap.

  bool isAttacked(const square piecePos);    // Check whether the square at
                                             // piecePos is attacked
  bool hasAttacker(square curPos,
                   const square dir);              // Invesitgate the possibility of
                                             // attacks from dir at curPos
  bool isFriendly(const square piecePos);
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
  Piece::Piece getSquare(const square pos){ return board[pos.x][pos.y];}

  bool isCheck(){ return state && checkMask;  }                   // return checkflag
  bool isMate();                    // check if current board is mate

  // Setters
  void execMove(const move mv); // Execute mv.

  // Print function
  void printBoard();
};
