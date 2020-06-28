#include <iostream>
#include "PlayerAgent.h"
#include <algorithm>

int PlayerAgent::get_action(Board board, int color, std::vector<int> possibles) {
    if (possibles.empty()) {
        return 100;
    }
    readClicks = true;
    for(auto i : possibles){
        std::cout << i <<std::endl;
    }
    int x, y;
    while (true){
        if (!clicks.empty()){
            x = (clicks.front().first + 0.8)/0.2;
            y = 8 - (clicks.front().second + 0.8)/0.2;
            clicks.pop();
            if(x >=0 && x < 8 && y >= 0 && y < 8){
                if(std::find(possibles.begin(), possibles.end(), y*8+x) != possibles.end()){
                    clicks = std::queue<std::pair<double, double>>();
                    readClicks = false;
                    return y*8+x;
                }
            }
        }
    }

}

PlayerAgent::PlayerAgent() {

}