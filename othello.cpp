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
    int player= 0; //first player is black
    int moveChosen=-1, numOfLegalMoves;
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
            //cerr << moveChosen;
            if (playerIsAI[player]){ 
                start = std::chrono::steady_clock::now();
                int depth = 1;
                int theresTimeLeft = 999, bestMove;
                do{
                    bestMove = theresTimeLeft;
                    theresTimeLeft = miniMax(board, depth, INT_MIN, INT_MAX, player, true); // @
                    //cerr << "Minimax returns: "<< theresTimeLeft << endl;
                    depth++;
                }
                while(theresTimeLeft > 0);
                cout << "Searched to depth " << depth - 1  << "." << endl;
                //moveChosen = bestMove;
                moveChosen = 1;
                /* 
                // if player number of AI, choose a random move for now. replaced w/ minimax
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
        cerr << "number of pieces " << board.piecesOnBoard << endl;
    }
}

int miniMax(Board prevBoard, int depth, int alpha, int beta, int isMax, bool completeLayer){
    // seed randomness
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distLegalMoves(0,1); // distribution in range [0, 1]
    
    // check if out of time
    double time_elapsed = since(start).count();
    //cerr << "Time elapsed:" << time_elapsed << endl;
    //if (time_elapsed >= (aiTimeLimit *1000) ){
    if (time_elapsed >= (2*1000) ){
        return -1; // signal out of time
    }
    Board foreseenBoard = prevBoard; // make copy of previous board
    int maxEval = INT_MIN, minEval = INT_MAX;
    int eval, numOfLegalMoves, bestMove;
    if (depth == 0 || foreseenBoard.consecTurnsSkipped >=2 || foreseenBoard.piecesOnBoard >=64){
        return heuristic(isMax); // are isMax and player num correlated?
    }
    if (isMax){
        numOfLegalMoves= foreseenBoard.legalMoves(isMax, SHOW_DISPLAY); // @
        for (int potentialMove = 0; potentialMove < numOfLegalMoves; potentialMove++){
            foreseenBoard.applyMove(isMax,potentialMove, SHOW_DISPLAY); // apply one move to temp board, then pass to minimax
            eval = miniMax(foreseenBoard, depth - 1, alpha, beta, 0, SHOW_DISPLAY); //pass to min
            if (eval > maxEval){
                maxEval = eval;
                bestMove = potentialMove;
            }
            else if (eval == maxEval){
                int cointoss = distLegalMoves(rng);
                if (cointoss){
                    bestMove= potentialMove;
                }
            }
            alpha = max(alpha, eval);
            if (beta <= alpha){ break; }
        }
        int retVal = completeLayer? maxEval: bestMove;
        return retVal;
    }
    else{
        numOfLegalMoves = foreseenBoard.legalMoves(1-isMax, SHOW_DISPLAY);
        for (int potentialMove = 0; potentialMove < numOfLegalMoves; potentialMove++){
            foreseenBoard.applyMove(isMax,potentialMove, SHOW_DISPLAY); // apply one move to temp board, then pass to minimax
            eval = miniMax(foreseenBoard, depth - 1, alpha, beta, 1, SHOW_DISPLAY); // pass to max
            if (eval < minEval){
                minEval = eval;
                bestMove = potentialMove;
            }
            else if (minEval ==eval){
                int cointoss = distLegalMoves(rng);
                if (cointoss){
                    bestMove = potentialMove;
                }
            }
            beta = min(beta, eval);
            if (beta <= alpha){ break; }
        }
        int retVal = completeLayer? minEval: bestMove;
        return retVal;
    }
    //return -29; // should never happen
}

int heuristic(int isMax){
    return 1;
}