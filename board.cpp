#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "board.h"

using namespace std;

board::board(){
    updatedBoard.resize( 8, vector <char> (8) ); // Initialize 2D vec w/ 0's (empty spaces)
    //vector < vector<char> > board ( 8, vector <char> (8)); // Initialize 2D vec w/ 0's (empty spaces)

};

void board::displayBoard(){
    int row, col;
    for (row = 0; row < 8; row++){
        for (col = 0; col < 8; col++){
            char piece = updatedBoard[row][col];
            switch(piece){
                case '0': //empty
                    if ( (row + col) & 1){ //if row + col is odd, then color is black
                        cout << "\u2593\u2593"; //black tile
                    }
                    else {
                        cout << "\u2591\u2591"; //white tile
                    } 
                    break;

                case '1': //player 1
                    cout << "\u26ab"; // black piece
                    break;

                case '2': //player 2
                    cout << "\u26aa"; // white piece
                    break;
                default:
                    cerr << "Not 0,1, or 2" <<endl;
                    break;
            }
        }
        cout << endl; //change row
    }
}

void board::setTile(int row, int col, int newVal){
    updatedBoard[row][col] = newVal; 
}