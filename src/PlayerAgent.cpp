#include <iostream>
#include "PlayerAgent.h"


int PlayerAgent::get_action(Board board, char color, std::vector<int> possibles) {
    if (possibles.empty()) {
        return 100;
    }
    readClicks = true;
    int x, y;
    while (true){
        if (!clicks.empty()){
            if(true){
                x = 3;
                y = 2;

                break;
            }
        }
    }
    clicks.clear();
    readClicks = false;
    return y*8+x;
}

PlayerAgent::PlayerAgent() {

}