//
// Created by 洪至德 on 2020/6/11.
//

#include <iostream>
#include "PlayerAgent.h"


int PlayerAgent::get_actions(const std::vector<char> &state, char color, std::vector<int> possibles) {
    if (possibles.empty()) {
        return 100;
    }
    int x, y;
    std::cin >> x >> y;
    return x*8+y;
}

PlayerAgent::PlayerAgent() {

}
