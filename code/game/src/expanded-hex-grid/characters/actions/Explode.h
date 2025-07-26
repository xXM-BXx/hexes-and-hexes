#pragma once
#include "Action.h"

namespace hnh::game {

class Explode : public Action {
public:
    explicit Explode(Character *owner);
    ///Mark a radius of 1 around the owners position, regardless of target.
    std::vector<HexCoords> markTiles(HexCoords target) override;

    ///Set the \ref Obstacle on the marked tiles to explosion.
    bool execute(HexCoords target) override;
};

}
