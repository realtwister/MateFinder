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

template <typename T>
struct square
{
  T x;
  T y;

  square<T>() {}
  square<T>(const T x, const T y) : x(x), y(y) {}
  template<typename newT>
  square<T>(const square<newT>& other) : x(other.x), y(other.y) {}
  
  template<typename newT>
  square<typename std::common_type<T,newT>::type> operator+(const square<newT> &other) const {return square<typename std::common_type<T,newT>::type>(x+other.x,y+other.y);}
  square<T> operator+(const square<void> &other) const;

  square<T>& operator+=(const square<T> &other) {x+=other.x; y+=other.y; return *this;}
  square<T>& operator+=(const square<void> &other);
};

template<>
struct square<void>
{
  signed char x:4;
  signed char y:4;
  
  square<void>() {}
  square<void>(const int x, const int y) : x(x), y(y) {}
  template<typename newT>
  square<void>(const square<newT>& other) : x(other.x), y(other.y) {}
  
  template<typename newT>
  square<newT> operator+(const square<newT>& other) {return square<newT>(x+other.x,y+other.y);}
  
  template<typename newT>
  square<void>& operator+=(const square<newT>& other) {x += other.x; y += other.y; return *this;}
};

template<typename T>
square<T> square<T>::operator+(const square<void> &other) const {return square<T>(x + other.x,y + other.y);}

template<typename T>
square<T>& square<T>::operator+=(const square<void> &other) {x += other.x; y+= other.y; return *this;}

struct move
{
  square<void> start;
  square<void> end;
  Piece::Piece promoteTo; // what to promote to if promotion is possible
  
  void printMove(bool blackToMove)
  {
    std::cout << (char)(start.x + 'a') << ((blackToMove ? 7 - start.y : start.y) + 1) << "-" << (char)(end.x + 'a') << ((blackToMove ? 7 - end.y : end.y) + 1) << std::endl;
  }
};

struct check
{
  int len;
  char heatMap[8][8];

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
};

struct moveArray
{
  int num;
  int ctr;
  move * moves;
  
  moveArray() : num(0), ctr(0), moves(NULL) {}
  moveArray(const int n) : num(n), ctr(0), moves(new move[n]) {}
  moveArray(const moveArray& other)
  {
    num = other.num;
    ctr = other.ctr;
    moves = new move[num];
    for (int i = 0; i < ctr; i++)
      moves[i] = other.moves[i];
  }
  ~moveArray() {delete[] moves;}
  
  moveArray& operator=(const moveArray& other)
  {
    num = other.num;
    ctr = other.ctr;
    moves = new move[num];
    for (int i = 0; i < ctr; i++)
      moves[i] = other.moves[i];
    return *this;
  }
  
  moveArray& operator=(moveArray&& other)
  {
    num = other.num;
    ctr = other.ctr;
    moves = other.moves;
    other.moves = NULL;
    return *this;
  }
  
  inline move& operator[](const int n) const {return moves[n];}
  inline int size() const {return ctr;}
  
  inline void push_back(const move toAdd)
  {
    #ifdef DEBUG
    if (ctr >= num)
    {
      std::cout << "Overflow in the moveArray." << std::endl;
      return;
    }
    #endif
    moves[ctr++] = toAdd;
  }
  
  inline moveArray shrink_to_fit()
  {
    moveArray result(ctr);
    result.ctr = ctr;
    for (int i = 0; i < ctr; i++)
      result.moves[i] = moves[i];
    return result;
  }
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
  moveArray legalMoves;

  // IO functions to read FEN notation
  int fromStr(const char * str);         // read the FEN notation from a string
  int fromFile(const char * fileName);   // read the FEN notation from a file

  // Function and helper functions to calculate the legal moves
  void calcMoves();                    // Calculate legal moves
  void getPieceMoves(moveArray& result, const check& kingEnv, const square<int> curPos, const square<int> kingPos);	//Calculate the legal moves of the piece on square<int> curPos
  inline void checkDir(moveArray& result, const check& kingEnv, const square<int> basePos, const square<int> dir) const; //Check the possible moves of a piece along some file, rank or diagonal
  
  check getCheck(const square<int> kingPos); // Get the details about a possible check at kingPos
  bool firstPiece(check& result, const square<int> curPos, const square<int> dir, const int friendlies) const; // Investigate the possibility of attacks from dir to curPos (Recursive) with heatmap.

  bool isAttacked(const square<int> piecePos) const;    // Check whether the square<int> at piecePos is attacked
  bool hasAttacker(square<int> pos, const square<int> dir) const;        // Invesitgate the possibility of attacks from dir at curPos
  inline bool isFriendly(const Piece::Piece piece) const {
    return !(piece & 0x20);
  }

  inline bool isFriendly(const square<int> pos) const {
    return isFriendly(board[pos.x][pos.y]);
  }
  
  void flipBoard();
  
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
  moveArray& getMoves() {return legalMoves;}
  
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
