//#include <iostream>
#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>

class Board{
    public:
    Board();
    int consecTurnsSkipped, piecesOnBoard;

    void displayBoard();
    void setTile(int row, int col, int newVal);
    //private: //need to make board global to all files
    std:: vector <std::vector<int> > updatedBoard; //actual board
    std:: vector <std::pair <int, int> > potentialTilesToFlip; //temp vec to store tiles in case legal move, then flip
    std:: vector <std::vector <std::pair <int, int> > > tilesToFlip; // legalMoveIdx indicates which entry of this vec to execute

    int legalMoveIdx; //needed by applyMove as well
    //int numPieces[1];
    int legalMoves(int playerNum, bool showDisplay); //returns largest index of legal moves
    void applyMove(int playerNum, int moveChosen, bool showDisplay); //uses tilesToFlip to update the actual board
    int heuristic (int isMax);
    std::vector<std::vector<int>> greed(int isMax);
    int mobility (int isMax);
};

//int weights[64] ;
/*
= {
        300, -100, 100,  50,  50, 100, -100,  300,
        -100, -200, -50, -50, -50, -50, -200, -100,
         100,  -50, 100,   0,   0, 100,  -50,  100,
          50,  -50,   0,   0,   0,   0,  -50,   50,
          50,  -50,   0,   0,   0,   0,  -50,   50,
         100,  -50, 100,   0,   0, 100,  -50,  100,
        -100, -200, -50, -50, -50, -50, -200, -100,
         300, -100, 100,  50,  50, 100, -100,  300
};
*/

#endif