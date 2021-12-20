#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include "board.hpp"

using namespace std;

Board::Board(){
    consecTurnsSkipped = 0;
    piecesOnBoard = 4;
    updatedBoard.resize( 8, vector <int> (8 , 2) ); // Initialize 2D vec w/ 2's (empty spaces)
    //tilesToFlip.resize(25); //make 65 if it doesn't work
    potentialTilesToFlip.clear();
};

void Board::displayBoard(){
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
                case 2: //empty
                    if ( (row + col) & 1){ //if row + col is odd, then color is black
                        cout << "\u2593\u2593"; //black tile
                    }
                    else {
                        cout << "\u2591\u2591"; //white tile
                    } 
                    break;

                case 0: //player 1
                    cout << "\u26ab"; // black piece
                    break;

                case 1: //player 2
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
/*
Board::Tile::Tile(int rowNum, int colNum){
    int row= rowNum;
    int col= colNum;
}
*/

void Board::setTile(int row, int col, int newVal){ //not related to Tile class in code
    updatedBoard[row][col] = newVal; 
}

int Board::legalMoves(int playerNum, bool showDisplay){
    int row, col, hzOffset, vtOffset, currRow, currCol, currTile;
    legalMoveIdx = 0;
    bool moveFound = false;
    std::pair <int, int> tile;
    //auto it = tilesToFlip.begin();
    tilesToFlip.clear();
    tilesToFlip.resize(25); //make 65 if it doesn't work
    for (row = 0; row < 8; row++){
        for (col = 0; col < 8; col++){
            currTile = updatedBoard[row][col];
            if (currTile == 2 ){ //if current tile is empty
                for (hzOffset = -1; hzOffset <= 1; hzOffset++){
                    for (vtOffset = -1; vtOffset <= 1; vtOffset++){
                        if (hzOffset == 0 && vtOffset == 0){
                            continue; //Skip staying in place
                        }
                        /* find valid moves */
                        currRow = row + hzOffset; //temp var's to "foresee" which tiles we are eval'ing 
                        currCol = col + vtOffset;
                        
                        while ( (currRow < 8 && currRow >= 0 && currCol < 8 && currCol >= 0) ){ //within bounds of Board
                            char evalPiece = updatedBoard[currRow][currCol];
                            if ( evalPiece == (1-playerNum) ){ //their color
                                tile = make_pair(currRow, currCol);
                                potentialTilesToFlip.push_back(tile);
                                currRow+=hzOffset;
                                currCol+=vtOffset; //travel in this dir
                            }
                            else if (evalPiece == 2 ){ //empty
                                potentialTilesToFlip.clear();
                                break;
                            }
                            else { //our color
                                if (currRow != (row + hzOffset) || currCol != (col + vtOffset) ){ //if not in adj tile
                                    if (!moveFound){ //this move has not been previously found
                                        legalMoveIdx++; //indicate one more move has been found //move to tiles to flip if not working
                                        if (showDisplay){
                                            /*Then Display legal moves*/
                                            cout << legalMoveIdx << ": ";
                                            cout <<  "(" << row <<"," << col << ")";
                                            //cout << std::endl;
                                            cout << "  Found by: (" << currRow << ',' << currCol << ')' << endl;
                                        }
                                    }    
                                    //tilesToFlip.insert(tilesToFlip.begin(), tile(row,col) ); //insert at head 
                                    if (!potentialTilesToFlip.empty() ){
                                        potentialTilesToFlip.push_back(make_pair(row, col) ); //add move as tile to "flip"
                                        for (int i = 0; i < potentialTilesToFlip.size(); i++){
                                            tilesToFlip[legalMoveIdx].push_back(potentialTilesToFlip[i]); // add tiles to flip at corr move
                                        }
                                        //potentialTilesToFlip.clear(); //don't think it's needed
                                    }
                                    moveFound = true; //set flag that move has been found for this empty tile
                                }
                                break;
                            }
                        }
                        potentialTilesToFlip.clear();
                    }
                } 
            }
            moveFound = false; //reset this flag
        }
    }
    /*Error Checking
    std::cerr << "Number of legal moves:" << legalMoveIdx<< std::endl;
    for (int k = 0; k < tilesToFlip.size(); k++)
    {
        std::cerr<< "Move " << k << ":";
        for (int l = 0; l < tilesToFlip[k].size(); l++)
        {
            std::cerr<< " " << tilesToFlip[k][l].first <<"," <<tilesToFlip[k][l].second ;
        }
        std::cerr<<std::endl;
    }
    */
   return legalMoveIdx;
}

void Board::applyMove(int playerNum, int moveChosen, bool showDisplay){
    //if (moveChosen > tilesToFlip.size() ){
    if (tilesToFlip[moveChosen].empty() || (moveChosen > legalMoveIdx) ){ //first half of statement may be unnecessary
        if (tilesToFlip[moveChosen].empty()){
            std::cerr << "tilesToFlip is empty" << std::endl;
        }
        else{   
            std::cerr << "Move Chosen: " << moveChosen << " legalIdx" << legalMoveIdx << std::endl;
            std::cerr << "Invalid move. Please select a valid move." << std::endl;
        }
        return;
    }

    for (int i = 0; i < tilesToFlip[moveChosen].size(); i++){
        std::pair<int, int> tile = tilesToFlip[moveChosen][i];
        setTile(tile.first, tile.second, playerNum);
    }
    if (showDisplay){
        displayBoard();
    }
    else{
        //std::cerr << "display suppressed" << endl;
    }
}