#include "doctest.h"
#include "../src/Board.h"
TEST_CASE("Test squares structure"){
  square s1={1,2};
  square s2={10,-3};

  SUBCASE("Addition operator"){
    square s3 = s1+s2;
    CHECK(s3.x == 11);
    CHECK(s3.y == -1);
  }

  SUBCASE("+= operator"){
    s2 +=s1;
    CHECK(s2.x== 11);
    CHECK(s2.y == -1);
  }
}