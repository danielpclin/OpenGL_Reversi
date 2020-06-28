#ifndef OPENGL_PRACTICE_COMPUTERAGENT_H
#define OPENGL_PRACTICE_COMPUTERAGENT_H


#include "Agent.h"

class ComputerAgent : public Agent{
public:
    int get_action(Board board, int color, std::vector<int> possibles) override;
private:
    static int eval_board(Board board, int color);
    static int minimax(Board board, int color, int search_depth, int current_depth, bool maximizing);
};


#endif //OPENGL_PRACTICE_COMPUTERAGENT_H
