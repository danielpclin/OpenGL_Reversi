#ifndef OPENGL_PRACTICE_COMPUTERAGENT_H
#define OPENGL_PRACTICE_COMPUTERAGENT_H


#include "Agent.h"

class ComputerAgent : public Agent{
public:
    int get_action(Board board, char color, std::vector<int> possibles) override;
};


#endif //OPENGL_PRACTICE_COMPUTERAGENT_H
