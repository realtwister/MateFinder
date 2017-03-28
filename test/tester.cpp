#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#define DEBUG
#include "../src/Board.h"
#include <iostream>

TEST_CASE("Starting check position") {
  Board b;

  SUBCASE("test getters") {
    for (signed char i = 0; i < 8; i++) {
      CHECK(b.getSquare({ i, 1 }) == 'P');
      CHECK(b.getSquare({ i, 6 }) == 'p');
    }
  }

  SUBCASE("isFriendly"){
    for (signed char i = 0; i < 8; i++) {
      CHECK(b.isFriendly({ i, 1 }));
      CHECK(!b.isFriendly({ i, 6 }));
    }
  }
}

TEST_CASE("Testing getCheck function")
{
  Board c;
  for (int i = 0; i < 64; i++) {c.board[i / 8][i % 8] = Piece::none;}
  
  SUBCASE("Orindary check on empty board")
  {
    //King can be placed anywhere
    for (int h = 0; h < 64; h++)
    {
      signed char xking = h / 8;
      signed char yking = h % 8;
      
      //Other piece can be placed anywhere
      for (int i = 0; i < 64; i++)
      {
        //Toggle between who is to move
        for (char move = 0; move <= 1; move++)
        {
          //Set up the board except for the other piece
          for (int j = 0; j < 64; j++) {c.board[j / 8][j % 8] = Piece::none;}
          if (move != 0) {c.state |= Board::blackToMoveMask;}
          else {c.state &= (char)~Board::blackToMoveMask;}
          c.board[xking][yking] = (move == 0 ? Piece::whiteKing : Piece::blackKing);
          
          //Calculate location of the other piece
          signed char x = i % 8;
          signed char y = i / 8;
          if (x == xking && y == yking) {continue;}
          signed char diffx = abs(x - xking);
          signed char diffy = abs(y - yking);
          signed char dir = 2 * move - 1;
          
          //Loop through all the pieces
          for (unsigned char j = 0; j < sizeof(Piece::blackPieces) / sizeof(Piece::Piece); j++)
          {
            Piece::Piece piece = Piece::blackPieces[j];
            if (((y == 0 || y == 7) && piece == Piece::blackPawn) || piece == Piece::blackKing) {continue;}
            
            c.board[x][y] = (Piece::Piece)(piece ^ (c.state & Board::blackToMoveMask));
            check result = c.getCheck();
            
            //Check for check
            if (((diffx == 0 || diffy == 0) && (piece == Piece::blackQueen || piece == Piece::blackRook))
                || (diffx == diffy && (piece == Piece::blackQueen || piece == Piece::blackBishop))
                || (piece == Piece::blackKnight && diffx + diffy == 3 && diffx != 0 && diffy != 0)
                || (piece == Piece::blackPawn && diffx == 1 && yking - y == dir))
            {
              CHECK((c.state & Board::checkMask) == Board::checkMask);
              CHECK(result.len == 1);
              for (signed char newy = 0; newy < 8; newy++)
                for (signed char newx = 0; newx < 8; newx++)
                  if (piece == Piece::blackKnight || piece == Piece::blackPawn)
                  {
                    if (newx == x && newy == y)
                      CHECK(((result.heatMap[newy] >> newx) & 0x1) == 1);
                    else
                      CHECK(((result.heatMap[newy] >> newx) & 0x1) == 0);
                  }
                  else if (diffy == 0)
                  {
                    if (((newx >= x && newx < xking) || (newx > xking && newx <= x)) && newy == y)
                      CHECK(((result.heatMap[newy] >> newx) & 0x1) == 1);
                    else
                      CHECK(((result.heatMap[newy] >> newx) & 0x1) == 0);
                  }
                  else if (diffx == 0)
                  {
                    if (((newy >= y && newy < yking) || (newy > yking && newy <= y)) && newx == x)
                      CHECK(((result.heatMap[newy] >> newx) & 0x1) == 1);
                    else
                      CHECK(((result.heatMap[newy] >> newx) & 0x1) == 0);
                  }
                  else
                  {
                    if ((abs(newx - xking) == abs(newy - yking)) && ((newx >= x && newx < xking) || (newx > xking && newx <= x)) && ((newy >= y && newy < yking) || (newy > yking && newy <= y)))
                      CHECK(((result.heatMap[newy] >> newx) & 0x1) == 1);
                    else
                      CHECK(((result.heatMap[newy] >> newx) & 0x1) == 0);
                  }
            }
            else
            {
              CHECK((c.state & Board::checkMask) == 0);
              CHECK(result.len == 0);
              for (signed char y = 0; y < 8; y++)
                CHECK(result.heatMap[y] == 0);
            }
          }
        }
      }
    }
  }
  
  SUBCASE("Blockades and pinned pieces")
  {
    for (int h = 0; h < 64; h++)
    {
      signed char xking = h / 8;
      signed char yking = h % 8;
      
      for (int i = 0; i < 64; i++)
      {
        if (i == h) {continue;}
        signed char x = i / 8;
        signed char y = i % 8;
        
        for (int j = 0; j < 64; j++) c.board[j / 8][j % 8] = Piece::none;
        c.board[xking][yking] = Piece::whiteKing;
        c.board[x][y] = Piece::blackQueen;
        check result = c.getCheck();
        if (result.len != 0)
        {
          signed char diffx = abs(xking - x);
          signed char diffy = abs(yking - y);
          if (diffx <= 1 && diffy <= 1) {continue;}
          signed char newx,newy;
          if (diffx == 0)
          {
            newx = xking;
            newy = yking + (y - yking) / abs(y - yking);
          }
          else if (diffy == 0)
          {
            newx = xking + (x - xking) / abs(x - xking);
            newy = yking;
          }
          else
          {
            newx = xking + (x - xking) / abs(x - xking);
            newy = yking + (y - yking) / abs(y - yking);
          }
          c.board[newx][newy] = Piece::whiteBishop;
          result = c.getCheck();
          CHECK(((result.heatMap[newy] >> newx) & 0x1) == 1);
          CHECK((c.state & Board::checkMask) == 0);
          c.board[newx][newy] = Piece::blackKnight;
          result = c.getCheck();
          CHECK(((result.heatMap[newy] >> newx) & 0x1) == 0);
          CHECK((c.state & Board::checkMask) == 0);
        }
      }
    }
  }
}
