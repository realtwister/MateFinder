#ifndef BOARD_LIBRARY_DEFINED
#define BOARD_LIBRARY_DEFINED

#include <iostream>

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

static const Piece whitePieces[6] =
{ whiteKing, whiteQueen, whiteRook, whiteBishop, whiteKnight, whitePawn };

static const Piece blackPieces[6] =
{ blackKing, blackQueen, blackRook, blackBishop, blackKnight, blackPawn };
}

struct square
{
  int x;
  int y;

  square operator+(const square &other)const{
    square res;
    res.x = x+other.x;
    res.y = y+other.y;
    return res;
  }

  square& operator+=(const square &other){
    x+=other.x;
    y+=other.y;
    return *this;
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
  int len;
  move        *moves;
  moveArray() : len(0), moves(nullptr) {}
  moveArray(int n) : len(0), moves(new move[n]) {}
  ~moveArray() {delete [] moves;}
};

struct check
{
  int len;
  char heatMap[8][8];

  #ifdef DEBUG
    void display()
    {
      int x,y;
      std::cout << "Debug output of check:" << std::endl;
      std::cout << " - Length: " << (int)len << std::endl;
      std::cout << " - Heatmap: " << std::endl;
      std::cout << "+--------+" << std::endl;
      for (y = 7; y >= 0; y--)
      {
        std::cout << "|";
        for (x = 0; x < 8; x++)
          std::cout << (heatMap[x][y] ? "X" : " ");
        std::cout << "|" << std::endl;
      }
      std::cout << "+--------+" << std::endl;
    }
  #endif
};

class Board {
private:

  #ifdef DEBUG
public:
  #endif
  // Properties
  Piece::Piece board[8][8]; // The board of pieces in FEN notation
  enum stateFlags : char {
    checkMask                = 0x01,
    whiteCastleKingsideMask  = 0x02,
    whiteCastleQueensideMask = 0x04,
    blackCastleKingsideMask  = 0x08,
    blackCastleQueensideMask = 0x10,
    blackToMoveMask          = 0x20,
  };
  char state;     // Environment flags

  signed char enPassant; // the x coordinate of the enPassent move
  moveArray legalMoves;

  // IO functions to read FEN notation
  int fromStr(const char *str);         // read the FEN notation
                                        // from a string
  int fromFile(const char *fileName);   // read the FEN notation
                                        // from a file

  // Function and helper functions to calculate the legal moves
  void  calcMoves();                    // Calculate legal moves
  void	getMoves(moveArray * result, check * kingEnv, const square curPos, const square kingPos);	//Calculate the legal moves of the piece on square curPos
  void  checkDir(moveArray * result, check * kingEnv, const square basePos, const square curPos, const square dir); //Check the possible moves of a piece along some file, rank or diagonal
  
  check getCheck(const square kingPos); // Get the details about a possible
                                        // check at kingPos
  bool  firstPiece(check *result,
                   const square       curPos,
                   const square       dir,
                   const int          friendlies); // Investigate the possibility of
                                             // attacks from dir to curPos
                                             // (Recursive) with heatmap.

  bool isAttacked(const square piecePos);    // Check whether the square at
                                             // piecePos is attacked
  bool hasAttacker(square pos,
                   const square dir);        // Invesitgate the possibility of
                                             // attacks from dir at curPos
  inline bool isFriendly(const Piece::Piece piece) {
    return (piece != Piece::none) && !((state ^ piece) & blackToMoveMask);
  }

  inline bool isFriendly(const square pos) {
    return isFriendly(board[pos.x][pos.y]);
  }
	
  Piece::Piece getPieceType(const square piecePos);
  Piece::Piece getPieceType(const Piece::Piece piece);

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

  bool isCheck(){ return state & checkMask;  }                   // return checkflag
  bool blackToMove(){ return state & blackToMoveMask;}
  bool isMate();                    // check if current board is mate
  
  // Setters
  void execMove(const move mv); // Execute mv.
  void changeColor(){ state ^= blackToMoveMask; }
  void clearBoard(){for(int i=0; i<64; i++) board[i/8][i%8]= Piece::none;}
  void setPiece(const square sq, const Piece::Piece piece){board[sq.x][sq.y]=piece;}

  // Print function
  void printBoard();
  void printLegalMoves();
};
#endif
