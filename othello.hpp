#ifndef OTHELLO_H 
#define OTHELLO_H

#include "board.hpp"
#include <chrono>

void playGame(int firstPlayer, Board &board);

clock_t beginTime, endTime;
template <
    class result_t   = std::chrono::milliseconds,
    class clock_t    = std::chrono::steady_clock,
    class duration_t = std::chrono::milliseconds
>
auto since(std::chrono::time_point<clock_t, duration_t> const& start)
{
    return std::chrono::duration_cast<result_t>(clock_t::now() - start);
}
int aiTimeLimit;

int miniMax(Board possibleBoard, int depth, int alpha, int beta, int isMax, bool completeLayer);
//int heuristic(int isMax);

bool playerIsAI[1];
#endif