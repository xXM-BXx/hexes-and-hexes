//
// Created by mburc on 18.07.2025.
//

#pragma once
#include "SingleAttack.h"

#include "../Character.h"
#include "../../../Game.h"

namespace hnh::game {
    SingleAttack::SingleAttack(Character *owner) : Action(owner) {
        maxCooldown = 0;
        displayName = "Attack";
        displayDescription = "Create an explosion\n"
                "on one tile with\n"
                "line of sight.\n"
                "Cooldown: 0 Activations";
    }

    std::vector<HexCoords> SingleAttack::markTiles(HexCoords target) {
        bool hasLineOfSight = HexCoords::hasLineOfSight(owner->getHexPosition(), target, [](HexCoords coord) {
           return Game::instance->getHexGrid()->getTileOrNull(coord)->isCover();
        });
        if(hasLineOfSight) {
            Game::instance->markTiles({target});
            return{target};
        }
        std::vector<HexCoords> emptyResult;
        Game::instance->markTiles(emptyResult);
        return {emptyResult};
    }

    bool SingleAttack::execute(HexCoords target) {
        auto targets = markTiles(target);
        if(targets.empty() || currentCooldown != 0) { return false; }
        owner->animateSpell([&, targets]() {
            for (auto position: targets) {
                auto tile = Game::instance->getHexGrid()->getTileOrNull(position);
                tile->getObstacle()->setObstacleType(Obstacle::Explosion);
                if(tile->character) tile->character->takeDamage(3);
            }
        });
        currentCooldown = maxCooldown;
        return true;
    }
}
