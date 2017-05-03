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
      REQUIRE(b.getSquare({ i, 1 }) == 'P');
      REQUIRE(b.getSquare({ i, 6 }) == 'p');
    }
  }

  SUBCASE("Test color get/set"){
    REQUIRE(!b.blackToMove());
    b.changeColor();
    REQUIRE(b.blackToMove());
    b.clearBoard();
    for( signed char i =0; i<8;i++)
      for(signed char j=0; j<8;j++){
        REQUIRE(b.getSquare({i,j}) == Piece::none);
    }
    b.setPiece({3,3},Piece::blackKing);
    REQUIRE(b.getSquare({3,3}) == Piece::blackKing);
  }

  SUBCASE("isFriendly"){
    for (signed char i = 0; i < 7; i++) {
      REQUIRE(b.isFriendly({ i, 1 }));
      REQUIRE(b.isFriendly({ i, 0 }));
      REQUIRE(!b.isFriendly({ i, 6 }));
      REQUIRE(!b.isFriendly({ i, 7 }));
    }
    REQUIRE(!b.isFriendly({1, 3}));
    b.changeColor();
    REQUIRE(!b.isFriendly({1,3}));
  }

  SUBCASE("hasAttacker"){
    //black pawn
    for (signed char i = 1; i < 7; i++) {
    REQUIRE(b.hasAttacker({i,5},{1,1}));
    REQUIRE(b.hasAttacker({i,5},{-1,1}));
    }
    REQUIRE(b.hasAttacker({0,5},{1,1}));
    REQUIRE(b.hasAttacker({7,5},{-1,1}));
    
    //black bishop
    REQUIRE(b.hasAttacker({1,6},{1,1}));
    REQUIRE(b.hasAttacker({3,6},{-1,1}));
    REQUIRE(b.hasAttacker({6,6},{-1,1}));
    REQUIRE(b.hasAttacker({4,6},{1,1}));
    
    //black rook
    REQUIRE(b.hasAttacker({1,7},{-1,0}));
    REQUIRE(b.hasAttacker({0,6},{0,1}));
    REQUIRE(b.hasAttacker({6,7},{1,0}));
    REQUIRE(b.hasAttacker({7,6},{0,1}));

    //black queen
    REQUIRE(b.hasAttacker({2,7},{1,0}));
    REQUIRE(b.hasAttacker({2,6},{1,1}));
    REQUIRE(b.hasAttacker({3,6},{0,1}));
    REQUIRE(b.hasAttacker({4,6},{-1,1}));
    REQUIRE(b.hasAttacker({4,7},{-1,0}));

    /*b.changeColor();
    //white pawn
    for (signed char i = 1; i < 7; i++) {
    REQUIRE(b.hasAttacker({i,2},{-1,-1}));
    REQUIRE(b.hasAttacker({i,2},{1,-1}));
    }
    REQUIRE(b.hasAttacker({7,2},{-1,-1}));
    REQUIRE(b.hasAttacker({0,2},{1,-1}));
    // white bishop
    REQUIRE(b.hasAttacker({1,1},{1,-1}));
    REQUIRE(b.hasAttacker({3,1},{-1,-1}));
    REQUIRE(b.hasAttacker({6,1},{-1,-1}));
    REQUIRE(b.hasAttacker({4,1},{1,-1}));

    //black rook
    REQUIRE(b.hasAttacker({1,0},{-1,0}));
    REQUIRE(b.hasAttacker({0,1},{0,-1}));
    REQUIRE(b.hasAttacker({6,0},{1,0}));
    REQUIRE(b.hasAttacker({7,1},{0,-1}));

    //black queen
    REQUIRE(b.hasAttacker({2,0},{1,0}));
    REQUIRE(b.hasAttacker({2,1},{1,-1}));
    REQUIRE(b.hasAttacker({3,1},{0,-1}));
    REQUIRE(b.hasAttacker({4,1},{-1,-1}));
    REQUIRE(b.hasAttacker({4,0},{-1,0}));*/

    //none
    for(signed char i =0; i<8; i++){
      REQUIRE(!b.hasAttacker({i,3},{1,0}));
      REQUIRE(!b.hasAttacker({i,3},{1,1}));
      REQUIRE(!b.hasAttacker({i,3},{1,-1}));
      REQUIRE(!b.hasAttacker({i,3},{0,1}));
      REQUIRE(!b.hasAttacker({i,3},{0,-1}));
      REQUIRE(!b.hasAttacker({i,3},{1,0}));
      REQUIRE(!b.hasAttacker({i,3},{1,-1}));
      REQUIRE(!b.hasAttacker({i,3},{1,1}));
      REQUIRE(!b.hasAttacker({i,4},{1,0}));
      REQUIRE(!b.hasAttacker({i,4},{1,1}));
      REQUIRE(!b.hasAttacker({i,4},{1,-1}));
      REQUIRE(!b.hasAttacker({i,4},{0,1}));
      REQUIRE(!b.hasAttacker({i,4},{0,-1}));
      REQUIRE(!b.hasAttacker({i,4},{1,0}));
      REQUIRE(!b.hasAttacker({i,4},{1,-1}));
      REQUIRE(!b.hasAttacker({i,4},{1,1}));
    }
  }
}


TEST_CASE("Check hasAttacker"){
  Board b;
  bool attacks[8][8]={false};
  square<int> dir={};
  b.clearBoard();
  SUBCASE("hasAttacker"){
    SUBCASE("BLACK PAWN LOOP"){
      b.setPiece({3,3}, Piece::blackPawn);
      attacks[2][2]=true;
      attacks[4][2]=true;
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

      REQUIRE(b.hasAttacker({i,j},dir)==attacks[i][j]);
      REQUIRE(b.isAttacked({i,j})==attacks[i][j]);
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
      REQUIRE(b.isAttacked({i,j})==attacks[i][j]);
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
        //Set up the board except for the other piece
        for (int j = 0; j < 64; j++) {c.board[j / 8][j % 8] = Piece::none;}
        c.board[xking][yking] = Piece::whiteKing;

        //Calculate location of the other piece
        int x = i % 8;
        int y = i / 8;
        if (x == xking && y == yking) {continue;}
        int diffx = abs(x - xking);
        int diffy = abs(y - yking);

        //Loop through all the pieces
        for (unsigned char j = 0; j < sizeof(Piece::blackPieces) / sizeof(Piece::Piece); j++)
        {
          Piece::Piece piece = Piece::blackPieces[j];
          if (((y == 0 || y == 7) && piece == Piece::blackPawn) || piece == Piece::blackKing) {continue;}

          c.board[x][y] = piece;
          check result = c.getCheck({xking,yking});

          //Check for check
          if (((diffx == 0 || diffy == 0) && (piece == Piece::blackQueen || piece == Piece::blackRook))
              || (diffx == diffy && (piece == Piece::blackQueen || piece == Piece::blackBishop))
              || (piece == Piece::blackKnight && diffx + diffy == 3 && diffx != 0 && diffy != 0)
              || (piece == Piece::blackPawn && diffx == 1 && yking - y == -1))
          {
            REQUIRE((c.state & Board::checkMask) == Board::checkMask);
            REQUIRE(result.len == 1);
            for (int newy = 0; newy < 8; newy++)
              for (int newx = 0; newx < 8; newx++)
                if (piece == Piece::blackKnight || piece == Piece::blackPawn)
                {
                  if (newx == x && newy == y)
                    REQUIRE(result.heatMap[newx][newy] == 1);
                  else
                    REQUIRE(result.heatMap[newx][newy] == 0);
                }
                else if (diffy == 0)
                {
                  if (((newx >= x && newx < xking) || (newx > xking && newx <= x)) && newy == y)
                    REQUIRE(result.heatMap[newx][newy] == 1);
                  else
                    REQUIRE(result.heatMap[newx][newy] == 0);
                }
                else if (diffx == 0)
                {
                  if (((newy >= y && newy < yking) || (newy > yking && newy <= y)) && newx == x)
                    REQUIRE(result.heatMap[newx][newy] == 1);
                  else
                    REQUIRE(result.heatMap[newx][newy] == 0);
                }
                else
                {
                  if ((abs(newx - xking) == abs(newy - yking)) && ((newx >= x && newx < xking) || (newx > xking && newx <= x)) && ((newy >= y && newy < yking) || (newy > yking && newy <= y)))
                    REQUIRE(result.heatMap[newx][newy] == 1);
                  else
                    REQUIRE(result.heatMap[newx][newy] == 0);
                }
          }
          else
          {
            REQUIRE((c.state & Board::checkMask) == 0);
            REQUIRE(result.len == 0);
            for (int y = 0; y < 8; y++)
            	for (int x = 0; x < 8; x++)
                REQUIRE(result.heatMap[x][y] == 0);
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
          if (diffx * diffy == -1) REQUIRE(result.heatMap[newx][newy] == 1);
          else if (diffx * diffy == 1) REQUIRE(result.heatMap[newx][newy] == 3);
          else if (diffx == 0) REQUIRE(result.heatMap[newx][newy] == 4);
          else REQUIRE(result.heatMap[newx][newy] == 2);
          REQUIRE((c.state & Board::checkMask) == 0);
          c.board[newx][newy] = Piece::blackKnight;
          result = c.getCheck({xking,yking});
          REQUIRE(result.heatMap[newx][newy] == 0);
          REQUIRE((c.state & Board::checkMask) == 0);
        }
      }
    }
  }
}

//Check if move specified by the movestring is present in the moveArray
void CheckPresence(Board * b, const moveArray moves, const char * movestr, bool checkPresent)
{
  move checkMove = {{(int)(movestr[0] - 'a'), (int)(movestr[1] - '1')}, {(int)(movestr[3] - 'a'), (int)(movestr[4] - '1')}, (Piece::Piece)movestr[5]};
  if (b->state & Board::blackToMoveMask) {checkMove.start.y = 7 - checkMove.start.y; checkMove.end.y = 7 - checkMove.end.y;}
  for (int i = 0; i < moves.size(); i++)
  {
    if (moves[i].start.x == checkMove.start.x && moves[i].start.y == checkMove.start.y
        && moves[i].end.x == checkMove.end.x && moves[i].end.y == checkMove.end.y
        && (checkMove.promoteTo == 0 || moves[i].promoteTo == checkMove.promoteTo))
    {
      if (!checkPresent)
      {
        b->printLegalMoves();
        std::cout << "The move " << movestr << " is not present." << std::endl;
        REQUIRE(0);
      }
      else
      {
        REQUIRE(1);
        return;
      }
      return;
    }
  }
  if (checkPresent)
  {
    b->printLegalMoves();
    std::cout << "The move " << movestr << " is not present." << std::endl;
    REQUIRE(0);
  }
  else
  {
    REQUIRE(1);
  }
}

#define CheckBoard(BOARDSTR, LEGALMOVES...)  {Board b(BOARDSTR); \
                                              moveArray moves = b.getMoves(); \
                                              const char * legalMoves[] = {LEGALMOVES}; \
                                              REQUIRE(sizeof(legalMoves) / sizeof(char*) == moves.size()); \
                                              for (unsigned int i = 0; i < sizeof(legalMoves) / sizeof(char*); i++) \
                                                CheckPresence(&b, moves, legalMoves[i], true);}

#define CheckMove(BOARDSTR, LEGALMOVE) {Board b(BOARDSTR); \
                                        moveArray moves = b.getMoves(); \
                                        CheckPresence(&b, moves, LEGALMOVE, true);}
                                        
#define CheckMoveAbsent(BOARDSTR, LEGALMOVE) {Board b(BOARDSTR); \
                                              moveArray moves = b.getMoves(); \
                                              CheckPresence(&b, moves, LEGALMOVE, false);}

TEST_CASE("calcMoves")
{
  SUBCASE("Standard board position")
  {
    CheckBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -",
    "a2-a3",
    "a2-a4",
    "b2-b3",
    "b2-b4",
    "c2-c3",
    "c2-c4",
    "d2-d3",
    "d2-d4",
    "e2-e3",
    "e2-e4",
    "f2-f3",
    "f2-f4",
    "g2-g3",
    "g2-g4",
    "h2-h3",
    "h2-h4",
    "b1-a3",
    "b1-c3",
    "g1-f3",
    "g1-h3");
    
    CheckBoard("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq -",
    "a7-a6",
    "a7-a5",
    "b7-b6",
    "b7-b5",
    "c7-c6",
    "c7-c5",
    "d7-d6",
    "d7-d5",
    "e7-e6",
    "e7-e5",
    "f7-f6",
    "f7-f5",
    "g7-g6",
    "g7-g5",
    "h7-h6",
    "h7-h5",
    "b8-a6",
    "b8-c6",
    "g8-f6",
    "g8-h6");
    
    CheckBoard("8/8/8/8/6q1/8/5k2/7K b - -",
    "f2-e3",
    "f2-e2",
    "f2-e1",
    "f2-f1",
    "f2-f3",
    "f2-g3",
    "g4-c8",
    "g4-d7",
    "g4-e6",
    "g4-f5",
    "g4-a4",
    "g4-b4",
    "g4-c4",
    "g4-d4",
    "g4-e4",
    "g4-f4",
    "g4-d1",
    "g4-e2",
    "g4-f3",
    "g4-g8",
    "g4-g7",
    "g4-g6",
    "g4-g5",
    "g4-g3",
    "g4-g2",
    "g4-g1",
    "g4-h5",
    "g4-h4",
    "g4-h3");
  }
  
  SUBCASE("Castles")
  {
    //Check castles
    CheckMove("4k3/8/8/8/8/8/8/4K2R w K -", "e1-g1");
    CheckMove("4k3/3r4/8/8/8/8/8/4K2R w K -", "e1-g1");
    CheckMove("4k3/7r/8/8/8/8/8/4K2R w K -", "e1-g1");
    CheckMoveAbsent("4k3/4r3/8/8/8/8/8/4K2R w K -", "e1-g1"); //Check if being checked prevents castles
    CheckMoveAbsent("4k3/5r2/8/8/8/8/8/4K2R w K -", "e1-g1"); //Check if moving over an attacked square prevents castles
    CheckMoveAbsent("4k3/6r1/8/8/8/8/8/4K2R w K -", "e1-g1"); //Check if an attacked target square prevents castles
    CheckMoveAbsent("4k3/8/8/8/8/8/8/4K1NR w K -", "e1-g1");  //Check if an obstruction prevents castles
    CheckMoveAbsent("4k3/8/8/8/8/8/8/4K1nR w K -", "e1-g1");
    CheckMoveAbsent("4k3/8/8/8/8/8/8/4K2R w - -","e1-g1");
    
    //Auxilliary checks on queenside and opponent
    CheckMoveAbsent("4k3/8/8/8/8/8/8/RN2K3 w Q -","e1-c1");
    CheckMoveAbsent("4k3/8/8/8/8/8/8/R2NK3 w Q -","e1-c1");
    CheckMove("r3k3/8/8/8/8/8/8/4K3 b q -","e8-c8");
    CheckMoveAbsent("r3k3/8/8/8/7Q/8/8/4K3 b q -","e8-c8");
    CheckMove("4k2r/8/8/8/8/8/8/4K3 b k -","e8-g8");
  }
  
  SUBCASE("Pawn")
  {
    CheckBoard("3r3k/2P5/8/8/8/8/8/4K3 w - -",
    "e1-e2",
    "e1-f2",
    "e1-f1",
    "c7-c8Q",
    "c7-c8R",
    "c7-c8B",
    "c7-c8N",
    "c7-d8Q",
    "c7-d8R",
    "c7-d8B",
    "c7-d8N");
    CheckMove("7k/8/3r4/1pP5/8/8/8/3K4 w - b6","c5-d6");
    CheckMoveAbsent("7k/8/3r4/1pP5/8/8/8/3K4 w - b6","c5-b6");
    CheckMoveAbsent("7k/8/3r4/1pP5/8/8/8/3K4 w - b6","c5-c6");
    CheckMove("7k/8/8/r1Pp1K2/8/8/8/8 w - d6","c5-c6");
    CheckMoveAbsent("7k/8/8/r1Pp1K2/8/8/8/8 w - d6","c5-d6");
    CheckMove("7k/8/8/r4K2/2P5/8/8/8 w - -","c4-c5");
    CheckMoveAbsent("8/8/8/8/R1Pp1k2/8/8/7K b - c3","d4-c3");
    CheckMove("8/8/8/8/R1Pp1k2/8/8/7K b - c3","d4-d3");
    CheckMove("7k/2b5/8/3pP3/8/6K1/8/8 w - d6","e5-d6");
    CheckMove("7k/2b5/3p4/4P3/8/6K1/8/8 w - -","e5-d6");
    CheckMove("7k/4r3/8/8/8/8/4P3/4K3 w - -","e2-e4");
    CheckMove("7k/4r3/8/8/8/8/4P3/4K3 w - -","e2-e3");
  }
  
  SUBCASE("Blocking")
  {
    CheckBoard("7k/3r4/8/4N3/8/5q2/4B3/3K4 w - -",
    "d1-c1",
    "d1-c2",
    "d1-e1",
    "e5-d7",
    "e5-d3");
    CheckMove("7k/3r4/8/8/8/5q2/3NB3/3K4 w - -","e2-f3");
  }
  
  SUBCASE("Double check")
  {
  	CheckBoard("7k/8/8/6q1/3r4/2Q5/2QKQ3/4Q3 w - -");
  }
  
  SUBCASE("Draw")
  {
    Board b;
    REQUIRE(!(b.state & Board::drawMask));
    
    b = Board("8/8/4k3/8/8/2K5/5N2/8 w - -");
    REQUIRE((b.state & Board::drawMask));
    
    b = Board("8/8/2k5/5B2/8/3K4/8/8 w - -");
    REQUIRE((b.state & Board::drawMask));
    
    b = Board("8/4n3/2k5/8/5B2/3K4/8/8 w - -");
    REQUIRE(!(b.state & Board::drawMask));
    
    b = Board("8/8/2k5/8/5R2/3K4/8/8 w - -");
    REQUIRE(!(b.state & Board::drawMask));
    
    b = Board("8/4b3/2k5/8/8/3K4/8/8 w - -");
    REQUIRE((b.state & Board::drawMask));
  }
}

TEST_CASE("execMove")
{
  //Test if pieces get moved
  SUBCASE("Basic functionality")
  {
    Board b;
    b.execMove({{0,1},{0,3}});
    REQUIRE(b.board[0][6] == Piece::none);
    REQUIRE(b.board[0][4] == Piece::blackPawn);
    REQUIRE((b.state & Board::whiteCastleKingsideMask));
    REQUIRE((b.state & Board::whiteCastleQueensideMask));
    REQUIRE((b.state & Board::blackCastleKingsideMask));
    REQUIRE((b.state & Board::blackCastleQueensideMask));
    REQUIRE(b.enPassant == 0);
    REQUIRE((b.state & Board::blackToMoveMask));
    REQUIRE(b.legalMoves.size() == 20);
  }
  
  //Test if the castle is unset
  SUBCASE("Castle flags")
  {
    Board b("r1bqkbnr/pp1p1ppp/2n1p3/1Bp5/4P3/5N2/PPPP1PPP/RNBQK2R w KQkq -");
    Board c = b;
    c.execMove({{4,0},{6,0}});
    REQUIRE(c.board[4][7] == Piece::none);
    REQUIRE(c.board[7][7] == Piece::none);
    REQUIRE(c.board[6][7] == Piece::blackKing);
    REQUIRE(c.board[5][7] == Piece::blackRook);
    REQUIRE(!(c.state & Board::blackCastleKingsideMask));
    REQUIRE(!(c.state & Board::blackCastleQueensideMask));
    REQUIRE((c.state & Board::whiteCastleKingsideMask));
    REQUIRE((c.state & Board::whiteCastleQueensideMask));
    
    c = b;
    c.execMove({{7,0},{6,0}});
    REQUIRE(!(c.state & Board::blackCastleKingsideMask));
    REQUIRE((c.state & Board::blackCastleQueensideMask));
    REQUIRE((c.state & Board::whiteCastleKingsideMask));
    REQUIRE((c.state & Board::whiteCastleQueensideMask));
  }
  
  SUBCASE("En passant")
  {
    Board b("rnbqkbnr/ppp2ppp/4p3/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6");
    b.execMove({{4,4},{3,5}});
    REQUIRE(b.board[4][3] == Piece::none);
    REQUIRE(b.board[3][2] == Piece::blackPawn);
    REQUIRE(b.board[3][3] == Piece::none);
    REQUIRE(b.enPassant == -1);
  }
  
  SUBCASE("Promotion")
  {
    Board b("k7/2P5/1K6/8/8/8/8/8 w - -");
    b.execMove({{2,6},{2,7},Piece::whiteQueen});
    REQUIRE(b.board[2][1] == Piece::none);
    REQUIRE(b.board[2][0] == Piece::blackQueen);
  }
}
