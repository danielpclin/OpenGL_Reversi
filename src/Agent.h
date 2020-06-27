//
// Created by 洪至德 on 2020/6/10.
//

#ifndef REVERSI_AGENT_H
#define REVERSI_AGENT_H


#include <vector>
#include "Board.h"

class Agent {
public:
    virtual int get_action(Board board, char color, std::vector<int> possibles) = 0;
};


#endif //REVERSI_AGENT_H
