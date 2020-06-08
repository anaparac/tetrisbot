#pragma once

#include "Board.h"
#include <utility>

using namespace std;

pair<int, int> agent(Board board_, int  mPieces_);





int number_of_holes(Board board_);
int clear_lines(Board board_);
int sum_height(Board board_);
int blockades(Board board_);



