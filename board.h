//#include <iostream>
#include <vector>
#include <string>

class board{
    public:
    board();
    void displayBoard();
    //void setBoard(char* input);
    void setTile(int row, int col, int newVal);
    private:
    std:: vector <std::vector<char> > updatedBoard;
};