#ifndef REVERSI_REVERSI_H
#define REVERSI_REVERSI_H


#include "Agent.h"
#include "Reversi_env.h"

class Reversi {
public:
    Reversi(Agent& agent1, Agent& agent2);
//    Reversi(PlayerAgent player1, PlayerAgent player2);
//    ~Reversi();
    void init();
    void play();
    Board getBoard();
private:
    Agent &agent1, &agent2;
    Reversi_env reversiEnv;
};


#endif //REVERSI_REVERSI_H
