//
// Created by 洪至德 on 2020/6/9.
//

#ifndef REVERSI_BOARD_H
#define REVERSI_BOARD_H
#include <vector>

class Board {
public:
    Board();
    enum player_color {white = 0, black = 1};
    std::pair<int, int> get_score();
    void swap_color();
    void reset();
    int length;
    int color;
    std::vector<char> map;
};


#endif //REVERSI_BOARD_H
