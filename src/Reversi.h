//
// Created by 洪至德 on 2020/6/10.
//

#ifndef REVERSI_REVERSI_H
#define REVERSI_REVERSI_H


#include "PlayerAgent.h"
#include "Reversi_env.h"

class Reversi {
public:
    Reversi(Agent& agent1, Agent& agent2);
//    Reversi(PlayerAgent player1, PlayerAgent player2);
//    ~Reversi();
    void start();
private:
    Agent &agent1, &agent2;
    Reversi_env reversiEnv;
    std::vector<char, std::allocator<char>> state;
};


#endif //REVERSI_REVERSI_H
