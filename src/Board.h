#ifndef BOARD_LIBRARY_DEFINED
#define BOARD_LIBRARY_DEFINED

#include <iostream>
#include <vector>

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

template <typename T>
struct square
{
  T x;
  T y;

  square<T> operator+(const square<T> &other)const{
    square<T> res;
    res.x = x+other.x;
    res.y = y+other.y;
    return res;
  }

  square<T>& operator+=(const square<T> &other){
    x+=other.x;
    y+=other.y;
    return *this;
  }
};

struct move
{
  square<int> start;
  square<int> end;
  Piece::Piece promoteTo; // what to promote to if promotion is possible
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
    drawMask                 = 0x40
  };
  char state;     // Environment flags
  signed char enPassant; // the x coordinate of the enPassent move
  std::vector<move> legalMoves;

  // IO functions to read FEN notation
  int fromStr(const char * str);         // read the FEN notation from a string
  int fromFile(const char * fileName);   // read the FEN notation from a file

  // Function and helper functions to calculate the legal moves
  void calcMoves();                    // Calculate legal moves
  void getPieceMoves(std::vector<move>& result, const check& kingEnv, const square<int> curPos, const square<int> kingPos);	//Calculate the legal moves of the piece on square<int> curPos
  void checkDir(std::vector<move>& result, const check& kingEnv, const square<int> basePos, const square<int> curPos, const square<int> dir) const; //Check the possible moves of a piece along some file, rank or diagonal
  
  check getCheck(const square<int> kingPos); // Get the details about a possible check at kingPos
  bool firstPiece(check& result, const square<int> curPos, const square<int> dir, const int friendlies) const; // Investigate the possibility of attacks from dir to curPos (Recursive) with heatmap.

  bool isAttacked(const square<int> piecePos) const;    // Check whether the square<int> at piecePos is attacked
  bool hasAttacker(square<int> pos, const square<int> dir) const;        // Invesitgate the possibility of attacks from dir at curPos
  inline bool isFriendly(const Piece::Piece piece) const {
    return (piece != Piece::none) && !((state ^ piece) & blackToMoveMask);
  }

  inline bool isFriendly(const square<int> pos) const {
    return isFriendly(board[pos.x][pos.y]);
  }
  
  //Private constructor
  Board(const Board& other, const move mv);

public:
  // Constructors
  Board() : Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", false) {}
  Board(const char *str) : Board(str, false) {}
  Board(const char *str, const bool file); // read from FEN notation from file or string and calculate legal moves.

  // Getters
  Piece::Piece getSquare(const square<int> pos) const {return board[pos.x][pos.y];}
  bool isCheck() const {return state & checkMask;}                   // return checkflag
  bool blackToMove() const {return state & blackToMoveMask;}
  bool isMate() const {return (legalMoves.size() == 0 && state & checkMask);}  // check if current board is mate
  bool isDraw() const {return (state & drawMask);} //check if the current position is stalemated
  std::vector<move> getMoves() const {return legalMoves;}
  
  // Setters
  void execMove(const move mv); // Execute mv.
  void changeColor() {state ^= blackToMoveMask;}
  void clearBoard() {for(int i=0; i<64; i++) board[i/8][i%8] = Piece::none;}
  void setPiece(const square<int> sq, const Piece::Piece piece) {board[sq.x][sq.y]=piece;}
  
  // Cloning function
  Board cloneAndExecMove(const move mv) const;

  // Print function
  void printBoard() const;
  void printLegalMoves() const;
};
#endif
