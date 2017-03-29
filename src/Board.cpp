#include "Board.h"
#include <iostream>
#include <fstream>
#include <exception>
#include <iostream>
#include <cmath>
#include <tgmath.h>

static square knightMoves[8]={
  {-2,1},
  {-2,-1},
  {2,1},
  {2,-1},
  {1,-2},
  {-1,-2},
  {1,2},
  {-1,2}
};
// PRIVATE METHODS

/**
 * Create a Board object from a string in FEN notation
 * @param  str The string in FEN notation
 * @return int error codes:
 *                0: No error
 *                1: Syntax of str is wrong
 */
int Board::fromStr(const char * str) {
  int x = 0, y = 7;

  // read Board
  while (*str != ' ')
  {
    switch (*str++) {
      #define PIECE_CASE(c, p) case c: \
  if (x > 7) return 1;                 \
  board[x][y] = Piece::p;              \
  x++;                                 \
  break;                               \

      PIECE_CASE('K', whiteKing)
      PIECE_CASE('Q', whiteQueen)
      PIECE_CASE('R', whiteRook)
      PIECE_CASE('B', whiteBishop)
      PIECE_CASE('N', whiteKnight)
      PIECE_CASE('P', whitePawn)

      PIECE_CASE('k', blackKing)
      PIECE_CASE('q', blackQueen)
      PIECE_CASE('r', blackRook)
      PIECE_CASE('b', blackBishop)
      PIECE_CASE('n', blackKnight)
      PIECE_CASE('p', blackPawn)

      #undef PIECE_CASE
    case '/':

      if (x != 8) return 1;  // Syntax error TODO:add Exception?

      y--;
      x = 0;
      break;

      #define NUM_CASE(c, n) case c: \
  if (x > n) return 1;               \
  board[x][y] = Piece::none;         \
  x++;

      NUM_CASE('8', 0)
      NUM_CASE('7', 1)
      NUM_CASE('6', 2)
      NUM_CASE('5', 3)
      NUM_CASE('4', 4)
      NUM_CASE('3', 5)
      NUM_CASE('2', 6)
      NUM_CASE('1', 7)
      break;

      #undef NUM_CASE

    default:
      return 1;
    }
  }
  str++; // skip space

  // Who is to move?
  switch (*str++) {
  case 'w':
    state = 0;
    break;

  case 'b':
    state = blackToMoveMask;
    break;

  default:
    return 1;
  }
  str++; // skip space

  // read castle flags
  if (*str == '-') {
    str++;
  }
  else {
    if (*str == 'K') {
      state |= whiteCastleKingsideMask;
      str++;
    }

    if (*str == 'Q') {
      state |= whiteCastleQueensideMask;
      str++;
    }

    if (*str == 'k') {
      state |= blackCastleKingsideMask;
      str++;
    }

    if (*str == 'q') {
      state |= blackCastleQueensideMask;
      str++;
    }
  }
  str++;
  enPassant = (*str >= 'a' and * str <= 'h') ? (*str - 'a') : -1;
  return 0;
}

/**
 * Create a Board object from a file containing a string in FEN notation
 * @param  fileName The filename of the file
 * @return      int Error codes:
 *                    0: No error
 *                    1: Syntax error in FEN notation
 *                    2: Error reading file
 */
int Board::fromFile(const char *fileName) {
  std::ifstream file;

  std::cout << fileName << std::endl;
  file.open(fileName);

  if (!file.good()) return 2;  // Error reading file

  char str[255];
  file.getline(str, 255);
  std::cout << str << std::endl;
  return fromStr(str);
}

bool Board::isAttacked(const square piecePos) {
  int xmin, xmax, ymin, ymax, i, j;

  xmin = piecePos.x == 0 ? 0 : -1;
  xmax = piecePos.x == 7 ? 0 : 1;
  ymin = piecePos.y == 0 ? 0 : -1;
  ymax = piecePos.y == 7 ? 0 : 1;


  for (i = xmin; i <= xmax; i++) {
    for (j = ymin; j <= ymax; j++) {
      if (!((i == 0) && (j == 0)) && hasAttacker(piecePos, { i, j })) return true;
    }
  }
  square pos;
  for(i=0; i<8;i++){
    pos = piecePos+knightMoves[i];
    if((pos.x<8 and pos.y<8 and pos.x>=0 and pos.y>=0)and (board[pos.x][pos.y]== (Piece::whiteKnight|(~state & blackToMoveMask)))) return true;
  }
  return false;
}

/**
 * This function finds out what is going on in relation to the king of the player that is to move
 * In particular, it returns a check struct, containing the number of pieces attacking the king,
 * and 8 charachters containing a bitmap of the entire board, where a set bit indicates either
 * a possible square to resolve check if the number of attackers is one and the square is empty,
 * or a pinned piece if it occupied by a friendly piece. Moreover, it updates the check flag.
 */
check Board::getCheck()
{
	check result = {0,{0,0,0,0,0,0,0,0}};
	state &= ~checkMask;

	//First, get the position of the king
	square kingPos;
	for (int i = 0; i < 64; i++)
	{
		if (board[i / 8][i % 8] == (Piece::whiteKing ^ (state & blackToMoveMask)))
		{
			kingPos = {i / 8,i % 8};
			break;
		}
	}

	//Now, find out if the king is under attack by some long range piece
	int xmin = (kingPos.x == 0 ? 0 : -1);
	int xmax = (kingPos.x == 7 ? 0 : 1);
	int ymin = (kingPos.y == 0 ? 0 : -1);
	int ymax = (kingPos.y == 7 ? 0 : 1);
	int dirx,diry;
	for (dirx = xmin; dirx <= xmax; dirx++)
	  for (diry = ymin; diry <= ymax; diry++)
    {
      if (dirx != 0 || diry != 0);
      if (firstPiece(&result, kingPos, {dirx,diry}, 0)) state |= checkMask;
    }

  //Find out if the king is under attack by a knight
  square newPos;
  for(int i=0; i<8; i++)
  {
    newPos = kingPos + knightMoves[i];
    if (newPos.x >= 0 && newPos.x <= 7 && newPos.y >= 0 && newPos.y <= 7 && board[newPos.x][newPos.y] == (Piece::blackKnight ^ (state & blackToMoveMask)))
    {
      result.len++;
      result.heatMap[newPos.y] |= (0x1 << newPos.x);
      state |= checkMask;
    }
  }

	//Find out if the king is under attack by a pawn
	int dir = 1 - ((state & blackToMoveMask) >> 5) * 2;
	if (kingPos.y + dir < 8 && kingPos.y + dir >= 0)
	{
	  if (kingPos.x < 7)
	  {
	    if (board[kingPos.x + 1][kingPos.y + dir] == (Piece::blackPawn ^ (state & blackToMoveMask)))
	    {
  	    result.len++;
	      result.heatMap[kingPos.y + dir] |= (0x1 << (kingPos.x + 1));
	      state |= checkMask;
	    }
	  }
	  if (kingPos.x > 0)
	  {
	    if (board[kingPos.x - 1][kingPos.y + dir] == (Piece::blackPawn ^ (state & blackToMoveMask)))
	    {
  	    result.len++;
	      result.heatMap[kingPos.y + dir] |= (0x1 << (kingPos.x - 1));
	      state |= checkMask;
	    }
	  }
	}

	//Note that the king cannot be attacked by the other king, so we need not check that
	return result;
}

/**
 * This function helps the getCheck function to investigate the influence of one of the 8 directions
 * on the output of the getCheck function.
 */
bool Board::firstPiece(check * result, const square curPos, const square dir, const int friendlies)
{
	square newsq = curPos + dir;
  if (newsq.x >= 0 && newsq.x <= 7 && newsq.y >= 0 && newsq.y <= 7)
  {
    if (board[newsq.x][newsq.y] == Piece::none)
    {
      //Deal with empty squares
      if (firstPiece(result, newsq, dir, friendlies))
      {
        if (friendlies == 0)
          result->heatMap[newsq.y] |= (0x1 << newsq.x);
        return true;
      }
      return false;
    }

    if (isFriendly(newsq))
    {
      //Deal with pinned pieces
      if (friendlies == 1)
        return false;
      if (firstPiece(result, newsq, dir, 1))
        result->heatMap[newsq.y] |= (0x1 << newsq.x);
      return false;
    }
    else
    {
      //Deal with direct attackers
      Piece::Piece piece = (Piece::Piece)(board[newsq.x][newsq.y] & ~((char)0x20));
      if (piece == Piece::whiteQueen || (piece == Piece::whiteRook && dir.x * dir.y == 0) || (piece == Piece::whiteBishop && dir.x * dir.y != 0))
      {
        if (friendlies == 0)
        {
          result->heatMap[newsq.y] |= (0x1 << newsq.x);
          result->len++;
        }
        return true;
      }
    }
  }
  return false;
}

bool Board::hasAttacker(square pos, const square dir) {
  int dist = 0;
  if(dir.x == 0){
    dist = dir.y >= 0 ? 7 - pos.y : pos.y;
  }
  else if(dir.y == 0){
      dist = dir.x >= 0 ? 7 - pos.x : pos.x;
  }
  else{
    dist = fmin(dir.x >= 0 ? 7 - pos.x : pos.x,dir.y >= 0 ? 7 - pos.y : pos.y);
  }
  for (int i = 0; i < dist; i++) {
    pos += dir;
    if (board[pos.x][pos.y] != Piece::none) {
      if (isFriendly(pos)) return false;
      else if (dir.x * dir.y) {
        switch (board[pos.x][pos.y] & ~((char)0x20)) {
        case Piece::whiteKing:
          return i==0;
        case Piece::whitePawn:
          if (i > 0) return false;

          return !(state & blackToMoveMask) != !(dir.y == 1);

        case Piece::whiteQueen:
        case Piece::whiteBishop:
          return true;

        default:
          return false;
        }
      }
      else {
        switch (board[pos.x][pos.y] & ~((char)0x20)) {
          case Piece::whiteKing:
            return i==0;
          case Piece::whiteQueen:
          case Piece::whiteRook:
            return true;
          default:
            return false;
        }
      }
    }
  }
  return false;
}

bool Board::isFriendly(const square pos) {
  return !((state ^ board[pos.x][pos.y]) & blackToMoveMask ||
           (board[pos.x][pos.y] == Piece::none));
}

bool Board::isFriendly(const Piece::Piece piece) {
  return !((state ^ piece) & blackToMoveMask);
}

// PUBLIC FUNCTIONS

/**
 * Board constructor creating Board object from either a file or a string
 * @param   str Either the FEN string or the filename
 * @param  file Boolean whether to read file or string (true is file)
 * @return void
 */

Board::Board(const char *str,
             const bool  file) {
  int error;

  if (file) {
    error = fromFile(str);
  }
  else {
    error = fromStr(str);
  }

  if (error) {
    switch (error) {
    case 1:
      throw std::invalid_argument("Syntax error encountered during FEN reading");
      break;

    case 2:
      throw std::invalid_argument("Error while reading file");
      break;

    default:
      throw std::runtime_error("Unexpected return value");
    }
  }
}

/**
 * Print a formatted representation of the board.
 */
void Board::printBoard() {
  // Board
  std::cout << " +------------------------+" << std::endl;

  for (int i = 7; i >= 0; i--) {
    std::cout << i + 1 << '|';

    for (int j = 0; j < 8; j++) {
      std::cout << ' '
                << (char)board[j][i]
                << ' ';
    }
    std::cout << '|' << std::endl;
  }
  std::cout << " +------------------------+"
            << std::endl
            << "   A  B  C  D  E  F  G  H "
            << std::endl
            << std::endl;

  // Who to move
  std::cout << "        "
            << ((state & blackToMoveMask) ? "black" : "white")
            << " to move"
            << std::endl
            << std::endl;

  // Castle details
  std::cout << "CASTLE| queenside kingside"
            << std::endl
            << " White|      "
            << ((state & whiteCastleQueensideMask) ? "1" : "0")
            << "        "
            << ((state & whiteCastleKingsideMask) ? "1" : "0")
            << std::endl

            << " Black|      "
            << ((state & blackCastleQueensideMask) ? "1" : "0")
            << "        "
            << ((state & blackCastleKingsideMask) ? "1" : "0")
            << std::endl;

  // enPassant
  if (enPassant >= 0) {
    std::cout << "En passant on file: " << char('a' + enPassant);
  }
}
