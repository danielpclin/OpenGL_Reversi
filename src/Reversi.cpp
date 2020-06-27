#include "Reversi.h"
#include <iostream>

Reversi::Reversi(Agent& agent1, Agent& agent2 ): agent1(agent1), agent2(agent2) {
}

Board Reversi::getBoard(){
    return reversiEnv.board;
}

void Reversi::play() {
    char color;
    int action, result;
    while (!reversiEnv.done()) {
        reversiEnv.set_possible_moves();
        for (int & i : reversiEnv.get_possible_moves()) {
            std::cout << Reversi_env::index_to_position(i).first << " " << Reversi_env::index_to_position(i).second << std::endl;
        }
        (reversiEnv.board.color) ? color = 0 : color = 1;
        action = agent1.get_action(reversiEnv.board, color, reversiEnv.get_possible_moves());
        result = reversiEnv.step(action);
        if (result == 4 or !result) {
            std::cout << "You lose!" << std::endl;
            break;
        } else if (result == 2) {
            std::cout << "Pass!" << std::endl;
        } else {
            Reversi_env::draw_board(reversiEnv.board);
        }

        reversiEnv.set_possible_moves();
        for (int & i : reversiEnv.get_possible_moves()) {
            std::cout << Reversi_env::index_to_position(i).first << " " << Reversi_env::index_to_position(i).second << std::endl;
        }
        (reversiEnv.board.color) ? color = 0 : color = 1;
        action = agent2.get_action(reversiEnv.board, color, reversiEnv.get_possible_moves());
        result = reversiEnv.step(action);
        if (result == 4 or !result) {
            std::cout << "You lose!" << std::endl;
            break;
        } else if (result == 2) {
            std::cout << "Pass!" << std::endl;
        } else {
            Reversi_env::draw_board(reversiEnv.board);
        }
    }
    if(reversiEnv.done()) {
        std::pair<int, int> scores = reversiEnv.board.get_score();
        std::cout << "P1: " << scores.first << " vs P2: " << scores.second << std::endl;
    }
}

void Reversi::init(){
    reversiEnv.reset();
    Reversi_env::draw_board(reversiEnv.board);
}
