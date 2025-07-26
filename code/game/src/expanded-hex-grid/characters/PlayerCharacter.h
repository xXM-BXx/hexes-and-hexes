//
// Created by mburc on 07.07.2025.
//

#pragma once
#include "Character.h"

namespace hnh::game {

class PlayerCharacter : public Character {
public:
    ///Creates a character with player specific parameters, such as different texture and actions.
    PlayerCharacter();
    ///takeDamage variant, that can end the game on death.
    bool takeDamage(int damage) override;
    static const unsigned int scoreValue = 0;
};

}
