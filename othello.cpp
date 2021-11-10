#include <iostream>
#include <string>
#include <fstream>
#include "board.h"

using namespace std;

int main(){
    string s, boardLoadFile = "";
    int whoseTurn = 1, aiTimeLimit = 5;
    int row, col;

    /* ask to load a game state */
    boardLoadFile= "sampleBoard1";

    /* init board */
    board Board = board();
    if (boardLoadFile!=""){ // skip loading preload if no file is given
        ifstream input;
        input.open(boardLoadFile.c_str() );
        for (row = 0; row < 8; row++){
            getline (input, s);
            for (col = 0; col < 8; col++){
                Board.setTile(row, col, s[col <<1]); //skip spaces
            }
        }
        getline(input, s);
        whoseTurn = s[0] -'1'; //player 1 = 0, player 2 = 1
        getline(input, s);
        aiTimeLimit=atoi(s.c_str() );
        input.close();
    }
        
    /* Display */
    Board.displayBoard();
    return 0;
}