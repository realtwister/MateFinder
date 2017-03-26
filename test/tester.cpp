#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#define DEBUG
#include "../src/Board.h"
#include "testSquare.cpp"

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
