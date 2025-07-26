#pragma once
#include "Lifesteal.h"

#include "../../../Game.h"

namespace hnh::game {
    Lifesteal::Lifesteal(Character *owner) : Action(owner) {
        displayName = "Lifesteal";
        maxCooldown = 10;
        displayDescription = "Damage enemies in\n"
                "an area you can\n"
                "see and heal based\n"
                "based on the hit\n"
                "enemies.\n"
                "Cooldown:10 Activations";
    }

    std::vector<HexCoords> Lifesteal::markTiles(HexCoords target) {
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

    bool Lifesteal::execute(HexCoords target) {
        auto targets = Lifesteal::markTiles(target);
        if(targets.empty() || currentCooldown != 0) return false;
        currentCooldown = maxCooldown;
        owner->animateSpell([targets, this]() {
            int healAmount = 0;
            for (auto target: targets) {
                auto targetTile = Game::instance->getHexGrid()->getTileOrNull(target);
                if(targetTile && targetTile->character != nullptr) {
                    targetTile->getObstacle()->setObstacleType(Obstacle::Lifesteal);
                    targetTile->character->takeDamage(1);
                    healAmount++;
                }
            }
            if(healAmount > 0) {
                Game::instance->getHexGrid()->getTileOrNull(owner->getHexPosition())->getObstacle()->setObstacleType(Obstacle::Heal);
                owner->takeDamage(-healAmount);
            }
        });
        return true;
    }
}
