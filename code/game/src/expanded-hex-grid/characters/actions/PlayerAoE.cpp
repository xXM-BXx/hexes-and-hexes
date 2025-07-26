//
// Created by mburc on 18.07.2025.
//

#pragma once
#include "PlayerAoE.h"

#include "../../../Game.h"

namespace gl3::game {
    PlayerAoE::PlayerAoE(Character *owner) : Action(owner) {
        displayName = "Explosion";
        maxCooldown = 7;
        displayDescription = "Damage enemies in\n"
                "an area you can\n"
                "see.\n"
                "Cooldown:7 Activations";
    }

    std::vector<HexCoords> PlayerAoE::markTiles(HexCoords target) {
        bool hasLineOfSight = HexCoords::hasLineOfSight(owner->getHexPosition(), target, [](HexCoords coord) {
            return Game::instance->getHexGrid()->getTileOrNull(coord)->isCover();
        });
        if (!hasLineOfSight) {
            Game::instance->markTiles(std::vector<HexCoords>());
            return std::vector<HexCoords>();
        }

        auto area = HexCoords::floodFill(target, 3, [](HexCoords coord) {
            return coord.neighborCoords();
        });
        auto flatArea = engine::Utility::flattenVec2D(area);
        Game::instance->markTiles({flatArea});
        return {flatArea};
    }

    bool PlayerAoE::execute(HexCoords target) {
        auto targets = PlayerAoE::markTiles(target);
        if(targets.empty() || currentCooldown != 0) return false;
        currentCooldown = maxCooldown;
        owner->animateSpell([targets]() {
            for (auto target: targets) {
                auto targetTile = Game::instance->getHexGrid()->getTileOrNull(target);
                if(targetTile) {
                    targetTile->getObstacle()->setObstacleType(Obstacle::Explosion);
                    if(targetTile->character) targetTile->character->takeDamage(2);
                }
            }
        });
        return true;
    }
}
