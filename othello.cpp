#include <iostream>
#include <string>
#include <fstream>
#include "board.h"

using namespace std;

int main(){
    string s, boardLoadFile = "cleanBoard";
    int whoseTurn = 1, aiTimeLimit = 5;
    int row, col;

    /* ask to load a game state */
    boardLoadFile= "sampleBoard3";

    /* init board */
    board Board = board();

    if (boardLoadFile!="cleanBoard"){ // skip loading preload if no file is given
        ifstream input;
        input.open(boardLoadFile.c_str() );
        for (row = 0; row < 8; row++){
            getline (input, s);
            for (col = 0; col < 8; col++){
                Board.setTile(row, col, s[col <<1]); //skip spaces
            }
        }
        getline(input, s);
        whoseTurn = s[0] -'0'; //player 1 = 0, player 2 = 1
        getline(input, s);
        aiTimeLimit=atoi(s.c_str() );
        input.close();
    }
        
    /* Display */
    Board.displayBoard();
    //Board.legalMoves(1); //white
    Board.legalMoves(1); //black
    return 0;
}