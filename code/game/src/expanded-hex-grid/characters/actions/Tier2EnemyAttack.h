#pragma once
#include "Action.h"

namespace hnh::game {

class Tier2EnemyAttack : public Action {
public:
    explicit Tier2EnemyAttack(Character *owner);
    ///Mark a tile, that has line of sight to the owner.
    std::vector<HexCoords> markTiles(HexCoords target) override;
    ///Set the \ref Obstacle on the marked tile to explosion.
    bool execute(HexCoords target) override;
};

}
