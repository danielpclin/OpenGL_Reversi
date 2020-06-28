#include <iostream>
#include "Board.h"

Board::Board() {
    length = 8;
    color = white;
}

void Board::reset() {
    if (map.empty()) {
        for (int idx=0; idx<length*length; idx++) {
            map.push_back('-');
        }
    } else {
        for (int idx=0; idx<length*length; idx++) {
            map[idx] = '-';
        }
    }
    map[28] = 'O';
    map[35] = 'O';
    map[27] = 'X';
    map[36] = 'X';
}

void Board::swap_color() {
    switch (color) {
        case white:
            color = black;
            break;
        case black:
            color = white;
            break;
        default:
            break;
    }
}

std::pair<int, int> Board::get_score() {
    int white=0, black=0;
    for (char & i : map) {
        if (i == 'O') white++;
        else if (i == 'X') black++;
    }
    return std::make_pair(white, black);
}

