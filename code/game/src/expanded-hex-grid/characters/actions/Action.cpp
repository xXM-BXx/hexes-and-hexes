//
// Created by mburc on 06.07.2025.
//

#pragma once
#include "Action.h"


namespace hnh::game {
    Action::Action(Character *owner) : owner(owner){
    }

    void Action::onTurnEnd() {
        if(currentCooldown > 0) currentCooldown--;
    }
}
