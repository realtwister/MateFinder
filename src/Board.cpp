#include "Board.h"
#include <iostream>
#include <fstream>
#include <exception>
#include <iostream>
#include <cmath>
#include <tgmath.h>

static square<int> knightMoves[8]={
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

bool Board::isAttacked(const square<int> piecePos) {
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
  square<int> pos;
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
 * a possible square<int> to resolve check if the number of attackers is one and the square<int> is empty,
 * or a pinned piece if it occupied by a friendly piece. Moreover, it updates the check flag.
 */
check Board::getCheck(const square<int> kingPos)
{
	check result = {0,{{0}}};
	state &= ~checkMask;

	//Find out if the king is under attack by some long range piece
	int xmin = (kingPos.x == 0 ? 0 : -1);
	int xmax = (kingPos.x == 7 ? 0 : 1);
	int ymin = (kingPos.y == 0 ? 0 : -1);
	int ymax = (kingPos.y == 7 ? 0 : 1);
	int dirx,diry;
	for (dirx = xmin; dirx <= xmax; dirx++)
	  for (diry = ymin; diry <= ymax; diry++)
    {
      if (dirx != 0 || diry != 0);
      if (firstPiece(result, kingPos, {dirx,diry}, 0)) state |= checkMask;
    }

  //Find out if the king is under attack by a knight
  square<int> newPos;
  for(int i=0; i<8; i++)
  {
    newPos = kingPos + knightMoves[i];
    if (newPos.x >= 0 && newPos.x <= 7 && newPos.y >= 0 && newPos.y <= 7 && board[newPos.x][newPos.y] == (Piece::blackKnight ^ (state & blackToMoveMask)))
    {
      result.len++;
      result.heatMap[newPos.x][newPos.y] = 1;
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
        result.heatMap[kingPos.x + 1][kingPos.y + dir] = 1;
        state |= checkMask;
      }
    }
    if (kingPos.x > 0)
    {
      if (board[kingPos.x - 1][kingPos.y + dir] == (Piece::blackPawn ^ (state & blackToMoveMask)))
      {
  	    result.len++;
        result.heatMap[kingPos.x - 1][kingPos.y + dir] = 1;
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
bool Board::firstPiece(check & result, const square<int> curPos, const square<int> dir, const int friendlies)
{
	square<int> newsq = curPos + dir;
  if (newsq.x >= 0 && newsq.x <= 7 && newsq.y >= 0 && newsq.y <= 7)
  {
    if (board[newsq.x][newsq.y] == Piece::none)
    {
      //Deal with empty square<int>s
      if (firstPiece(result, newsq, dir, friendlies))
      {
        if (friendlies == 0)
          result.heatMap[newsq.x][newsq.y] = 1;
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
      {
        result.heatMap[newsq.x][newsq.y] = 1;
        if (dir.x * dir.y == -1) result.heatMap[newsq.x][newsq.y] |= (0x1 << 1);
        else if (dir.x * dir.y == 1) result.heatMap[newsq.x][newsq.y] |= (0x1 << 2);
        else if (dir.x == 0) result.heatMap[newsq.x][newsq.y] |= (0x1 << 3);
        else result.heatMap[newsq.x][newsq.y] |= (0x1 << 4);
      }
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
          result.heatMap[newsq.x][newsq.y] = 1;
          result.len++;
        }
        return true;
      }
    }
  }
  return false;
}

bool Board::hasAttacker(square<int> pos, const square<int> dir) {
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

/**
 * This function calculates all possible moves, and stores them in the moveArray moves.
 * Along the way, the check state flag is updated.
 */

void Board::calcMoves()
{
  //First we allocate space for the friendly squares array and the array of moves
  legalMoves.clear();
  legalMoves.reserve(256);
  square<int> * friendlySquares = new square<int>[15];
  int friendlySquaresCtr = 0;
  
  //We set up some counters to check if we have reached a draw
  int friendlyCtr = 0;
  Piece::Piece lastFriendly = Piece::none;
  int enemyCtr = 0;
  Piece::Piece lastEnemy = Piece::none;
  state &= (char)(~drawMask);
  
  //Next, we determine the position of the king
  int globalCounter;
  check kingEnv;
  square<int> kingPos;
  for (globalCounter = 0; globalCounter < 64; globalCounter++)
  {
    kingPos.x = globalCounter / 8;
    kingPos.y = globalCounter % 8;
    if (board[kingPos.x][kingPos.y] == (Piece::whiteKing ^ (state & blackToMoveMask)))
    {
      kingEnv = getCheck(kingPos);
      break;
    }
    if (board[kingPos.x][kingPos.y] != Piece::none)
    {
      if (isFriendly(kingPos))
      {
        friendlySquares[friendlySquaresCtr++] = kingPos;
        friendlyCtr++;
        lastFriendly = board[kingPos.x][kingPos.y];
      }
      else if (board[kingPos.x][kingPos.y] != (Piece::blackKing ^ (state & blackToMoveMask)))
      {
        enemyCtr++;
        lastEnemy = board[kingPos.x][kingPos.y];
      }
    }
  }
  
  //Then, we determine the possible moves of the king
  int xmin = (kingPos.x == 0 ? 0 : -1);
  int xmax = (kingPos.x == 7 ? 0 : 1);
  int ymin = (kingPos.y == 0 ? 0 : -1);
  int ymax = (kingPos.y == 7 ? 0 : 1);
  
  board[kingPos.x][kingPos.y] = Piece::none;
  square<int> dir;
  for (dir.x = xmin; dir.x <= xmax; dir.x++)
    for (dir.y = ymin; dir.y <= ymax; dir.y++)
      if ((dir.x != 0 || dir.y != 0) && !isFriendly(kingPos + dir) && !isAttacked(kingPos + dir))
        legalMoves.push_back({kingPos, kingPos + dir});
  board[kingPos.x][kingPos.y] = (Piece::Piece)((char)Piece::whiteKing | (char)(state & blackToMoveMask));
  
  //We look at castles now
  if (!(state & blackToMoveMask))
  {
  	//White is to move
  	if ((state & whiteCastleKingsideMask) && !(state & checkMask) && board[5][0] == Piece::none && board[6][0] == Piece::none
  	    && !isAttacked({5,0}) && !isAttacked({6,0}))
  	  legalMoves.push_back({{4,0},{6,0}});
  	if ((state & whiteCastleQueensideMask) && !(state & checkMask) && board[3][0] == Piece::none && board[2][0] == Piece::none && board[1][0] == Piece::none
  	    && !isAttacked({3,0}) && !isAttacked({2,0}))
  	  legalMoves.push_back({{4,0},{2,0}});
  }
  else
  {
    //Black is to move
    if ((state & blackCastleKingsideMask) && !(state & checkMask) && board[5][7] == Piece::none && board[6][7] == Piece::none
        && !isAttacked({5,7}) && !isAttacked({6,7}))
      legalMoves.push_back({{4,7},{6,7}});
    if ((state & blackCastleQueensideMask) && !(state & checkMask) && board[3][7] == Piece::none && board[2][7] == Piece::none&& board[1][7] == Piece::none
        && !isAttacked({3,7}) && !isAttacked({2,7}))
      legalMoves.push_back({{4,7},{2,7}});
  }
  
  //We can only move other pieces if at most one piece is attacking the king
  if (kingEnv.len <= 1)
  {
    //Now, loop over all the square<int>s in the friendlySquares array
    for (int i = 0; i < friendlySquaresCtr; i++)
      getPieceMoves(legalMoves, kingEnv, friendlySquares[i], kingPos);
    
    //Finally, loop over all remaining square<int>s
    square<int> remaining;
    for (globalCounter++; globalCounter < 64; globalCounter++)
    {
      remaining.x = globalCounter / 8;
      remaining.y = globalCounter % 8;
      if (board[remaining.x][remaining.y] != Piece::none)
      {
        if (isFriendly(remaining))
        {
          getPieceMoves(legalMoves, kingEnv, remaining, kingPos);
          friendlyCtr++;
          lastFriendly = board[remaining.x][remaining.y];
        }
        else if (board[remaining.x][remaining.y] != (Piece::blackKing ^ (state & blackToMoveMask)))
        {
          enemyCtr++;
          lastEnemy = board[remaining.x][remaining.y];
        }
      }
    }
  }
  
  //Determine if we have a draw
  if (friendlyCtr + enemyCtr <= 1
      && (friendlyCtr == 0 || (friendlyCtr == 1 && ((lastFriendly & (char)(~0x20)) == Piece::whiteKnight || (lastFriendly & (char)(~0x20)) == Piece::whiteBishop)))
      && (enemyCtr == 0 || (enemyCtr == 1 && ((lastEnemy & (char)(~0x20)) == Piece::whiteKnight || (lastEnemy & (char)(~0x20)) == Piece::whiteBishop))))
    state |= drawMask;
  
  //Deallocate the temporary variables
  legalMoves.shrink_to_fit();
  delete[] friendlySquares;
}

void Board::getPieceMoves(std::vector<move> & result, const check & kingEnv, const square<int> curPos, const square<int> kingPos)
{
  int xmin, xmax, ymin, ymax, dir;
  square<int> newPos;
  switch (board[curPos.x][curPos.y] & (char)~0x20)
  {
    case Piece::whiteQueen:
      xmin = (curPos.x == 0 ? 0 : -1);
      xmax = (curPos.x == 7 ? 0 : 1);
      ymin = (curPos.y == 0 ? 0 : -1);
      ymax = (curPos.y == 7 ? 0 : 1);
      
      for (int dirx = xmin; dirx <= xmax; dirx++)
        for (int diry = ymin; diry <= ymax; diry++)
          if (dirx != 0 || diry != 0)
            if (kingEnv.heatMap[curPos.x][curPos.y] == 0
                || (dirx * diry == -1 && kingEnv.heatMap[curPos.x][curPos.y] & 0x2)
                || (dirx * diry == 1 && kingEnv.heatMap[curPos.x][curPos.y] & 0x4)
                || (dirx == 0 && kingEnv.heatMap[curPos.x][curPos.y] & 0x8)
                || (diry == 0 && kingEnv.heatMap[curPos.x][curPos.y] & 0x10))
              checkDir(result, kingEnv, curPos, curPos, {dirx,diry});
      break;
    
    case Piece::whiteRook:
      xmin = (curPos.x == 0 ? 1 : -1);
      xmax = (curPos.x == 7 ? -1 : 1);
      ymin = (curPos.y == 0 ? 1 : -1);
      ymax = (curPos.y == 7 ? -1 : 1);
      
      if (kingEnv.heatMap[curPos.x][curPos.y] == 0 || kingEnv.heatMap[curPos.x][curPos.y] & 0x10)
        for (int dirx = xmin; dirx <= xmax; dirx += 2)
          checkDir(result, kingEnv, curPos, curPos, {dirx,0});
      if (kingEnv.heatMap[curPos.x][curPos.y] == 0 || kingEnv.heatMap[curPos.x][curPos.y] & 0x8)
        for (int diry = ymin; diry <= ymax; diry += 2)
          checkDir(result, kingEnv, curPos, curPos, {0,diry});
      break;
    
    case Piece::whiteBishop:
      xmin = (curPos.x == 0 ? 1 : -1);
      xmax = (curPos.x == 7 ? -1 : 1);
      ymin = (curPos.y == 0 ? 1 : -1);
      ymax = (curPos.y == 7 ? -1 : 1);
      
      for (int dirx = xmin; dirx <= xmax; dirx += 2)
        for (int diry = ymin; diry <= ymax; diry += 2)
          if (kingEnv.heatMap[curPos.x][curPos.y] == 0
              || (dirx * diry == -1 && kingEnv.heatMap[curPos.x][curPos.y] & 0x2)
              || (dirx * diry == 1 && kingEnv.heatMap[curPos.x][curPos.y] & 0x4))
            checkDir(result, kingEnv, curPos, curPos, {dirx,diry});
      break;
    
    case Piece::whiteKnight:
      for(int i=0; i<8; i++)
      {
        newPos = curPos + knightMoves[i];
        if (newPos.x >= 0 && newPos.x <= 7 && newPos.y >= 0 && newPos.y <= 7 && !isFriendly(newPos)
            && kingEnv.heatMap[curPos.x][curPos.y] == 0 && (!(state & checkMask) || kingEnv.heatMap[newPos.x][newPos.y] != 0))
          result.push_back({curPos, newPos});
      }
      break;
      
    case Piece::whitePawn:
      dir = 1 - 2 * (int)((state & blackToMoveMask) >> 5);
      xmin = (curPos.x == 0 ? 0 : -1);
      xmax = (curPos.x == 7 ? 0 : 1);
      for (int diffx = xmin; diffx <= xmax; diffx++)
      {
        if (diffx == 0)
        {
          if (board[curPos.x][curPos.y + dir] == Piece::none && (kingEnv.heatMap[curPos.x][curPos.y] == 0 || kingEnv.heatMap[curPos.x][curPos.y] & 0x8))
          {
          	if (!(state & checkMask) || kingEnv.heatMap[curPos.x][curPos.y+dir] != 0)
          	{
              if ((curPos.y + dir) % 7 == 0)
              {
                result.push_back({curPos,{curPos.x,curPos.y+dir},(state & blackToMoveMask ? Piece::blackQueen : Piece::whiteQueen)});
                result.push_back({curPos,{curPos.x,curPos.y+dir},(state & blackToMoveMask ? Piece::blackRook : Piece::whiteRook)});
                result.push_back({curPos,{curPos.x,curPos.y+dir},(state & blackToMoveMask ? Piece::blackBishop : Piece::whiteBishop)});
                result.push_back({curPos,{curPos.x,curPos.y+dir},(state & blackToMoveMask ? Piece::blackKnight : Piece::whiteKnight)});
              }
              else
                result.push_back({curPos,{curPos.x,curPos.y+dir}});
            }
            if ((curPos.y - dir) % 7 == 0 && board[curPos.x][curPos.y+2*dir] == Piece::none && (!(state & checkMask) || kingEnv.heatMap[curPos.x][curPos.y+2*dir] != 0))
              result.push_back({curPos,{curPos.x,curPos.y+2*dir}});
          }
        }
        else
        {
          if ((kingEnv.heatMap[curPos.x][curPos.y] == 0
                   || (dir * diffx == -1 && kingEnv.heatMap[curPos.x][curPos.y] & 0x2)
                   || (dir * diffx == 1 && kingEnv.heatMap[curPos.x][curPos.y] & 0x4))
               && (!(state & checkMask) || kingEnv.heatMap[curPos.x+diffx][curPos.y+dir] != 0))
          {
            if (board[curPos.x + diffx][curPos.y + dir] != Piece::none && !isFriendly({curPos.x + diffx, curPos.y + dir}))
            {
              if ((curPos.y + dir) % 7 == 0)
              {
                result.push_back({curPos,{curPos.x+diffx,curPos.y+dir},(state & blackToMoveMask ? Piece::blackQueen : Piece::whiteQueen)});
                result.push_back({curPos,{curPos.x+diffx,curPos.y+dir},(state & blackToMoveMask ? Piece::blackRook : Piece::whiteRook)});
                result.push_back({curPos,{curPos.x+diffx,curPos.y+dir},(state & blackToMoveMask ? Piece::blackBishop : Piece::whiteBishop)});
                result.push_back({curPos,{curPos.x+diffx,curPos.y+dir},(state & blackToMoveMask ? Piece::blackKnight : Piece::whiteKnight)});
              }
              else
                result.push_back({curPos,{curPos.x+diffx,curPos.y+dir}});
            }
            else if (curPos.y == (dir == 1 ? 4 : 3) && enPassant == curPos.x + diffx)
            {
              board[curPos.x][curPos.y] = Piece::none;
              board[curPos.x + diffx][curPos.y] = Piece::none;
              if (curPos.y != kingPos.y || !isAttacked(kingPos))   //OPTIMIZATION POSSIBLE
                result.push_back({curPos,{curPos.x+diffx,curPos.y+dir}});
              board[curPos.x][curPos.y] = (Piece::Piece)(Piece::whitePawn ^ (state & blackToMoveMask));
              board[curPos.x + diffx][curPos.y] = (Piece::Piece)(Piece::blackPawn ^ (state & blackToMoveMask));
            }
          }
        }
      }
      break;
  }
}

void Board::checkDir(std::vector<move> & result, const check & kingEnv, const square<int> basePos, const square<int> curPos, const square<int> dir)
{
  square<int> newPos = curPos + dir;
  if (newPos.x >= 0 && newPos.y >= 0 && newPos.x <= 7 && newPos.y <= 7 && !isFriendly(newPos))
  {
    if (!(state & checkMask) || kingEnv.heatMap[newPos.x][newPos.y] != 0)
      result.push_back({basePos, newPos});
    if (board[newPos.x][newPos.y] == Piece::none)
      checkDir(result, kingEnv, basePos, newPos, dir);
  }
}

Board::Board(const Board& other, const move mv)
{
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      board[i][j] = other.board[i][j];
  state = other.state;
  execMove(mv);
}

// PUBLIC FUNCTIONS

/**
 * Board constructor creating Board object from either a file or a string
 * @param   str Either the FEN string or the filename
 * @param  file Boolean whether to read file or string (true is file)
 * @return void
 */

Board::Board(const char *str, const bool file)
{
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
  
  calcMoves();
}

void Board::execMove(const move move)
{
  enPassant = -1;
  Piece::Piece piece = board[move.start.x][move.start.y];
  switch (piece & (char)(~0x20))
  {
    case Piece::whitePawn:
      if (abs(move.start.x - move.end.x) == 1 && abs(move.start.y - move.end.y) == 1 && board[move.end.x][move.end.y] == Piece::none)
        board[move.end.x][move.start.y] = Piece::none;
      board[move.end.x][move.end.y] = (move.end.y % 7 == 0 ? move.promoteTo : piece);
      board[move.start.x][move.start.y] = Piece::none;
      if (abs(move.start.y - move.end.y) == 2) enPassant = move.start.x;
      break;
    
    case Piece::whiteRook:
      if (move.start.x == 0)
      {
        if (state & blackToMoveMask) {state &= (char)(~blackCastleQueensideMask);}
        else {state &= (char)(~whiteCastleQueensideMask);}
      }
      else if (move.start.x == 7)
      {
        if (state & blackToMoveMask) {state &= (char)(~blackCastleKingsideMask);}
        else {state &= (char)(~whiteCastleKingsideMask);}
      }
      board[move.end.x][move.end.y] = board[move.start.x][move.start.y];
      board[move.start.x][move.start.y] = Piece::none;
      break;
      
    case Piece::whiteKing:
      if (move.start.x == 4 && move.end.x == 6) {board[5][move.start.y] = board[7][move.start.y]; board[7][move.start.y] = Piece::none;}
      else if (move.start.x == 4 && move.end.x == 2) {board[3][move.start.y] = board[0][move.start.y]; board[0][move.start.y] = Piece::none;}
      if (state & blackToMoveMask) {state &= (char)(~(blackCastleKingsideMask | blackCastleQueensideMask));}
      else {state &= (char)(~(whiteCastleKingsideMask | whiteCastleQueensideMask));}
    default:
      board[move.end.x][move.end.y] = board[move.start.x][move.start.y];
      board[move.start.x][move.start.y] = Piece::none;
      break;
  }
  state ^= blackToMoveMask;
  calcMoves();
}

Board Board::cloneAndExecMove(const move mv)
{
  return Board(*this, mv);
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
    std::cout << "En passant on file: " << char('a' + enPassant) << std::endl;
  }
}

void Board::printLegalMoves()
{
  std::cout << "Below, we will show a list of legal moves." << std::endl;
  std::cout << "The current position is depicted below." << std::endl;
  printBoard();
  std::cout << "There are " << legalMoves.size() << " legal moves." << std::endl;
  for (unsigned int i = 0; i < legalMoves.size(); i++)
  {
    std::cout << (i+1) << ": " << (char)(legalMoves[i].start.x + 'a') << (legalMoves[i].start.y + 1)
              << "-" << (char)(legalMoves[i].end.x + 'a') << (legalMoves[i].end.y + 1) << std::endl;
  }
}
