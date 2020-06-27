//
// Created by 洪至德 on 2020/6/11.
//

#ifndef REVERSI_PLAYERAGENT_H
#define REVERSI_PLAYERAGENT_H


#include "Agent.h"

class PlayerAgent : public Agent{
public:
    PlayerAgent();
    int get_actions(const std::vector<char>& state, char color, std::vector<int> possibles) override;
private:
};


#endif //REVERSI_PLAYERAGENT_H
