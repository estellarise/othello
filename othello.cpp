#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <climits>
#include <ctime>
#include <chrono>
#include "board.hpp"
#include "othello.hpp"

#define SHOW_DISPLAY false
using namespace std;

/*Clock*/
auto start = std::chrono::steady_clock::now();

int main(){
    string s, boardLoadFile = "";
    int firstPlayer;
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
            cout << "Would you like to go first? (Y/n): ";
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
        
    //cerr << firstPlayer << " " << aiTimeLimit << endl;

    /* Display */
    playGame(firstPlayer, board);
    return 0;
}

void playGame(int firstPlayer, Board &board){
    int player= 0; //first player is black
    int moveChosen=-1, numOfLegalMoves, depth;
    string userMoveChosen;
    bool moveIsValid;

    board.displayBoard();
    while (board.consecTurnsSkipped < 2 && board.piecesOnBoard < 64){
        numOfLegalMoves=board.legalMoves(player, true);
        //cerr << "Num of piecesOnBoard: " << piecesOnBoard  << endl;
        if (numOfLegalMoves == 0){ //check if moves are available
            board.consecTurnsSkipped+=1;
            cout <<"No moves, skipping turn. "<< board.consecTurnsSkipped << endl;
            player= 1-player;
            continue; //skip to next iteration of whilel loop
        }

        if (numOfLegalMoves == 1){ 
            cout << "Only one move, applying the move automatically." <<endl;
            moveChosen = 1;
        }
        else{ //if more than one move left
            if (playerIsAI[player]){ 
                start = std::chrono::steady_clock::now();
                depth = 0;
                int theresTimeLeft = 999, bestMove;
                do{
                    depth++;
                    bestMove = theresTimeLeft;
                    //cerr << "Minimax re-entry, depth " << depth << endl;
                    theresTimeLeft = miniMax(board, depth, INT_MIN, INT_MAX, player, true); // @
                    //cerr << "Minimax returns: "<< theresTimeLeft << endl;
                } while(theresTimeLeft > 0);
                cout << "Time searched: " << since(start).count()<< "ms." << endl;
                cout << "Searched to depth " << depth << "." << endl; //-1 ?
                moveChosen = bestMove;
                //cerr << "othello Move Chosen" << moveChosen << endl;
                //moveChosen = 1;

                /*
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distLegalMoves(1,board.legalMoveIdx); // distribution in range [0, 1]
    moveChosen= distLegalMoves(rng);
                */
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
        board.applyMove(player, moveChosen, true);
        player = 1-player; //give turn to other player
        board.piecesOnBoard++;
        board.consecTurnsSkipped = 0; //reset
        moveChosen = -1; //reset
        //cerr << "number of pieces " << board.piecesOnBoard << endl;
    }
}

int miniMax(Board prevBoard, int depth, int alpha, int beta, int isMax, bool completeLayer){
    //cerr << "miniMax called by " << (isMax? "max white":"min black") << endl;
    // seed randomness
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distLegalMoves(0,1); // distribution in range [0, 1]
    
    // check if out of time
    double time_elapsed = since(start).count();
    //cerr << "Time elapsed:" << time_elapsed << endl;
    //if (time_elapsed >= (aiTimeLimit *1000 -1) ){
    if (time_elapsed >= (10) ){
        return -1; // signal out of time
    }
    int maxEval = INT_MIN, minEval = INT_MAX;
    int eval, numOfLegalMoves, bestMove, minRetVal, maxRetVal;
    if (depth == 0 || prevBoard.consecTurnsSkipped >=2 || prevBoard.piecesOnBoard >=64){
        cerr << "out of depth, returning" << endl;
        return prevBoard.heuristic(isMax); // are isMax and player num correlated?
    }
    
    numOfLegalMoves= prevBoard.legalMoves(isMax, SHOW_DISPLAY); // @
    if (isMax){
        for (int potentialMove = 1; potentialMove <= numOfLegalMoves; potentialMove++){
            Board foreseenBoard = prevBoard; // make copy of previous board
            //std::cerr << "  potential Move Chosen: " << potentialMove;
            cerr << " legalIdx " << numOfLegalMoves<< std::endl;
            foreseenBoard.applyMove(isMax,potentialMove, SHOW_DISPLAY); // apply one move to temp board, then pass to minimax
            /*
            int numTilesFlipped = foreseenBoard.tilesToFlip[potentialMove].size();
            foreseenBoard.numPieces[isMax]+= numTilesFlipped;
            foreseenBoard.numPieces[1-isMax]-= numTilesFlipped-1; //change to minus 1 if off
            cerr<< "num white " << foreseenBoard.numPieces[1] << " num black " << foreseenBoard.numPieces [0] << endl;
            */

            eval = miniMax(foreseenBoard, depth - 1, alpha, beta, 0, SHOW_DISPLAY); //pass to min
            if (eval > maxEval){
                maxEval = eval;
                bestMove = potentialMove;
            }
            else if (eval == maxEval){
                //int coinToss = distLegalMoves(rng);
                int coinToss = 0;
                if (coinToss){
                    bestMove= potentialMove;
                }
            }
            alpha = max(alpha, eval);
            if (beta <= alpha){ break; }
        }
        maxRetVal = completeLayer? bestMove:maxEval;
        return maxRetVal;
    }
    else{
        for (int potentialMove = 1; potentialMove <= numOfLegalMoves; potentialMove++){
            Board foreseenBoard = prevBoard; // make copy of previous board
            /*
            int numTilesFlipped = foreseenBoard.tilesToFlip[potentialMove].size();
            foreseenBoard.numPieces[isMax]+= numTilesFlipped;
            foreseenBoard.numPieces[1-isMax]-= numTilesFlipped-1; //change to minus 1 if off
            cerr<< "num white " << foreseenBoard.numPieces[1] << " num black " << foreseenBoard.numPieces [0] << endl;
            */
            //std::cerr << "  potential Move Chosen: " << potentialMove;
            //cerr << " legalIdx" << numOfLegalMoves<< std::endl << endl;
            foreseenBoard.applyMove(isMax,potentialMove, SHOW_DISPLAY); // apply one move to temp board, then pass to minimax
            eval = miniMax(foreseenBoard, depth - 1, alpha, beta, 1, SHOW_DISPLAY); // pass to max
            if (eval < minEval){
                minEval = eval;
                bestMove = potentialMove;
            }
            else if (eval == minEval){
                //int coinToss = distLegalMoves(rng);
                int coinToss = 0;
                if (coinToss){
                    bestMove = potentialMove;
                }
            }
            beta = min(beta, eval);
            if (beta <= alpha){ break; }
        }
        minRetVal = completeLayer? bestMove:minEval;
        return minRetVal;
    }
    //return -29; // should never happen
}