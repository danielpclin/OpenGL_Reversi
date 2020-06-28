#include <iostream>
#include "Reversi_env.h"


void Reversi_env::draw_board(Board board) {
    for (int idx=0; idx<board.map.size(); idx++) {
        std::cout << board.map[idx];
        if ((idx+1)%8 == 0) std::cout << std::endl;
    }
}


Reversi_env::Reversi_env() {
    board.reset();
    pass_count=0;
}


int Reversi_env::step(int action) {
    std::pair<int, int> position = index_to_position(action);
    int pos_x, pos_y, temp_x, temp_y, count;
    char current, opponent;
    bool flipped = false;
    if (board.color == board.white) {
        current = 'O';
        opponent = 'X';
    } else {
        current = 'X';
        opponent = 'O';
    }
    if (done()) return game_over;
    else if (action == 100){
        if (possible_moves.empty()){
            board.swap_color();
            pass_count++;
            return pass;
        }else{
            return you_lose;
        }
    }
    else if (!is_valid(position_to_index(std::make_pair(position.first, position.second)))) return you_lose;
    else {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                pos_x = position.first + dx;
                pos_y = position.second + dy;
                count = 0;
                if (pos_x >= 8 || pos_x < 0 || pos_y >= 8 || pos_y < 0) continue;
                while (board.map[position_to_index(std::make_pair(pos_x, pos_y))] == opponent) {
                    temp_x = pos_x + dx;
                    temp_y = pos_y + dy;
                    if (temp_x >= 8 || temp_x < 0 || temp_y >= 8 || temp_y < 0) break;
                    count++;
                    pos_x += dx;
                    pos_y += dy;
                }
                if (count > 0 && board.map[position_to_index(std::make_pair(pos_x, pos_y))] == current) {
                    board.map[position_to_index(std::make_pair(position.first, position.second))] = current;
                    pos_x = position.first+dx;
                    pos_y = position.second+dy;
                    flipped = true;
                    while (board.map[position_to_index(std::make_pair(pos_x, pos_y))] == opponent) {
                        board.map[position_to_index(std::make_pair(pos_x, pos_y))] = current;
                        pos_x += dx;
                        pos_y += dy;
                    }
                    board.map[position_to_index(std::make_pair(pos_x, pos_y))] = current;
                }
            }
        }
        board.swap_color();
        if (flipped) return success;
    }
    return invalid;
}

std::pair<int, int> Reversi_env::index_to_position(int index) {
    return std::make_pair((index)/8, (index)%8);
}

int Reversi_env::position_to_index(std::pair<int, int> position) {
    return position.first*8+position.second;
}

bool Reversi_env::done() const {
    return pass_count == 2;
}

std::vector<char> Reversi_env::reset() {
    board.reset();
    return board.map;
}

std::vector<int> Reversi_env::get_possible_moves() {
    return possible_moves;
}

bool Reversi_env::is_valid(int index) {
    for (int & i : possible_moves) {
        if (i == index) return true;
    }
    return false;
}

void Reversi_env::set_possible_moves() {
    std::vector<int> possibles;
    for (int i=0; i<board.map.size(); i++) {
        std::pair<int, int> position = index_to_position(i);
        int pos_x = position.first, pos_y = position.second, count, temp_x, temp_y;
        char current, opponent;
        if (board.color == board.white) {
            current = 'O';
            opponent = 'X';
        } else {
            current = 'X';
            opponent = 'O';
        }
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                pos_x = position.first + dx;
                pos_y = position.second + dy;
                count = 0;
                if (pos_x >= 8 || pos_x < 0 || pos_y >= 8 || pos_y < 0) continue;
                while (board.map[position_to_index(std::make_pair(pos_x, pos_y))] == opponent) {
                    temp_x = pos_x + dx;
                    temp_y = pos_y + dy;
                    if (temp_x >= 8 || temp_x < 0 || temp_y >= 8 || temp_y < 0) break;
                    count++;
                    pos_x += dx;
                    pos_y += dy;
                }
                if (count > 0 && board.map[position_to_index(std::make_pair(pos_x, pos_y))] == current && board.map[position_to_index(std::make_pair(position.first, position.second))] == '-') {
                    bool is_found = false;
                    for (int & idx : possibles) {
                        if (idx == i) is_found = true;
                    }
                    if (!is_found) possibles.push_back(i);
                }
            }
        }
    }
    possible_moves = possibles;
}
