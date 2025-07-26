//
// Created by mburc on 18.07.2025.
//

#include "WallAction.h"

#include "../Character.h"
#include "../../../Game.h"

namespace gl3::game {
    WallAction::WallAction(Character *owner) : Action(owner) {
        maxCooldown = 5;
        displayName = "Wall";
        displayDescription = "Create a Wall of\n"
                "Crystals close to \n"
                "you.\n"
                "The Crystals deal 1\n"
                "damage and persist for\n"
                "10 Activations.\n"
                "Cooldown: 5 Activations";
    }

    
    std::vector<HexCoords> WallAction::markTiles(HexCoords target) {
        auto relativeTarget = target - owner->getHexPosition();
        auto snappedDirection = HexCoords::getNearestNormalizedAxis(owner->getHexPosition(), target);
        int maxRange = 3;
        auto length = std::min(relativeTarget.length(), maxRange);
        if (length == 0) {
            Game::instance->markTiles({});
            return {};
        }

        auto angle = snappedDirection.angleFromAxis();
        std::vector<HexCoords> shape = {
            {0, 0},
            {-1, 1},
            {-2, 2},
            {1, 0},
            {2, 0}
        };
        std::vector<HexCoords> result;
        for (auto position: shape) {
            result.emplace_back(
                owner->getHexPosition() + (HexCoords(0, -1) * length).rotatedBy(angle) + (position.rotatedBy(angle)));
        }
        Game::instance->markTiles(result);
        return result;
    }

    bool WallAction::execute(HexCoords target) {
        if (currentCooldown != 0) return false;
        auto targets = markTiles(target);
        if (targets.empty()) { return false; }
        owner->animateSpell([&, targets]() {
            for (auto position: targets) {
                auto tile = Game::instance->getHexGrid()->getTileOrNull(position);
                tile->getObstacle()->setObstacleType(Obstacle::Crystal);
            }
        });
        currentCooldown = maxCooldown;
        return true;
    }
}
