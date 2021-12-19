#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include "board.hpp"
#include "othello.hpp"

using namespace std;

int main(){
    string s, boardLoadFile = "";
    int firstPlayer, aiTimeLimit;
    int row, col;
    string YorN;
    bool userVSaiMode;

    /* ask to load a game state */
    cout << "Load a game file? (y/N):";
    cin >> YorN;
    if (YorN[0] == 'y' || YorN[0] == 'Y'){
        cout << "Please enter name of file:" ;
        cin >> boardLoadFile;
        //boardLoadFile= "sampleBoard4"; //DEL
    }
    else{ // Let user choose parameters
        boardLoadFile = "cleanBoard";
        cout << "Let two computers battle it out? (Y/n): ";
        cin >> YorN;
        userVSaiMode = (YorN[0] == 'n' || YorN[0] == 'N');
        if (userVSaiMode){
            cout << "Would you like to go first? (Y/n)";
            cin >> YorN;
            playerIsAI[!(YorN[0] == 'n' || YorN[0] == 'N')] = true;
            playerIsAI[(YorN[0] == 'n' || YorN[0] == 'N')] = false;
            cout << "Time limit on AI: ";
            cin >> s;
            aiTimeLimit = atoi(s.c_str() );
        }
        else{ //assign one AI as max, one as min
            playerIsAI[0] = true;
            playerIsAI[1] = true;
            cout << playerIsAI[0];
            cout << playerIsAI[1];
            firstPlayer = 0;
            aiTimeLimit = 5;
        }
    }

    /* init board */
    Board board = Board();
    ifstream input;
    input.open(boardLoadFile.c_str() );
    for (row = 0; row < 8; row++){
        getline (input, s);
        for (col = 0; col < 8; col++){
            char tileNo = s[col << 1]; //skip spaces
            if (tileNo != '0'){
                board.setTile(row, col, tileNo -'1'); //player 1 = 0, player 2 = 1
            }
            else{
                board.setTile(row, col, 2); //define empty space as 2
            }
        }
    }
    if (boardLoadFile!="cleanBoard"){
        getline(input, s);
        firstPlayer = s[0]-'1'; //player 1 = 0, player 2 = 1
        getline(input, s);
        aiTimeLimit=atoi(s.c_str() );
    }
    input.close();
        
    cerr << firstPlayer << " " << aiTimeLimit << endl;

    /* Display */
    playGame(firstPlayer, board);
    //board.displayBoard();
    //board.legalMoves(0); //black
    //board.applyMove(0, 2);
    /*
    board.applyMove(1, 55);
    board.applyMove(1, 92);
    */
    return 0;
}

void playGame(int firstPlayer, Board &board){
    int consecTurnsSkipped = 0, piecesOnBoard = 4, player= 0; //first player is black
    int moveChosen=-1, numOfLegalMoves;
    string userMoveChosen;
    bool moveIsValid;

    board.displayBoard();
    while (consecTurnsSkipped < 2 && piecesOnBoard < 64){
        numOfLegalMoves=board.legalMoves(player);
        //cerr << "Num of piecesOnBoard: " << piecesOnBoard  << endl;
        if (numOfLegalMoves == 0){ //check if moves are available
            consecTurnsSkipped+=1;
            cout <<"No moves, skipping turn. "<< consecTurnsSkipped << endl;
            player= 1-player;
            continue;
        }
        if (numOfLegalMoves == 1){ 
            cout << "Only one move, applying the move automatically." <<endl;
            moveChosen = 1;
        }
        else{
            if (playerIsAI[player]){ // if player number of AI, choose a random move for now. replace w/ search
                std::random_device dev;
                std::mt19937 rng(dev());
                std::uniform_int_distribution<std::mt19937::result_type> distLegalMoves(1,numOfLegalMoves); // distribution in range [1, 6]
                moveChosen = distLegalMoves(rng);
            }
            else{
                moveIsValid = true;
                while (!(moveIsValid) || moveChosen < 1 || moveChosen > numOfLegalMoves){ //keep prompting user until a valid move # is selected
                    cout << "Choose a move between 1 and " << numOfLegalMoves << ": ";
                    cin >> userMoveChosen;
                    for (int c = 0; c < userMoveChosen.size(); c++){
                        if (!isdigit(userMoveChosen[c]) ){ //if any of the user input is not a number, invalid move 
                            moveIsValid = false;                            
                            break;
                        }
                    }
                    if (moveIsValid){
                        moveChosen = stoi(userMoveChosen);
                    }
                }
            }
        }
        cout << "Move Chosen: " << moveChosen << endl;
        board.applyMove(player, moveChosen);
        player = 1-player; //give turn to other player
        piecesOnBoard++;
        consecTurnsSkipped = 0; //reset
        moveChosen = 0;
    }
}