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
    
    /*
    class Tile{
        int row;
        int col;
        public:
        Tile(int rowNum, int colNum); 
        
    };
    */
    //std:: vector < tiles[2] > tilesa;
    std:: vector <std::pair <int, int> > potentialTilesToFlip; //temp vec to store tiles in case legal move, then flip
    std:: vector <std::vector <std::pair <int, int> > > tilesToFlip; // legalMoveIdx indicates which entry of this vec to execute
    int legalMoveIdx; //needed by applyMove as well

    int legalMoves(int playerNum, bool showDisplay); //returns largest index of legal moves
    void applyMove(int playerNum, int moveChosen, bool showDisplay); //uses tilesToFlip to update the actual board
};

#endif