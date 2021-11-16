//#include <iostream>
#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>


class Board{
    public:
    Board();
    void displayBoard();
    void setTile(int row, int col, int newVal);
    //private: //need to make board global to all files
    std:: vector <std::vector<char> > updatedBoard; //actual board
    
    class Tile{
        int row;
        int col;
        public:
        Tile(int rowNum, int colNum); 
        
    };
    //std:: vector < tiles[2] > tilesa;
    std:: vector <std::pair <int, int> > potentialTilesToFlip;
    std:: vector <std::vector <std::pair <int, int> > > tilesToFlip;

    void legalMoves(int playerNum);
};

#endif