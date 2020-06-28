#ifndef REVERSI_REVERSI_H
#define REVERSI_REVERSI_H


#include "Agent.h"
#include "Reversi_env.h"

class Reversi {
public:
    Reversi(Agent& agent1, Agent& agent2);
    void init();
    void play();
    bool gameEnd();
    std::vector<int> possibleMoves();
    Board getBoard() const;
private:
    Agent &agent1, &agent2;
    Reversi_env reversiEnv;
};


#endif //REVERSI_REVERSI_H
