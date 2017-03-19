#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../src/Board.h"

TEST_CASE("Starting check position") {
  Board b;

  SUBCASE("test getters") {
    for (int i = 0; i < 8; i++) {
      CHECK(b.getSquare({ 1, 1 }) == 'P');
      CHECK(b.getSquare({ 1, 6 }) == 'p');
    }
  }
}
