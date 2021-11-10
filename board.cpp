#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "board.h"

using namespace std;

board::board(){
    updatedBoard.resize( 8, vector <char> (8 , 2) ); // Initialize 2D vec w/ 2's (empty spaces)
    legalMovesList.resize(65);
    //vector < vector<char> > board ( 8, vector <char> (8)); // Initialize 2D vec w/ 0's (empty spaces)

};

void board::displayBoard(){
    int row, col, i;
    cout << "  ";
    for (i = 0; i < 8; i++){
        cout << i << ' ';
    }
    cout << endl;
    for (row = 0; row < 8; row++){
        cout << row << ' ';
        for (col = 0; col < 8; col++){
            char piece = updatedBoard[row][col];
            switch(piece){
                case '2': //empty
                    if ( (row + col) & 1){ //if row + col is odd, then color is black
                        cout << "\u2593\u2593"; //black tile
                    }
                    else {
                        cout << "\u2591\u2591"; //white tile
                    } 
                    break;

                case '0': //player 1
                    cout << "\u26ab"; // black piece
                    break;

                case '1': //player 2
                    cout << "\u26aa"; // white piece
                    break;
                default:
                    /*int moveNum = piece - '2';
                    std:: string space =" ";
                    if (moveNum >=10) { space = ""}
                    cout << piece - '2' << " "; //add move Num to screen
                    */
                    std:: cerr << "Not 0 or 1 or 2" << endl;
                    break;
            }
        }
        cout << endl; //change row
    }
    cout << endl;
}

void board::setTile(int row, int col, int newVal){
    updatedBoard[row][col] = newVal; 
}

void board::legalMoves(int playerNum){
    int row, col, hzOffset, vtOffset, currRow, currCol, currTile, legalMoveIdx = 0;
    for (row = 0; row < 8; row++){
        for (col = 0; col < 8; col++){
            currTile = updatedBoard[row][col];
            if (currTile == '2'){ //if Tile empty
                for (hzOffset = -1; hzOffset <= 1; hzOffset++){
                    for (vtOffset = -1; vtOffset <= 1; vtOffset++){
                        if (hzOffset == 0 && vtOffset == 0){
                            continue; //Skip staying in place
                        }

                        /* find valid moves */
                        currRow = row + hzOffset; //temp var's to "foresee" which tiles we are eval'ing 
                        currCol = col + vtOffset;
                        
                            while ( (currRow < 8 && currRow >= 0 && currCol < 8 && currCol >= 0) //within bounds of board
                                && updatedBoard[currRow][currCol] == ('0'+ (1-playerNum) ) ){ //opposite of player PROB
                                currRow+=hzOffset;
                                currCol+=vtOffset; //travel in this dir
                            }
                
                        if (currRow != (row + hzOffset)  && currCol != (col + vtOffset) ){ //if not in adj tile
                            //add to vector of valid moves
                            //legalMovesList[ ((1 << (row + 2)) + col) ] = 1; //array of if a tile is a legal move or not
                            //legalMovesList[row][col] = 1; //this move is valid //segfault
                            cout << "(" << currRow - hzOffset<< ',' << currCol - vtOffset<< ')';
                            cout <<  ": (" << row <<"," << col << ")" << endl;
                            legalMoveIdx++;
                        }
                    }
                } 
            }
            /*Display legal moves*/
        }
    }

    std::cerr << "Number of legal moves:" << legalMoveIdx << endl;
}