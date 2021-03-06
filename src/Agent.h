#ifndef REVERSI_AGENT_H
#define REVERSI_AGENT_H


#include <vector>
#include "Board.h"

class Agent {
public:
    virtual int get_action(Board board, int color, std::vector<int> possibles) = 0;
};


#endif //REVERSI_AGENT_H
