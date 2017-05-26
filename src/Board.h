#ifndef BOARD_LIBRARY_DEFINED
#define BOARD_LIBRARY_DEFINED

#include <iostream>
#include "Piece.h"
#include "Square.h"
#include "Move.h"
#include "MoveArray.h"

/**
 * The check struct stores all the information that is somewhat related to the king, and is used for internal communication within the \a calcMoves method only.
 * On the one hand, it stores the number of pieces that are giving check to the king, and if the king is checked, which squares can be used to resolve this check.
 * Furthermore, it stores whether a piece is pinned, and if so, in which direction.
 */
struct check
{
  int len; //!< The number of pieces attacking the king.
  /**
   * All the 64 character entries of the heatmap correspond to exactly one square on the chess board.
   * The encoding of the heatmap depends on the piece that is occupying the square. The table below lists all the cases.
   * <table>
   * <tr><th>Occupation of the square     <th>Value    <th>Description
   * <tr><td rowspan = "2">Empty          <td>0        <td>No peculiarities.
   * <tr>                                 <td>1        <td>Placing a friendly piece here will resolve check.
   * <tr><td rowspan = "2">Enemy piece    <td>0        <td>No peculiarities.
   * <tr>                                 <td>1        <td>Taking this piece will resolve check.
   * <tr><td rowspan = "5">Friendly piece <td>0        <td>No peculiarities.
   * <tr>                                 <td>1        <td>This piece is pinned, such that it can only move in the northwest-southeast direction.
   * <tr>                                 <td>2        <td>This piece is pinned, such that it can only move in the horizontal direction.
   * <tr>                                 <td>3        <td>This piece is pinned, such that it can only move in the northeast-southwest direction.
   * <tr>                                 <td>4        <td>This piece is pinned, such that it can only move in the vertical direction.
   * </table>
   * Some edge cases are not included in the check structure. For example, consider the following board:
   * @image html "../../img/EnPassantPin.png"
   * @image latex "../../img/EnPassantPin.png" width=4cm
   * If white now decides to take the black pawn en passant, then white checks himself, so this is not a legal move.
   * Yet the white pawn is not considered to be pinned, as it can advance without checking anyone.
   * This, then, is a situation that cannot be stored in the check struct, and needs to be handled separately.
   */
  char heatMap[8][8];
};

/**
 * Objects of the Board class store exactly one position on the chess board.
 * Methods of this class include methods to calculate the all legal moves in the position, and executing a move to obtain a new position.
 * A lot of optimization has gone into the methods of Board and their internal communication.
 * The following flowchart shows the respective contribution of the methods to the total runtime of the program. It was created using google-pprof.
 * @image html "../../img/timing.png"
 * @image latex "../../img/timing.png" width=8cm
 */
class Board {
#ifdef DEBUG
public: //The test functions also test the private methods, so that is why this debug hack is needed.
#endif
  //Member variables
  /**
   * This character array stores the position of the pieces on the board. Every entry in the character array corresponds to one square.
   * The values of these characters correspond to the widely used FEN notation.
   * Value | ASCII representation | Piece
   * ----- | ------ | -----
   * 0x20  | Space  | None
   * 0x4B  | K      | White king
   * 0x51  | Q      | White queen
   * 0x52  | R      | White rook
   * 0x42  | B      | White bishop
   * 0x4E  | N      | White knight
   * 0x50  | P      | White pawn
   * 0x6B  | k      | Black king
   * 0x71  | q      | Black queen
   * 0x72  | r      | Black rook
   * 0x62  | b      | Black bishop
   * 0x6E  | n      | Black knight
   * 0x70  | p      | Black pawn
   * A convenient feature of this notation is that determining whether a piece is black can be done by doing an AND operation with the bitmask 0x20.
   */
  Piece::Piece board[8][8];
  
  enum stateFlags : char {
    checkMask                = 0x01,
    whiteCastleKingsideMask  = 0x02,
    whiteCastleQueensideMask = 0x04,
    blackCastleKingsideMask  = 0x08,
    blackCastleQueensideMask = 0x10,
    blackToMoveMask          = 0x20,
    drawMask                 = 0x40
  };
  /**
   * This character contains seven flags. These flags store some auxiliary information about the position. They are listed in the table below.
   * Bit | Keyword                  | Description
   * --- | ------------------------ | -----------
   * 0   | checkMask                | This flag is set if the player to move is checked.
   * 1   | whiteCastleKingsideMask  | This flag is set if the player to move has not yet moved his king and kingside rook.
   * 2   | whiteCastleQueensideMask | This flag is set if the player to move has not yet moved his king and queenside rook.
   * 3   | blackCastleKingsideMask  | This flag is set if the player that is not to move has not yet moved his king and kingside rook.
   * 4   | blackCastleQueensideMask | This flag is set if the player that is not to move has not yet moved his king and queenside rook.
   * 5   | blackToMoveMask          | This flag is set if black currently is to move.
   * 6   | drawMask                 | This flag is set if there is a stalemate, or if giving checkmate with the pieces that are left on the board is not possible. In both cases, the game is considered to be drawn.
   * Notice that it is no coincidence that the blackToMoveMask corresponds to the fifth bit. The fifth bit being set in a character yields 0x20, which is exactly the mask that can be used to determine whether a piece is black or white.
   */
  char state;
  
  /**
   * This signed character stores whether the opponent in the last move advanced one of his pawns by two squares.
   * If he did not, then the value is -1. Otherwise, it is in the range 0-7, where the value corresponds to the file on which the pawn was advanced in the last move.
   */
  signed char enPassant;
  
  /**
   * Here, all the legal moves that the player to move can play are stored. We have chosen not to go with the std::vector<move> implementation, as our custom implementation saves about 15% of runtime.
   */
  moveArray legalMoves;

  // IO functions to read FEN notation
  int fromStr(const char * str);         //!< Read the FEN notation from a string
  int fromFile(const char * fileName);   //!< Read the FEN notation from a file

  // Methods and helper functions to calculate the legal moves
  void calcMoves();                                                                                                       //!< Calculate legal moves
  void getPieceMoves(moveArray& result, const check& kingEnv, const square<int> curPos, const square<int> kingPos);	      //!< Calculate the legal moves of the piece on square<int> curPos
  inline void checkDir(moveArray& result, const check& kingEnv, const square<int> basePos, const square<int> dir) const;  //!< Check the possible moves of a piece along some file, rank or diagonal
  check getCheck(const square<int> kingPos);                                                                              //!< Get the details about a possible check at kingPos
  bool firstPiece(check& result, const square<int> curPos, const square<int> dir, const int friendlies) const;            //!< Investigate the possibility of attacks from dir to curPos (recursive) with heatmap.
  bool isAttacked(const square<int> piecePos) const;                                                                      //!< Check whether the square<int> at piecePos is attacked
  bool hasAttacker(square<int> pos, const square<int> dir) const;                                                         //!< Invesitgate the possibility of attacks from dir at curPos
  inline bool isFriendly(const Piece::Piece piece) const {return !(piece & 0x20);}                                        //!< Find out if a piece is friendly
  inline bool isFriendly(const square<int> pos) const {return isFriendly(board[pos.x][pos.y]);}                           //!< Find out if a square is occupied by a friendly piece

  // Methods needed for executing moves
  Board(const Board& other, const move mv);  //!< Private constructor to circumvent calculating the possible legal moves twice
  void flipBoard();                          //!< Flip the board such that pawns always advance in the direction of increasing rank

public:
  // Constructors
  Board() : Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", false) {} //!< Empty constructor, defaulting to the starting position
  Board(const char *str) : Board(str, false) {}                                         //!< Read from FEN string
  Board(const char *str, const bool file);                                              //!< Read from FEN notation from file or string and calculate legal moves

  // Getters
  Piece::Piece getSquare(const square<int> pos) const {return board[pos.x][pos.y];} //!< Get what piece is occupying the given square
  bool isCheck() const {return state & checkMask;}                                  //!< Get whether the player to move is checked
  bool blackToMove() const {return state & blackToMoveMask;}                        //!< Get whether black is to move
  bool isMate() const {return (legalMoves.size() == 0 && state & checkMask);}       //!< Get whether the current position is mate
  bool isDraw() const {return (state & drawMask);}                                  //!< Get whether the current position is drawn
  moveArray& getMoves() {return legalMoves;}                                        //!< Get the list of legal moves

  // Setters
  void execMove(const move mv);                                                             //!< Execute a move
  void changeColor() {state ^= blackToMoveMask;}                                            //!< Change the color of the player that is to move
  void clearBoard() {for(int i=0; i<64; i++) board[i/8][i%8] = Piece::none;}                //!< Clear the entire board
  void setPiece(const square<int> sq, const Piece::Piece piece) {board[sq.x][sq.y]=piece;}  //!< Place a piece on a given square

  // Cloning function
  Board cloneAndExecMove(const move mv) const;  //!< Clone the current position and execute a move

  // Print function
  void printBoard() const;  //!< Print a formatted representation of the board
};
#endif
