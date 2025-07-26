#pragma once
#include "Action.h"
#include "../Character.h"


namespace hnh::game {

    class MoveAction : public Action {
    public:
        int range;
        explicit MoveAction(Character* owner, int range = 3);
        ///Mark tiles within a flood fill from the owners position with the range as depth and tile.isWalkable() as stop condition.
        std::vector<HexCoords> markTiles(HexCoords target) override;
        ///If the target is within the marked tiles, move the character there.
        bool execute(HexCoords target) override;
    };

}
