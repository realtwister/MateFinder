#include "Board.h"
#include <iostream>
#include <fstream>
#include <exception>
#include <cmath>

// PRIVATE METHODS

/**
 * Create a Board object from a string in FEN notation
 * @param  str The string in FEN notation
 * @return int error codes:
 *                0: No error
 *                1: Syntax of str is wrong
 */
int Board::fromStr(const char *str) {
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
  int xmin, xmax, ymin, ymax;

  xmin = piecePos.x == 0 ? 0 : -1;
  xmax = piecePos.x == 7 ? 0 : 1;
  ymin = piecePos.y == 0 ? 0 : -1;
  ymax = piecePos.y == 7 ? 0 : 1;


  for (signed char i = xmin; i < xmax; i++) {
    for (signed char j = ymin; j < ymax; j++) {
      if (!((i == 0) && (j == 0)) && hasAttacker(piecePos, { i, j })) return true;
    }
  }
  return false;
}

/**
 * This function finds out what is going on in relation to the king of the player that is to move
 * In particular, it returns a check struct, containing the number of pieces attacking the king,
 * and 8 charachters containing a bitmap of the entire board, where a set bit indicates either
 * a possible square to resolve check if the number of attackers is one and the square is empty,
 * or a pinned piece if it occupied by a friendly piece.
 */
check Board::getCheck()
{
	check result = {0,{0,0,0,0,0,0,0,0}};
	
	//First, get the position of the king
	signed char x,y;
	for (x = 0; x < 8; x++)
	  for (y = 0; y < 8; y++) {
	    if (board[x][y] == (Piece::whiteKing ^ (state & blackToMoveMask))) goto kingposfound;
	  }
	kingposfound:;
	
	//Now, find out if the king is under attack by some long range piece
	signed char xmin = (x == 0 ? 0 : -1);
	signed char xmax = (x == 7 ? 0 : 1);
	signed char ymin = (y == 0 ? 0 : -1);
	signed char ymax = (y == 7 ? 0 : 1);
	signed char dirx,diry;
	for (dirx = xmin; dirx <= xmax; dirx++)
	  for (diry = ymin; diry <= ymax; diry++)
    {
      if (dirx != 0 || diry != 0);
      firstPiece(&result, {x,y}, {dirx,diry}, 0);
    }
  
  //Find out if the king is under attack by a knight
  signed char newx,newy;
	for (signed char hordir = -1; hordir <= 1; hordir += 2)
	  for (signed char verdir = -1; verdir <= 1; verdir += 2)
	    for (signed char absx = 1; absx <= 2; absx++)
	    {
	      newy = x + verdir * (3 - absx);
	      newx = y + hordir * absx;
	      
	      if (newx >= 0 && newx <= 7 && newy >= 0 && newy <= 7 && board[newx][newy] == (Piece::whiteKnight | (state & blackToMoveMask)))
	      {
	        result.len++;
	        result.heatMap[newy] |= (0x1 << newx);
	      }
	    }
	
	//Find out if the king is under attack by a pawn
	signed char dir = ((state & blackToMoveMask) >> 5) * 2 - 1;
	if (y + dir < 8 && y + dir >= 0)
	{
	  if (x < 7)
	  {
	    if (board[y + dir][x + 1] == (Piece::blackPawn ^ (state & blackToMoveMask)))
	    {
  	    result.len++;
	      result.heatMap[y + dir] |= (0x1 << (x + 1));
	    }
	  }
	  if (x > 0)
	  {
	    if (board[y + dir][x - 1] == (Piece::blackPawn ^ (state & blackToMoveMask)))
	    {
  	    result.len++;
	      result.heatMap[y + dir] |= (0x1 << (x - 1));
	    }
	  }
	}
	
	return result;
}

/**
 * This function helps the getCheck function to investigate the influence of one of the 8 directions
 * on the output of the getCheck function.
 */
bool Board::firstPiece(check * result, const square curPos, const square dir, const char friendlies)
{
  signed char newx = curPos.x + dir.x;
  signed char newy = curPos.y + dir.y;
  if (newx >= 0 && newx <= 7 && newy >= 0 && newy <= 7)
  {
    if (board[newx][newy] == Piece::none)
    {
      //Deal with empty squares
      if (firstPiece(result, {newx,newy}, dir, friendlies))
      {
        if (friendlies == 0)
          result->heatMap[newy] |= (0x1 << newx);
        return true;
      }
      return false;
    }
    
    Piece::Piece newpiece = board[newx][newy];
    if (isFriendly(newpiece))
    {
      //Deal with pinned pieces
      if (friendlies == 1)
        return false;
      if (firstPiece(result, {newx,newy}, dir, 1))
        result->heatMap[newy] |= (0x1 << newx);
      return false;
    }
    else
    {
      //Deal with direct attackers
      newpiece = (Piece::Piece)(newpiece & ~blackToMoveMask);
      if (newpiece == Piece::whiteQueen || (newpiece == Piece::whiteRook && dir.x * dir.y == 0) || (newpiece == Piece::whiteBishop && dir.x * dir.y != 0))
      {
        if (friendlies == 0)
        {
          result->heatMap[newy] |= (0x1 << newx);
          result->len++;
        }
        return true;
      }
    }
  }
  return false;
}

bool Board::hasAttacker(square pos, const square dir) {
  // TODO: implementation
  //const unsigned char distx = dir.x>0? 7-pos.x : pos.x;
  //const unsigned char disty = dir.y>0? 7-pos.y : pos.y;
  return false;
}

bool Board::isFriendly(const square pos) {
  return !((state ^ board[pos.x][pos.y]) & blackToMoveMask);
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
            << ((state && blackToMoveMask) ? "black" : "white")
            << " to move"
            << std::endl
            << std::endl;

  // Castle details
  std::cout << "CASTLE| queenside kingside"
            << std::endl
            << " White|      "
            << ((state && whiteCastleQueensideMask) ? "1" : "0")
            << "        "
            << ((state && whiteCastleKingsideMask) ? "1" : "0")
            << std::endl

            << " Black|      "
            << ((state && blackCastleQueensideMask) ? "1" : "0")
            << "        "
            << ((state && blackCastleKingsideMask) ? "1" : "0")
            << std::endl;

  // enPassant
  if(enPassant>=0){
    std::cout << "En passant on file: "<< char('a'+enPassant);
  }

}
