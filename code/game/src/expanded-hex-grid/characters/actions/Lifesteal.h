//
// Created by mburc on 18.07.2025.
//

#pragma once
#include "Action.h"

namespace hnh::game {

class Lifesteal : public Action {
public:
    explicit Lifesteal(Character *owner);

    ///Mark an area with radius 3 around a target, that has line of sight to the owner.
    std::vector<HexCoords> markTiles(HexCoords target) override;
    ///Set the \ref Obstacle on the marked tiles to lifesteal, if there is a character on it.
    ///Set the \ref Obstacle on the owners tile to heal and heal the owner.
    bool execute(HexCoords target) override;
};

}
