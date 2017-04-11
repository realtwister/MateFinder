#include "doctest.h"
#include "../src/Board.h"
TEST_CASE("Test squares structure"){
  square<void> sq1;
  sq1 = {1,2};
  sq1 += square<int>(3,4);
  sq1 += square<void>(-1,-2);
  sq1 += square<short>(-2,-2);
  sq1 = sq1 + sq1;
  square<int> sq2 = sq1;
  square<void> sq3 = sq2;
  REQUIRE(sq3.x == 2);
  REQUIRE(sq3.y == 4);
}
