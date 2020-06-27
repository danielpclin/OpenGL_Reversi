//
// Created by 洪至德 on 2020/6/10.
//

#include "Reversi.h"
#include <iostream>


//Reversi::Reversi(PlayerAgent player1, PlayerAgent player2): player1(player1), player2(player2) {
////    reversiEnv = new Reversi_env();
//}
Reversi::Reversi(Agent& agent1, Agent& agent2 ): agent1(agent1), agent2(agent2) {
//    reversiEnv = new Reversi_env();
}


//Reversi::~Reversi(){
//    free(reversiEnv);
//}

void Reversi::start() {
    char color;
    std::vector<char> actions;
    int action, result;
    Reversi_env::draw_board(reversiEnv.board);
    reversiEnv.reset();
    while (!reversiEnv.done()) {
        reversiEnv.set_possible_moves();
        for (int & i : reversiEnv.get_possible_moves()) {
            std::cout << Reversi_env::index_to_position(i).first << " " << Reversi_env::index_to_position(i).second << std::endl;
        }
        actions = state;
        (reversiEnv.board.color) ? color = 0 : color = 1;
        action = agent1.get_actions(state, color, reversiEnv.get_possible_moves());
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
        actions = state;
        (reversiEnv.board.color) ? color = 0 : color = 1;
        action = agent1.get_actions(state, color, reversiEnv.get_possible_moves());
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


