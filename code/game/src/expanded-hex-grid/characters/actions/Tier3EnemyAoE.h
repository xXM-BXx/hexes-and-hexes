#pragma once
#include "Action.h"

namespace hnh::game {
    class Tier3EnemyAoE : public Action {
    public:
        explicit Tier3EnemyAoE(Character *owner);
        ///Mark an area with radius 3 around a target, that has line of sight to the owner.
        std::vector<HexCoords> markTiles(HexCoords target) override;
        ///Set the \ref Obstacle on the marked tiles to explosion.
        bool execute(HexCoords target) override;
    };
}
