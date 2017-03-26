#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#define DEBUG
#include "../src/Board.h"
#include "testSquare.cpp"
#include <iostream>

#define BOARD_LOOP for(signed char i=0;i<8;i++) for(signed char j=0;j<8;j++)
#define ROW_LOOP for (signed char i=0;i<8;i++)
#define KNIGHT_LOOP for()

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

  }

}
