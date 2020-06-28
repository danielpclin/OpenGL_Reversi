#ifndef REVERSI_REVERSI_ENV_H
#define REVERSI_REVERSI_ENV_H


#include "Board.h"

class Reversi_env {
public:
    static void draw_board(Board board);
    bool done() const;
    Board board;
    Reversi_env();
    int step(int action);
    std::vector<char> reset();
    std::vector<int> get_possible_moves();
    static std::pair<int, int> index_to_position(int index);
    void set_possible_moves();
    static std::vector<int> get_possible_moves(Board board);
    static Board make_place(Board board, int action);
private:
    enum step_results{invalid = 0, success = 1, pass = 2, game_over = 3, you_lose = 4};
    static int position_to_index(std::pair<int, int> position);
    int pass_count;
    bool is_valid(int index);
    std::vector<int> possible_moves;
};


#endif //REVERSI_REVERSI_ENV_H
