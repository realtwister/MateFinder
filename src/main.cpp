#include <iostream>
#include "DFS.h"

/**
 * @mainpage
 * Welcome to MateFinder! This program will attempt to find out whether a chess player mate his or her opponent from a given position on the board.
 * 
 * @section Preliminaries
 * First of all, get the source from github: https://github.com/realtwister/matefinder.
 * Next, build the program by running make.
 * 
 * @section Authors
 * This program was braught to you by Erik Meulman and Arjan Cornelissen.
 * 
 * @section About
 * This project was part of the course Object Oriented Scientific Programming with C++ at Delft University of Technology.
 */

/**
 * @file
 * This file contains the CLI.
 */

/**
 * This function displays the help in the CLI.
 */
void displayHelp()
{
  std::cout << "This program will calculate if there is a mate position, from a position on the chess board." << std::endl << std::endl;
  std::cout << "Usage: matefinder [\"FEN\"] [OPTIONS]" << std::endl;
  std::cout << "Make sure the FEN is enclosed within double quotation marks." << std::endl;
  std::cout << "Optional arguments are:" << std::endl;
  std::cout << "  -f <filename>  Read the FEN from a file. When using this option, the normal \"FEN\" argument can be omitted." << std::endl;
  std::cout << "  -d <depth>     This option allows you to specify the depth with which the position should be searched, expressed in the number of half-moves." << std::endl;
  std::cout << "  -t             Enables the turbo mode. In this mode, the program will correctly find mates for the player that is to move, but it will not detect whether he can get mated himself." << std::endl;
  std::cout << "  -h             Display the help." << std::endl;
}

/**
 * This function displays the move sequence that is stored in a stack of moves.
 * @param[in] blackToMove This boolean stores whether black is to move.
 * @param[in] moves This stack stores a number of moves that are to be displayed.
 */
void printMoveSequence(bool blackToMove, std::stack<move> moves)
{
  int movectr = 1;
  if (blackToMove) {std::cout << "1. .....";}
  while (!moves.empty())
  {
    if (blackToMove)
    {
      std::cout << " - ";
      moves.top().printMove(true);
      std::cout << std::endl;
      movectr++;
    }
    else
    {
      std::cout << movectr << ". ";
      moves.top().printMove(false);
    }
    moves.pop();
    blackToMove = !blackToMove;
  }
  if (blackToMove) {std::cout << std::endl;}
}

/**
 * The main function. It processes the user input, calls the search algorithm, and displays the results.
 * @param[in] argc The number of arguments that the user passed to the program.
 * @param[in] argv A list of arguments.
 * @returns An exit code. 0 indicates that everything went according to plan. 1 indicates that an error occurred.
 */
int main(int argc, char * argv[])
{
  //Read the user input
  if (argc == 1)
  {
    std::cout << "Welcome to MateFinder!" << std::endl;
    displayHelp();
    return 0;
  }
  
  Board b;
  int depth = 5;
  
  bool FENinput = false;
  bool depthinput = false;
  bool turbo = false;
  try
  {
    for (int i = 1; i < argc; i++)
    {
      if (argv[i][0] == '-')
      {
        switch(argv[i][1])
        {
          case 0x0:
            continue;
          case 'h':
            displayHelp();
            return 0;
          case 'd':
            depth = atoi(argv[++i]);
            if (depth < 0) throw std::invalid_argument("Error: depth cannot be negative.");
            depthinput = true;
            continue;
          case 'f':
            if (i + 1 == argc) throw std::invalid_argument("Error: another argument following -f is expected.");
            b = Board(argv[++i], true);
            FENinput = true;
            continue;
          case 't':
            turbo = true;
            continue;
          default:
            throw std::invalid_argument("Unrecognized option identifier.");
        }
      }
      b = Board(argv[i]);
      FENinput = true;
    }
  }
  catch (const std::invalid_argument &e)
  {
    std::cout << e.what() << std::endl;
    return 1;
  }
  if (!FENinput)
  {
    std::cout << "No input position specified." << std::endl;
    std::cout << "The starting position will be used by default." << std::endl;
  }
  if (!depthinput)
  {
    std::cout << "No depth parameter specified." << std::endl;
    std::cout << "The default value of 5 half-moves will be used." << std::endl;
  }
  
  //Do the search
  std::cout << "Examining the following position:" << std::endl;
  b.printBoard();
  DFS dfs(&b, depth, turbo);
  std::cout << "---------------------------" << std::endl;
  std::cout << "Starting search..." << std::endl;
  DFSresult res = dfs.search();
  std::cout << "...Search done!" << std::endl;
  
  //Display the results
  switch(res.state)
  {
    case -2:
      std::cout << "There is a mate in " << (res.depth + 1)/2 << " move";
      if (res.depth > 1) {std::cout << "s";}
      std::cout << "." << std::endl;
      std::cout << "The best move sequence is:" << std::endl;
      printMoveSequence(b.blackToMove(), res.moves);
      break;
    case 0:
      std::cout << "A draw is reached." << std::endl;
      std::cout << "The best move sequence is:" << std::endl;
      printMoveSequence(b.blackToMove(), res.moves);
      break;
    case 1:
      std::cout << "The maximum depth was reached without a decisive result." << std::endl;
      std::cout << "An example continuation is:" << std::endl;
      printMoveSequence(b.blackToMove(), res.moves);
      break;
    case 2:
      std::cout << "You will be mated in " << res.depth/2 << " move";
      if (res.depth > 1) {std::cout << "s";}
      std::cout << "." << std::endl;
      std::cout << "The best move sequence is:" << std::endl;
      printMoveSequence(b.blackToMove(), res.moves);
      break;
    default:
      std::cout << "Unexpected return value." << std::endl;
  }
  return 0;
}
