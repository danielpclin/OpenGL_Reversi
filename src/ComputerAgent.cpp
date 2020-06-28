#include "ComputerAgent.h"

int ComputerAgent::get_action(Board board, char color, std::vector<int> possibles) {
    if (possibles.empty()) {
        return 100;
    }
    return possibles[0];
}
