//#include <iostream>
#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>


class board{
    public:
    board();
    void displayBoard();
    void setTile(int row, int col, int newVal);
    //private: //need to make board global to all files
    std:: vector <std::vector<char> > updatedBoard; //actual board
    std:: vector <std:: vector<int> > legalMovesList;
    
    class tile{
        tile(int row, int col); 
        
    };
    std:: vector <tile> tiles;

    void legalMoves(int playerNum);
};

#endif