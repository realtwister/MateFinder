#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "Board.h"

void executeAllMoves(Board b, int depth, int maxDepth)
{
  if (depth == maxDepth) return;
  for (int i = 0; i < b.getMoves().size(); i++)
  {
    if (depth == 1)
      std::cout << "Executing move " << i+1 << "/" << b.getMoves().size() << std::endl;
    executeAllMoves(b.cloneAndExecMove(b.getMoves()[i]), depth + 1, maxDepth);
  }
}

int main(int argc, char * argv[])
{
  //Set up the random number generator
  std::srand(std::time(0));
  
  //Check if the number of moves is given as an argument
  int N;
  if (argc <= 1)
  {
    std::cout << "You can supply the number of moves if you want." << std::endl;
    std::cout << "For now, the default value of 5 is used." << std::endl;
    N = 5;
  }
  else
  {
    N = std::atoi(argv[1]);
    if (N <= 0)
    {
      std::cout << "You cannot supply a non-positive number of moves. Exiting." << std::endl;
      return 1;
    }
  }
  
  //Time the program
  auto start = std::chrono::high_resolution_clock::now();
  executeAllMoves(Board(), 1, N);
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "The program ran in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds." << std::endl;
  return 0;
}
