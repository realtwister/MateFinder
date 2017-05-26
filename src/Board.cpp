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

/**
 * Initialize a Board object from a string in FEN notation.
 * @param[in] str The string in FEN notation
 * @return This function returns an error code, according to the following table:
 * Code | Description
 * ---: | -----------
 * 0    | Success.
 * 1    | The syntax of str is wrong.
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

  if (state & blackToMoveMask) {flipBoard();}
  return 0;
}

/**
 * Initialize a Board object from a file containing a string in FEN notation.
 * @param fileName The name of the file.
 * @return This function returns an error code, according to the following table:
 * Code | Description
 * ---: | -----------
 * 0 | Success.
 * 1 | The syntax of str is wrong.
 */
int Board::fromFile(const char *fileName) {
  std::ifstream file;

  file.open(fileName);

  if (!file.good()) return 2;  // Error reading file

  char str[255];
  file.getline(str, 255);
  return fromStr(str);
}

bool Board::isAttacked(const square<int> piecePos) const {
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
    if ((pos.x<8 and pos.y<8 and pos.x>=0 and pos.y>=0) and (board[pos.x][pos.y] == Piece::blackKnight)) return true;
  }
  return false;
}

/**
 * This function finds out what is going on in relation to the king of the player that is to move.
 * In particular, it returns a check struct containing information about pinned pieces and squares that
 * can resolve check. Moreover, it updates the check flag.
 * @param[in] kingPos The position of the king.
 * @return This function returns a check object, containing information about pinned pieces and, in case
 * of check, how many pieces are attacking the king, and, in case of 1 attacking piece, which squares will
 * resolve the check.
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
    if (newPos.x >= 0 && newPos.x <= 7 && newPos.y >= 0 && newPos.y <= 7 && board[newPos.x][newPos.y] == Piece::blackKnight)
    {
      result.len++;
      result.heatMap[newPos.x][newPos.y] = 1;
      state |= checkMask;
    }
  }

	//Find out if the king is under attack by a pawn
  if (kingPos.y + 1 < 8 && kingPos.y + 1 >= 0)
  {
    if (kingPos.x < 7)
    {
      if (board[kingPos.x + 1][kingPos.y + 1] == Piece::blackPawn)
      {
        result.len++;
        result.heatMap[kingPos.x + 1][kingPos.y + 1] = 1;
        state |= checkMask;
      }
    }
    if (kingPos.x > 0)
    {
      if (board[kingPos.x - 1][kingPos.y + 1] == Piece::blackPawn)
      {
  	    result.len++;
        result.heatMap[kingPos.x - 1][kingPos.y + 1] = 1;
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
 * @param[out] result The check struct that getCheck is working on.
 * @param[in] curPos The current position that is being investigated.
 * @param[in] dir The direction in which is being checked whether there are attacks going on.
 * @param[in] friendlies This value determines the number of friendly pieces that were already encountered.
 * If this is 1, and an attacking piece is encountered, then \a result will indicate that this piece is pinned.
 * @return This function returns a boolean. True is returned, if the square indicated by \a curPos is under attack
 * from the direction indicated by \a dir. Otherwise, false is returned.
 */
bool Board::firstPiece(check & result, const square<int> curPos, const square<int> dir, const int friendlies) const
{
  square<int> newsq = curPos + dir;
  if (newsq.x >= 0 && newsq.x <= 7 && newsq.y >= 0 && newsq.y <= 7)
  {
    if (board[newsq.x][newsq.y] == Piece::none)
    {
      //Deal with empty squares
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
        result.heatMap[newsq.x][newsq.y] = 2 + dir.x * dir.y + 2 * (dir.x == 0);
      return false;
    }
    else
    {
      //Deal with direct attackers
      Piece::Piece piece = board[newsq.x][newsq.y];
      if (piece == Piece::blackQueen || (piece == Piece::blackRook && dir.x * dir.y == 0) || (piece == Piece::blackBishop && dir.x * dir.y != 0))
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

/**
 * This function helps the isAttacked function to determine whether an attack over a long distance
 * from a fixed direction occurs.
 * @param[in] pos The position of which is determined whether it is under attack.
 * @param[in] dir The direction in which is being checked whether there is an attacker.
 * @return This function returns a boolean, indicating whether there is an emeny piece attacking the square \a pos from direction \a dir.
 */
bool Board::hasAttacker(square<int> pos, const square<int> dir) const {
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
          return dir.y == 1;

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
 * This function calculates all possible moves, and stores them in \a legalMoves member.
 * Additionally, it updates the \a check flag, to indicate whether the player that is to move is check.
 * Finally, it also updates the \a draw flag. If there are too few pieces on the board for any mate to occur, then this flag is set.
 */

void Board::calcMoves()
{
  //First, we allocate space for the friendly squares array
  moveArray tempLegalMoves(256);
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
    if (board[kingPos.x][kingPos.y] == Piece::whiteKing)
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
      else if (board[kingPos.x][kingPos.y] != Piece::blackKing)
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
        tempLegalMoves.push_back({kingPos, kingPos + dir});
  board[kingPos.x][kingPos.y] = Piece::whiteKing;

  //We look at castles now
  if (!(state & checkMask))
  {
    if ((state & whiteCastleKingsideMask) && board[5][0] == Piece::none && board[6][0] == Piece::none
        && !isAttacked({5,0}) && !isAttacked({6,0}))
      tempLegalMoves.push_back({{4,0},{6,0}});
    if ((state & whiteCastleQueensideMask) && board[3][0] == Piece::none && board[2][0] == Piece::none && board[1][0] == Piece::none
        && !isAttacked({3,0}) && !isAttacked({2,0}))
      tempLegalMoves.push_back({{4,0},{2,0}});
  }

  //We can only move other pieces if at most one piece is attacking the king
  if (kingEnv.len <= 1)
  {
    //Now, loop over all the squares in the friendlySquares array
    for (int i = 0; i < friendlySquaresCtr; i++)
      getPieceMoves(tempLegalMoves, kingEnv, friendlySquares[i], kingPos);

    //Finally, loop over all remaining squares
    square<int> remaining;
    for (globalCounter++; globalCounter < 64; globalCounter++)
    {
      remaining.x = globalCounter / 8;
      remaining.y = globalCounter % 8;
      if (board[remaining.x][remaining.y] != Piece::none)
      {
        if (isFriendly(remaining))
        {
          getPieceMoves(tempLegalMoves, kingEnv, remaining, kingPos);
          friendlyCtr++;
          lastFriendly = board[remaining.x][remaining.y];
        }
        else if (board[remaining.x][remaining.y] != Piece::blackKing)
        {
          enemyCtr++;
          lastEnemy = board[remaining.x][remaining.y];
        }
      }
    }
  }

  
  //Store the final movearray
  legalMoves = tempLegalMoves.shrink_to_fit();

  //Determine if we have a draw
  if ((legalMoves.size() == 0 && !(state & checkMask))
      || (friendlyCtr + enemyCtr <= 1
          && (friendlyCtr == 0 || (friendlyCtr == 1 && (lastFriendly == Piece::whiteKnight || lastFriendly == Piece::whiteBishop)))
          && (enemyCtr == 0 || (enemyCtr == 1 && (lastEnemy == Piece::blackKnight || lastEnemy == Piece::blackBishop)))))
    state |= drawMask;

  //Deallocate the temporary variables
  delete[] friendlySquares;
}

/**
 * This function helps the calcMoves function. It investigates the legal moves that one particular piece can make,
 * and appends these to the temporary moves array created by calcMoves.
 * @param[out] result This is the array that the newly found moves are being appended to.
 * @param[in] kingEnv The function uses this data structure to check whether the piece is pinned, or helps resolving a check.
 * @param[in] curPos This is the square where the piece, whose legal moves are begin investigated, is located.
 * @param[in] kingPos This is the square where the king of the player who is to move is located. Its value is used to speed up special cases of taking en passant.
 */
void Board::getPieceMoves(moveArray& result, const check & kingEnv, const square<int> curPos, const square<int> kingPos)
{
  int xmin, xmax, ymin, ymax;
  square<int> newPos, dir;
  switch ((char)board[curPos.x][curPos.y])
  {
    case Piece::whiteQueen:
      xmin = (curPos.x == 0 ? 0 : -1);
      xmax = (curPos.x == 7 ? 0 : 1);
      ymin = (curPos.y == 0 ? 0 : -1);
      ymax = (curPos.y == 7 ? 0 : 1);

      for (dir.x = xmin; dir.x <= xmax; dir.x++)
        for (dir.y = ymin; dir.y <= ymax; dir.y++)
          if ((dir.x != 0 || dir.y != 0) && (kingEnv.heatMap[curPos.x][curPos.y] == 0 || kingEnv.heatMap[curPos.x][curPos.y] == 2 + dir.x * dir.y + 2 * (dir.x == 0)))
            checkDir(result, kingEnv, curPos, dir);
      break;

    case Piece::whiteRook:
      xmin = (curPos.x == 0 ? 1 : -1);
      xmax = (curPos.x == 7 ? -1 : 1);
      ymin = (curPos.y == 0 ? 1 : -1);
      ymax = (curPos.y == 7 ? -1 : 1);

      dir.y = 0;
      if (kingEnv.heatMap[curPos.x][curPos.y] == 0 || kingEnv.heatMap[curPos.x][curPos.y] == 2)
        for (dir.x = xmin; dir.x <= xmax; dir.x += 2)
          checkDir(result, kingEnv, curPos, dir);
      dir.x = 0;
      if (kingEnv.heatMap[curPos.x][curPos.y] == 0 || kingEnv.heatMap[curPos.x][curPos.y] == 4)
        for (dir.y = ymin; dir.y <= ymax; dir.y += 2)
          checkDir(result, kingEnv, curPos, dir);
      break;

    case Piece::whiteBishop:
      xmin = (curPos.x == 0 ? 1 : -1);
      xmax = (curPos.x == 7 ? -1 : 1);
      ymin = (curPos.y == 0 ? 1 : -1);
      ymax = (curPos.y == 7 ? -1 : 1);

      for (dir.x = xmin; dir.x <= xmax; dir.x += 2)
        for (dir.y = ymin; dir.y <= ymax; dir.y += 2)
          if (kingEnv.heatMap[curPos.x][curPos.y] == 0 || kingEnv.heatMap[curPos.x][curPos.y] == 2 + dir.x * dir.y + 2 * (dir.x == 0))
            checkDir(result, kingEnv, curPos, dir);
      break;

    case Piece::whiteKnight:
      for (int i = 0; i < 8; i++)
      {
        newPos = curPos + knightMoves[i];
        if (newPos.x >= 0 && newPos.x <= 7 && newPos.y >= 0 && newPos.y <= 7 && !isFriendly(newPos)
            && kingEnv.heatMap[curPos.x][curPos.y] == 0 && (!(state & checkMask) || kingEnv.heatMap[newPos.x][newPos.y] != 0))
          result.push_back({curPos, newPos});
      }
      break;

    case Piece::whitePawn:
      //Check straight ahead
      if (board[curPos.x][curPos.y + 1] == Piece::none && (kingEnv.heatMap[curPos.x][curPos.y] == 0 || kingEnv.heatMap[curPos.x][curPos.y] == 4))
      {
      	if (!(state & checkMask) || kingEnv.heatMap[curPos.x][curPos.y+1] != 0)
      	{
          if (curPos.y == 6)
          {
            result.push_back({curPos,{curPos.x,curPos.y+1},Piece::whiteQueen});
            result.push_back({curPos,{curPos.x,curPos.y+1},Piece::whiteRook});
            result.push_back({curPos,{curPos.x,curPos.y+1},Piece::whiteBishop});
            result.push_back({curPos,{curPos.x,curPos.y+1},Piece::whiteKnight});
          }
          else
            result.push_back({curPos,{curPos.x,curPos.y+1}});
        }
        if (curPos.y == 1 && board[curPos.x][3] == Piece::none && (!(state & checkMask) || kingEnv.heatMap[curPos.x][3] != 0))
          result.push_back({curPos,{curPos.x,3}});
      }

      //Check taking
      xmin = (curPos.x == 0 ? 1 : -1);
      xmax = (curPos.x == 7 ? -1 : 1);
      for (int diffx = xmin; diffx <= xmax; diffx += 2)
      {
        if ((kingEnv.heatMap[curPos.x][curPos.y] == 0 || kingEnv.heatMap[curPos.x][curPos.y] == 2 + diffx)
             && (!(state & checkMask) || kingEnv.heatMap[curPos.x+diffx][curPos.y+1] != 0))
        {
          if (board[curPos.x + diffx][curPos.y + 1] != Piece::none && !isFriendly({curPos.x + diffx, curPos.y + 1}))
          {
            if (curPos.y == 6)
            {
              result.push_back({curPos,{curPos.x+diffx,curPos.y+1},Piece::whiteQueen});
              result.push_back({curPos,{curPos.x+diffx,curPos.y+1},Piece::whiteRook});
              result.push_back({curPos,{curPos.x+diffx,curPos.y+1},Piece::whiteBishop});
              result.push_back({curPos,{curPos.x+diffx,curPos.y+1},Piece::whiteKnight});
            }
            else
              result.push_back({curPos,{curPos.x+diffx,curPos.y+1}});
          }
          else if (curPos.y == 4 && enPassant == curPos.x + diffx)
          {
            if (curPos.y != kingPos.y)
       		    result.push_back({curPos,{curPos.x+diffx,curPos.y+1}});
       		  else
       		  {
              board[curPos.x][curPos.y] = Piece::none;
              board[curPos.x + diffx][curPos.y] = Piece::none;
              if (!hasAttacker(kingPos, {(curPos.x < kingPos.x ? -1 : 1),0}))
                result.push_back({curPos,{curPos.x+diffx,curPos.y+1}});
              board[curPos.x][curPos.y] = Piece::whitePawn;
              board[curPos.x + diffx][curPos.y] = Piece::blackPawn;
            }
          }
        }
      }
      break;
  }
}

/**
 * This function helps the getPieceMoves function. It checks to which squares a long range piece (i.e. queen, rook or bishop) can move in a given direction,
 * and appends these moves to the array of legal moves.
 * @param[out] result This is the array of moves which the found legal moves are appended to.
 * @param[in] kingEnv Through this \a check object, the information concerning whether a given move resolves check is supplied to the function.
 * @param[in] basePos This is the position of the piece whose legal moves are investigated.
 * @param[in] dir This is the direction in which the legal moves are being checked.
 */
inline void Board::checkDir(moveArray& result, const check & kingEnv, const square<int> basePos, const square<int> dir) const
{
  int dist;
  if (dir.x == 0) dist = (dir.y >= 0 ? 7 - basePos.y : basePos.y);
  else if (dir.y == 0) dist = (dir.x >= 0 ? 7 - basePos.x : basePos.x);
  else dist = fmin(dir.y >= 0 ? 7 - basePos.y : basePos.y, dir.x >= 0 ? 7 - basePos.x : basePos.x);

  square<int> curPos = basePos;
  for (int i = 0; i < dist; i++)
  {
    curPos += dir;
    if (!isFriendly(curPos) && (!(state & checkMask) || kingEnv.heatMap[curPos.x][curPos.y] != 0))
      result.push_back({basePos, curPos});
    if (board[curPos.x][curPos.y] != Piece::none)
      return;
  }
}

/**
 * This function flips the board. More precisely, it mirrors the position of all pieces along the axis between the 4th and 5th rank.
 * So, for example, a piece on e3 is placed on e6, and a piece on a8 is placed on a1.
 * This function is used to ensure that the player that is to move always plays with its pawns advancing to ever higher ranks.
 */
void Board::flipBoard()
{
  char tmp;
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 4; y++)
    {
      tmp = board[x][y];
      board[x][y] = (Piece::Piece)(board[x][7-y] != Piece::none ? board[x][7-y] ^ (char)0x20 : Piece::none);
      board[x][7-y] = (Piece::Piece)(tmp != Piece::none ? tmp ^ (char)0x20 : Piece::none);
    }

  state = (state & ~(whiteCastleKingsideMask | whiteCastleQueensideMask | blackCastleKingsideMask | blackCastleQueensideMask))
           | ((state & (blackCastleKingsideMask | blackCastleQueensideMask)) >> 2)
           | ((state & (whiteCastleKingsideMask | whiteCastleQueensideMask)) << 2);
  return;
}

/**
 * This constructor is used when a move is executed. All the pieces must be copied, but copying all the legal moves is not necessary, as they are recalculated anyway.
 * So, this private constructur is only to be used internally, when calling the \a cloneAndExecMove function.
 * @params[in] other This is the board that is to be cloned.
 * @params[in] mv This is the move that is to be executed.
 */
Board::Board(const Board& other, const move mv)
{
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      board[i][j] = other.board[i][j];
  state = other.state;
  execMove(mv);
}

/**
 * Board constructor creating Board object from either a file or a string
 * @param str Either the FEN string or the filename
 * @param file Boolean whether to read file or string (true is file)
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
      throw std::invalid_argument("Error initializing board: Syntax error encountered during FEN reading");
      break;

    case 2:
      throw std::invalid_argument("Error initializing board: Error while reading file");
      break;

    default:
      throw std::runtime_error("Error initializing board: Unexpected return value");
    }
  }

  calcMoves();
}

/**
 * Execute a move on a given board and consequently flip the board and recalculate the legal moves.
 * @params[in] move This is the move that is to be executed.
 */

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
        state &= (char)(~whiteCastleQueensideMask);
      else if (move.start.x == 7)
        state &= (char)(~whiteCastleKingsideMask);
      board[move.end.x][move.end.y] = board[move.start.x][move.start.y];
      board[move.start.x][move.start.y] = Piece::none;
      break;

    case Piece::whiteKing:
      if (move.start.x == 4 && move.end.x == 6) {board[5][move.start.y] = board[7][move.start.y]; board[7][move.start.y] = Piece::none;}
      else if (move.start.x == 4 && move.end.x == 2) {board[3][move.start.y] = board[0][move.start.y]; board[0][move.start.y] = Piece::none;}
      state &= (char)(~(whiteCastleKingsideMask | whiteCastleQueensideMask));
    default:
      board[move.end.x][move.end.y] = board[move.start.x][move.start.y];
      board[move.start.x][move.start.y] = Piece::none;
      break;
  }
  state ^= blackToMoveMask;
  flipBoard();
  calcMoves();
}

Board Board::cloneAndExecMove(const move mv) const
{
  return Board(*this, mv);
}

/**
 * Print a formatted representation of the board.
 */
void Board::printBoard() const
{
  Board c = *this;
  if (c.state & blackToMoveMask) {c.flipBoard();}

  // Board
  std::cout << " +------------------------+" << std::endl;

  for (int i = 7; i >= 0; i--) {
    std::cout << i + 1 << '|';

    for (int j = 0; j < 8; j++) {
      std::cout << ' '
                << (char)c.board[j][i]
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
            << ((c.state & blackToMoveMask) ? "black" : "white")
            << " to move"
            << std::endl
            << std::endl;

  // Castle details
  std::cout << "CASTLE| queenside kingside"
            << std::endl
            << " White|      "
            << ((c.state & whiteCastleQueensideMask) ? "1" : "0")
            << "        "
            << ((c.state & whiteCastleKingsideMask) ? "1" : "0")
            << std::endl

            << " Black|      "
            << ((c.state & blackCastleQueensideMask) ? "1" : "0")
            << "        "
            << ((c.state & blackCastleKingsideMask) ? "1" : "0")
            << std::endl;

  // enPassant
  if (c.enPassant >= 0) {
    std::cout << "En passant on file: " << char('a' + c.enPassant) << std::endl;
  }
}

/**
 * Print a list of the legal moves.
 */
void Board::printLegalMoves() const
{
  std::cout << "Below, we will show a list of legal moves." << std::endl;
  std::cout << "The current position is depicted below." << std::endl;
  printBoard();
  std::cout << "There are " << legalMoves.size() << " legal moves." << std::endl;
  for (int i = 0; i < legalMoves.size(); i++)
  {
    std::cout << (i+1) << ": " << (char)(legalMoves[i].start.x + 'a') << ((state & blackToMoveMask ? 7 - legalMoves[i].start.y : legalMoves[i].start.y) + 1)
              << "-" << (char)(legalMoves[i].end.x + 'a') << ((state & blackToMoveMask ? 7 - legalMoves[i].end.y : legalMoves[i].end.y) + 1) << std::endl;
  }
}
