#include "MoveAction.h"

#include "../../../Game.h"


namespace hnh::game {
    MoveAction::MoveAction(Character *owner, int range) : Action(owner), range(range) {
        maxCooldown = 0;
        displayName = "Move";
        displayDescription = "Move to any point\n"
                             "within 3 tiles.\n"
                             "Respects terrain.\n"
                             "Cooldown: 0 Activations";
    }

    std::vector<HexCoords> MoveAction::markTiles(HexCoords target) {
        auto neighborFunc = [&](const HexCoords &start) -> std::vector<HexCoords> {
            std::vector<HexCoords> eligibleNeighbors;
            for (auto neighbor: start.neighborCoords()) {
                if(auto tile = Game::instance->getHexGrid()->getTileOrNull(neighbor))
                    if(tile->isWalkable())
                        eligibleNeighbors.push_back(neighbor);
            }
            return eligibleNeighbors;
        };
        auto tiles = HexCoords::floodFill(owner->getHexPosition(), range, neighborFunc);
        tiles.erase(tiles.begin());
        auto tilesFlattened = engine::Utility::flattenVec2D(tiles);
        Game::instance->markTiles(tilesFlattened);
        return tilesFlattened;
    }


        bool MoveAction::execute(HexCoords target) {
            auto eligibleTiles = markTiles(target);
            if(std::find(eligibleTiles.begin(), eligibleTiles.end(), target) != eligibleTiles.end()) {
                owner->move(target);
                currentCooldown = maxCooldown;
                return true;
            }
            return false;
        }
    }
