#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "DFS.h"

int main(int argc, char * argv[])
{
  //Board b("5rk1/p5pp/2pN4/1p1n4/8/4QP1N/qP2K2P/n6R w - -");
  //b = Board("1k6/8/QK6/PP6/8/8/8/8 w - -"); //in 1
  //b = Board("1k6/8/8/PPK5/8/8/Q7/8 w - -");
  //b = Board("1k6/8/1K6/PP6/8/8/Q7/8 b - -");
//  b=Board("8/2k5/8/8/8/6R1/4K3/1Q6 w - -"); //in 3
  //b=Board("1k3n2/8/1b6/8/6Q1/5B2/R2K4/8 w - -"); // in 4
  //b = Board("1k6/8/8/PPK2Q2/8/8/8/8 w - -");
  //b = Board("1k6/8/8/2K2Q2/8/8/8/8 w - -");
  //b = Board("1k6/8/8/5Q2/8/1K6/8/8 w - -");
//  b = Board("8/1k6/8/5Q2/1K6/8/8/8 w - -");

  Board b= Board("r7/1b1pk2p/p3p3/q3N3/8/1PpB2Q1/P1P2PPP/2KR3R b - -");
  DFS dfs(&b,10);
  dfs.search();
  return 0;
}
