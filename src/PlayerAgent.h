#ifndef REVERSI_PLAYERAGENT_H
#define REVERSI_PLAYERAGENT_H


#include "Agent.h"
#include <queue>

class PlayerAgent : public Agent{
public:
    PlayerAgent();
    int get_action(Board board, char color, std::vector<int> possibles) override;
    bool readClicks = false;
    std::queue<std::pair<double, double>> clicks;
};


#endif //REVERSI_PLAYERAGENT_H
