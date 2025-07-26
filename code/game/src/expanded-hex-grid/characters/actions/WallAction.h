//
// Created by mburc on 18.07.2025.
//

#pragma once
#include "Action.h"

namespace hnh::game {

class WallAction : public Action {
public:
    explicit WallAction(Character *owner);

    ///Get the closest axis to the vector between owner and target.
    ///Get the angle of that axis.
    ///Get a clamped distance between owner and target.
    ///For each point in a pattern, translate, then rotate the owner position based on angle and distance.
    ///Mark this transformed pattern.
    std::vector<HexCoords> markTiles(HexCoords target) override;
    ///Set the \ref Obstacle on the marked tile to crystal.
    bool execute(HexCoords target) override;
};

}
