//
// Created by mburc on 18.07.2025.
//

#pragma once
#include "Tier3EnemyAoE.h"

#include "../../../Game.h"

namespace gl3::game {
    Tier3EnemyAoE::Tier3EnemyAoE(Character *owner) : Action(owner) {
        maxCooldown = 0;
    }

    std::vector<HexCoords> Tier3EnemyAoE::markTiles(HexCoords target) {
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

    bool Tier3EnemyAoE::execute(HexCoords target) {
        auto targets = Tier3EnemyAoE::markTiles(target);
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
