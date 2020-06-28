#include <cstdio>
#include <utility>
#include "ComputerAgent.h"
#include "Reversi_env.h"

int best_action = -1;

int ComputerAgent::get_action(Board board, int color, std::vector<int> possibles) {
    if (possibles.empty()) {
        return 100;
    }
    for (int i : possibles) {
        if (i == 0 || i == 7 || i ==56 || i == 63)
            return i;
    }
    int depth = 3;
    best_action = -1;
    minimax(board, color, depth, depth, true);
    return best_action;
}

int ComputerAgent::minimax(Board board, int color, int search_depth, int current_depth, bool maximizing=true) {
    // color 0 white 1 black
    if(current_depth == 0){
        return eval_board(std::move(board), color);
    }
    int best_value;
    if(maximizing){
        best_value = -2147483646;
        std::vector<int> valid_actions = Reversi_env::get_possible_moves(board);
        for(int action : valid_actions){
            Board tmp_board = Reversi_env::make_place(board, action);
            int tmp = minimax(tmp_board, search_depth, current_depth - 1, !maximizing);
            if(tmp >best_value){
                if (search_depth == current_depth)
                    best_action = action;
            }
        }
    }else{
        best_value = 2147483646;
        std::vector<int> valid_actions = Reversi_env::get_possible_moves(board);
        for(int action : valid_actions){
            Board tmp_board = Reversi_env::make_place(board, action);
            int tmp = minimax(tmp_board, search_depth, current_depth - 1, !maximizing);
            if(tmp < best_value){
                if (search_depth == current_depth)
                    best_action = action;
            }
        }
    }
    return best_value;
}

int ComputerAgent::eval_board(Board board, int color) {
    int blackPieces = 0, whitePieces = 0, corner_bonus = 10;
    for(char c : board.map){
        if(c == 'X')
            blackPieces++;
        if(c == 'O')
            whitePieces++;
    }
    if(board.map[0] == 'X')
        blackPieces += corner_bonus;
    if(board.map[0] == 'O')
        whitePieces += corner_bonus;
    if(board.map[7] == 'X')
        blackPieces += corner_bonus;
    if(board.map[7] == 'O')
        whitePieces += corner_bonus;
    if(board.map[56] == 'X')
        blackPieces += corner_bonus;
    if(board.map[56] == 'O')
        whitePieces += corner_bonus;
    if(board.map[63] == 'X')
        blackPieces += corner_bonus;
    if(board.map[63] == 'O')
        whitePieces += corner_bonus;
    if(color == 0){
        return whitePieces - blackPieces;
    }else{
        return blackPieces - whitePieces;
    }
}
