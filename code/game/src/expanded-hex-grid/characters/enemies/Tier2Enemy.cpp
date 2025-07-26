//
// Created by mburc on 18.07.2025.
//

#pragma once
#include "Tier2Enemy.h"

#include "../../../Game.h"
#include "../actions/Tier2EnemyAttack.h"

namespace gl3::game {
    Tier2Enemy::Tier2Enemy() : BasicEnemy(
        engine::Engine::get().getResourceManager()->loadResource<engine::AtlasTexture>(
            "sprites/Tier2Enemy.png", glm::ivec2(16, 16), "json/atlas-textures/CharacterAtlas.json")) {
        actions["attack"] = std::make_unique<Tier2EnemyAttack>(this);
    }

    void Tier2Enemy::decide() {
        HexCoords playerPos;
        if (Game::instance->getPlayerCharacter())
            playerPos = Game::instance->getPlayerCharacter()->getHexPosition();

        //See if can target player
        if (!actions["attack"]->markTiles(playerPos).empty()) {
            selectedAction = "attack";
            executeAction(playerPos);
            return;
        }

        //Attempt to find a tile, where player can be targeted
        auto options = actions["move"]->markTiles(playerPos);
        HexCoords bestOption;
        int distanceToPlayer = 0;
        for (auto option: options) {
            if (HexCoords::hasLineOfSight(option, playerPos, [](HexCoords h) {
                return Game::instance->getHexGrid()->getTileOrNull(h)->isCover();
            })) {
                auto newDistance = (option - playerPos).length();
                if (newDistance > distanceToPlayer) {
                    bestOption = option;
                    distanceToPlayer = newDistance;
                }
            };
        }
        if (distanceToPlayer != 0) {
            selectedAction = "move";
            executeAction(bestOption);
            return;
        }


        //Now try to get away from the player
        bestOption = {};
        distanceToPlayer = 0;
        for (auto option: options) {
            auto newDistance = (option - playerPos).length();
            if (newDistance > distanceToPlayer) {
                bestOption = option;
                distanceToPlayer = newDistance;
            }
        }
        selectedAction = "move";
        if(executeAction(bestOption)) return;
        Game::instance->animationFinished();
    }
}
