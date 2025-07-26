#pragma once
#include "Explode.h"

#include "../Character.h"
#include "../../../Game.h"

namespace hnh::game {
    Explode::Explode(Character *owner): Action(owner) {
        currentCooldown = 1;
    }

    std::vector<HexCoords> Explode::markTiles(HexCoords target) {
        return engine::Utility::flattenVec2D(HexCoords::floodFill(owner->getHexPosition(), 2, [](const HexCoords &h) -> std::vector<HexCoords> {
            return h.neighborCoords();
        }));
    }

    bool Explode::execute(HexCoords target) {
        auto targets = markTiles(target);
        if(currentCooldown != 0) { return false; }
        owner->animateSpell([targets]() {
            for (auto target: targets) {
                auto tile = Game::instance->getHexGrid()->getTileOrNull(target);
                if (tile) {
                    tile->getObstacle()->setObstacleType(Obstacle::Explosion);
                    if (tile->character) tile->character->takeDamage(1);
                }
            }
        });
        return true;
    }
}
