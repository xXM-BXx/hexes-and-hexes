#pragma once
#include "Action.h"

namespace hnh::game {

class PlayerSmallAoE : public Action{
public:
    explicit PlayerSmallAoE(Character *owner);

    ///Mark an area with radius 2 around a target, that has line of sight to the owner.
    std::vector<HexCoords> markTiles(HexCoords target) override;
    ///Set the \ref Obstacle on the marked tiles to explosion.
    bool execute(HexCoords target) override;
};

}
