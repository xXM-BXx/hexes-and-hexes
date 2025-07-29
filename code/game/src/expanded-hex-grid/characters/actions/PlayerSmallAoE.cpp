#pragma once
#include "PlayerSmallAoE.h"

#include "../../../Game.h"

namespace hnh::game {
    PlayerSmallAoE::PlayerSmallAoE(Character *owner)
        : Action(owner) {
        displayName = "Burst";
        maxCooldown = 3;
        displayDescription = "Damage enemies in\n"
                "a small area you can\n"
                "see.\n"
                "Cooldown:3 Activations";
    }
    std::vector<HexCoords> PlayerSmallAoE::markTiles(HexCoords target) {
        bool hasLineOfSight = HexCoords::hasLineOfSight(owner->getHexPosition(), target, [](HexCoords coord) {
            return Game::instance->getHexGrid()->getTileOrNull(coord)->isCover();
        });
        if (!hasLineOfSight) {
            Game::instance->markTiles(std::vector<HexCoords>());
            return std::vector<HexCoords>();
        }

        auto area = HexCoords::floodFill(target, 2, [](HexCoords coord) {
            return coord.neighborCoords();
        });
        auto flatArea = engine::Utility::flattenVec2D(area);
        Game::instance->markTiles({flatArea});
        return {flatArea};
    }

    bool PlayerSmallAoE::execute(HexCoords target) {
        auto targets = PlayerSmallAoE::markTiles(target);
        if(targets.empty() || currentCooldown != 0) return false;
        currentCooldown = maxCooldown;
        owner->animateSpell([targets]() {
            for (auto target: targets) {
                auto targetTile = Game::instance->getHexGrid()->getTileOrNull(target);
                if(targetTile) {
                    targetTile->getObstacle()->setObstacleType(Obstacle::Explosion);
                    if(targetTile->character) targetTile->character->takeDamage(1);
                }
            }
        });
        return true;
    }
}
