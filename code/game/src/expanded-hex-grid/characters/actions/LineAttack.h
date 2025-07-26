#pragma once
#include "Action.h"

namespace hnh::game {
    class LineAttack : public Action {
    public:
        explicit LineAttack(Character *owner);

        ///Get the nearest axis between the owner and the target, and follow that axis, until it reaches cover.
        std::vector<HexCoords> markTiles(HexCoords target) override;
        ///Set the \ref Obstacle on the marked tiles to crystal.
        bool execute(HexCoords target) override;
    };
}
