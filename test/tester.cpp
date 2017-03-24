#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#define DEBUG
#include "../src/Board.h"

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
