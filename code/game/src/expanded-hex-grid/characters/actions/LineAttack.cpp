//
// Created by mburc on 09.07.2025.
//

#pragma once
#include "LineAttack.h"
#include "../../../Game.h"


namespace gl3::game {
    LineAttack::LineAttack(Character *owner) : Action(owner) {
        displayName = "Line Attack";
        displayDescription =
                "Spawn Crystals in\n"
                "a straight line.\n"
                "The Crystals deal 1\n"
                "damage and persist for\n"
                "10 Activations.\n"
                "Cooldown: 5 Activations";
        maxCooldown = 5;
    }


    std::vector<HexCoords> LineAttack::markTiles(HexCoords target) {
        auto snappeddirection = HexCoords::getNearestNormalizedAxis(owner->getHexPosition(), target);
        auto maxLength = owner->getHexPosition().distanceTo(target);
        auto tiles = HexCoords::followAxis(owner->getHexPosition(), snappeddirection, [](HexCoords c) {
            return !Game::instance->getHexGrid()->getTileOrNull(c)->isCover();
        }, maxLength);
        Game::instance->markTiles({tiles});
        return tiles;
    }

    bool LineAttack::execute(HexCoords target) {
        if (currentCooldown != 0) return false;
        auto tiles = markTiles(target);
        owner->animateSpell([&, tiles]() {
            for (auto position: tiles) {
                auto tile = Game::instance->getHexGrid()->getTileOrNull(position);
                if (tile->terrain == 0) {
                    tile->getObstacle()->setObstacleType(Obstacle::Crystal);
                }
            }
        });
        currentCooldown = maxCooldown;
        Game::instance->animationFinished();
        return true;
    }
}
