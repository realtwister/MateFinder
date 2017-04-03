#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#define DEBUG
#include "../src/Board.h"
#include "testSquare.cpp"
#include <iostream>

#define BOARD_LOOP for(signed char i=0;i<8;i++) for(signed char j=0;j<8;j++)
#define ROW_LOOP for (signed char i=0;i<8;i++)

TEST_CASE("Start position") {
  Board b;

  SUBCASE("test getters") {
    for (signed char i = 0; i < 8; i++) {
      CHECK(b.getSquare({ i, 1 }) == 'P');
      CHECK(b.getSquare({ i, 6 }) == 'p');
    }
  }

  SUBCASE("Test color get/set"){
    CHECK(!b.blackToMove());
    b.changeColor();
    CHECK(b.blackToMove());
    b.clearBoard();
    for( signed char i =0; i<8;i++)
      for(signed char j=0; j<8;j++){
        CHECK(b.getSquare({i,j}) == Piece::none);
    }
    b.setPiece({3,3},Piece::blackKing);
    CHECK(b.getSquare({3,3}) == Piece::blackKing);
  }

  SUBCASE("isFriendly"){
    for (signed char i = 0; i < 7; i++) {
      CHECK(b.isFriendly({ i, 1 }));
      CHECK(b.isFriendly({ i, 0 }));
      CHECK(!b.isFriendly({ i, 6 }));
      CHECK(!b.isFriendly({ i, 7 }));
    }
    CHECK(!b.isFriendly({1, 3}));
    b.changeColor();
    CHECK(!b.isFriendly({1,3}));
  }

  SUBCASE("hasAttacker"){
    //black pawn
    for (signed char i = 1; i < 7; i++) {
    CHECK(b.hasAttacker({i,5},{1,1}));
    CHECK(b.hasAttacker({i,5},{-1,1}));
    }
    CHECK(b.hasAttacker({0,5},{1,1}));
    CHECK(b.hasAttacker({7,5},{-1,1}));
    //black bishop
    CHECK(b.hasAttacker({1,6},{1,1}));
    CHECK(b.hasAttacker({3,6},{-1,1}));
    CHECK(b.hasAttacker({6,6},{-1,1}));
    CHECK(b.hasAttacker({4,6},{1,1}));
    //black rook
    CHECK(b.hasAttacker({1,7},{-1,0}));
    CHECK(b.hasAttacker({0,6},{0,1}));
    CHECK(b.hasAttacker({6,7},{1,0}));
    CHECK(b.hasAttacker({7,6},{0,1}));

    //black queen
    CHECK(b.hasAttacker({2,7},{1,0}));
    CHECK(b.hasAttacker({2,6},{1,1}));
    CHECK(b.hasAttacker({3,6},{0,1}));
    CHECK(b.hasAttacker({4,6},{-1,1}));
    CHECK(b.hasAttacker({4,7},{-1,0}));

    b.changeColor();
    //white pawn
    for (signed char i = 1; i < 7; i++) {
    CHECK(b.hasAttacker({i,2},{-1,-1}));
    CHECK(b.hasAttacker({i,2},{1,-1}));
    }
    CHECK(b.hasAttacker({7,2},{-1,-1}));
    CHECK(b.hasAttacker({0,2},{1,-1}));
    // white bishop
    CHECK(b.hasAttacker({1,1},{1,-1}));
    CHECK(b.hasAttacker({3,1},{-1,-1}));
    CHECK(b.hasAttacker({6,1},{-1,-1}));
    CHECK(b.hasAttacker({4,1},{1,-1}));

    //black rook
    CHECK(b.hasAttacker({1,0},{-1,0}));
    CHECK(b.hasAttacker({0,1},{0,-1}));
    CHECK(b.hasAttacker({6,0},{1,0}));
    CHECK(b.hasAttacker({7,1},{0,-1}));

    //black queen
    CHECK(b.hasAttacker({2,0},{1,0}));
    CHECK(b.hasAttacker({2,1},{1,-1}));
    CHECK(b.hasAttacker({3,1},{0,-1}));
    CHECK(b.hasAttacker({4,1},{-1,-1}));
    CHECK(b.hasAttacker({4,0},{-1,0}));

    //none
    for(signed char i =0; i<8; i++){
      CHECK(!b.hasAttacker({i,3},{1,0}));
      CHECK(!b.hasAttacker({i,3},{1,1}));
      CHECK(!b.hasAttacker({i,3},{1,-1}));
      CHECK(!b.hasAttacker({i,3},{0,1}));
      CHECK(!b.hasAttacker({i,3},{0,-1}));
      CHECK(!b.hasAttacker({i,3},{1,0}));
      CHECK(!b.hasAttacker({i,3},{1,-1}));
      CHECK(!b.hasAttacker({i,3},{1,1}));
      CHECK(!b.hasAttacker({i,4},{1,0}));
      CHECK(!b.hasAttacker({i,4},{1,1}));
      CHECK(!b.hasAttacker({i,4},{1,-1}));
      CHECK(!b.hasAttacker({i,4},{0,1}));
      CHECK(!b.hasAttacker({i,4},{0,-1}));
      CHECK(!b.hasAttacker({i,4},{1,0}));
      CHECK(!b.hasAttacker({i,4},{1,-1}));
      CHECK(!b.hasAttacker({i,4},{1,1}));

    }
  }
}


TEST_CASE("Check hasAttacker"){
  Board b;
  bool attacks[8][8]={false};
  square dir={};
  b.clearBoard();
  SUBCASE("hasAttacker"){
    SUBCASE("BLACK PAWN LOOP"){
      b.setPiece({3,3}, Piece::blackPawn);
      attacks[2][2]=true;
      attacks[4][2]=true;
    }
    SUBCASE("WHITE PAWN LOOP"){
      b.changeColor();
      b.setPiece({3,3}, Piece::whitePawn);
      attacks[2][4]=true;
      attacks[4][4]=true;
    }
    SUBCASE("BLACK BISHOP"){
      std::cout<<"BLACK BISHOP"<<std::endl;
      b.setPiece({3,3}, Piece::blackBishop);
      ROW_LOOP{
        attacks[i][i]=true;
        if(i<7) attacks[6-i][i]=true;
      }
    }
    SUBCASE("BLACK ROOK"){
      std::cout<<"BLACK ROOK"<<std::endl;
      b.setPiece({3,3}, Piece::blackRook);
      ROW_LOOP{
        attacks[i][3]=true;
        attacks[3][i]=true;
      }
    }
    SUBCASE("BLACK QUEEN"){
      std::cout<<"BLACK QUEEN"<<std::endl;
      b.setPiece({3,3}, Piece::blackQueen);
      ROW_LOOP{
        attacks[i][i]=true;
        if(i<7) attacks[6-i][i]=true;
        attacks[i][3]=true;
        attacks[3][i]=true;
      }
    }
    SUBCASE("BLACK KING"){
      std::cout<<"BLACK KING"<<std::endl;
      b.setPiece({3,3}, Piece::blackKing);
      for(int i=2; i<5; i++)
      for(int j=2;j<5;j++){
        if(i==3 and j==3) continue;
        attacks[i][j]=true;
      }
    }
    BOARD_LOOP{
      if(i==3 and j==3) continue;
      dir.x = i==3?0:(i>3?-1:1);
      dir.y = j==3?0:(j>3?-1:1);

      CHECK(b.hasAttacker({i,j},dir)==attacks[i][j]);
      CHECK(b.isAttacked({i,j})==attacks[i][j]);
    }
  }
  SUBCASE("BLACK KNIGHT"){
    b.setPiece({3,3}, Piece::blackKnight);
    for(int i =1; i<=5; i+=4){
      attacks[2][i] = true;
      attacks[4][i] = true;
      attacks[i][2] = true;
      attacks[i][4] = true;
    }
    BOARD_LOOP{
      if(i==3 and j==3) continue;
      dir.x = i==3?0:(i>3?-1:1);
      dir.y = j==3?0:(j>3?-1:1);
      CHECK(b.isAttacked({i,j})==attacks[i][j]);
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
      int xking = h / 8;
      int yking = h % 8;

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
          int x = i % 8;
          int y = i / 8;
          if (x == xking && y == yking) {continue;}
          int diffx = abs(x - xking);
          int diffy = abs(y - yking);
          int dir = 2 * move - 1;

          //Loop through all the pieces
          for (unsigned char j = 0; j < sizeof(Piece::blackPieces) / sizeof(Piece::Piece); j++)
          {
            Piece::Piece piece = Piece::blackPieces[j];
            if (((y == 0 || y == 7) && piece == Piece::blackPawn) || piece == Piece::blackKing) {continue;}

            c.board[x][y] = (Piece::Piece)(piece ^ (c.state & Board::blackToMoveMask));
            check result = c.getCheck({xking,yking});

            //Check for check
            if (((diffx == 0 || diffy == 0) && (piece == Piece::blackQueen || piece == Piece::blackRook))
                || (diffx == diffy && (piece == Piece::blackQueen || piece == Piece::blackBishop))
                || (piece == Piece::blackKnight && diffx + diffy == 3 && diffx != 0 && diffy != 0)
                || (piece == Piece::blackPawn && diffx == 1 && yking - y == dir))
            {
              CHECK((c.state & Board::checkMask) == Board::checkMask);
              CHECK(result.len == 1);
              for (int newy = 0; newy < 8; newy++)
                for (int newx = 0; newx < 8; newx++)
                  if (piece == Piece::blackKnight || piece == Piece::blackPawn)
                  {
                    if (newx == x && newy == y)
                      CHECK(result.heatMap[newx][newy] == 1);
                    else
                      CHECK(result.heatMap[newx][newy] == 0);
                  }
                  else if (diffy == 0)
                  {
                    if (((newx >= x && newx < xking) || (newx > xking && newx <= x)) && newy == y)
                      CHECK(result.heatMap[newx][newy] == 1);
                    else
                      CHECK(result.heatMap[newx][newy] == 0);
                  }
                  else if (diffx == 0)
                  {
                    if (((newy >= y && newy < yking) || (newy > yking && newy <= y)) && newx == x)
                      CHECK(result.heatMap[newx][newy] == 1);
                    else
                      CHECK(result.heatMap[newx][newy] == 0);
                  }
                  else
                  {
                    if ((abs(newx - xking) == abs(newy - yking)) && ((newx >= x && newx < xking) || (newx > xking && newx <= x)) && ((newy >= y && newy < yking) || (newy > yking && newy <= y)))
                      CHECK(result.heatMap[newx][newy] == 1);
                    else
                      CHECK(result.heatMap[newx][newy] == 0);
                  }
            }
            else
            {
              CHECK((c.state & Board::checkMask) == 0);
              CHECK(result.len == 0);
              for (int y = 0; y < 8; y++)
              	for (int x = 0; x < 8; x++)
                  CHECK(result.heatMap[x][y] == 0);
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
      int xking = h / 8;
      int yking = h % 8;

      for (int i = 0; i < 64; i++)
      {
        if (i == h) {continue;}
        int x = i / 8;
        int y = i % 8;

        for (int j = 0; j < 64; j++) c.board[j / 8][j % 8] = Piece::none;
        c.board[xking][yking] = Piece::whiteKing;
        c.board[x][y] = Piece::blackQueen;
        check result = c.getCheck({xking,yking});
        if (result.len != 0)
        {
          int diffx = abs(xking - x);
          int diffy = abs(yking - y);
          if (diffx <= 1 && diffy <= 1) {continue;}
          int newx,newy;
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
          result = c.getCheck({xking,yking});
          
          diffx = newx - xking;
          diffy = newy - yking;
          if (diffx * diffy == -1) CHECK(result.heatMap[newx][newy] == 3);
          else if (diffx * diffy == 1) CHECK(result.heatMap[newx][newy] == 5);
          else if (diffx == 0) CHECK(result.heatMap[newx][newy] == 9);
          else CHECK(result.heatMap[newx][newy] == 17);
          CHECK((c.state & Board::checkMask) == 0);
          c.board[newx][newy] = Piece::blackKnight;
          result = c.getCheck({xking,yking});
          CHECK(result.heatMap[newx][newy] == 0);
          CHECK((c.state & Board::checkMask) == 0);
        }
      }
    }
  }
}

TEST_CASE("Testing calcMoves function")
{
  SUBCASE("Standard board position")
  {
    Board b("k7/8/4r3/8/5b2/4P3/3K4/8 w - -");
    b.calcMoves();
    b.printLegalMoves();
  }
}
