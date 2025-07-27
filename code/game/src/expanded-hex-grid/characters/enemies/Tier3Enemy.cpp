#include "Tier3Enemy.h"

#include "../../../Game.h"
#include "../actions/MoveAction.h"
#include "../actions/Tier3EnemyAoE.h"
#include "../actions/Tier3EnemySingle.h"

namespace hnh::game {
    Tier3Enemy::Tier3Enemy() : BasicEnemy(
        engine::Engine::get().getResourceManager()->loadResource<engine::AtlasTexture>(
            "sprites/Tier3Enemy.png", glm::ivec2(16, 16), "json/atlas-textures/CharacterAtlas.json")) {
        maxActions = 2;
        remainingActions = maxActions;
        actions["move"] = std::make_unique<MoveAction>(this, 4);
        actions["single_attack"] = std::make_unique<Tier3EnemySingle>(this);
        actions["aoe_attack"] = std::make_unique<Tier3EnemyAoE>(this);
    }

    void Tier3Enemy::decide() {
        HexCoords playerPos;
        if (Game::instance->getPlayerCharacter())
            playerPos = Game::instance->getPlayerCharacter()->getHexPosition();

        //See if can target player with single hit
        if (!actions["single_attack"]->markTiles(playerPos).empty()) {
            selectedAction = "single_attack";
            executeAction(playerPos);
            return;
        }

        //Attempt to find a tile, where player can be targeted
        auto moveOptions = actions["move"]->markTiles(playerPos);
        HexCoords bestOption;
        int distanceToPlayer = 0;
        for (auto option: moveOptions) {
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

        //Then see if can hit player with AoE
        auto inPlayerRange = HexCoords::floodFill(playerPos, 3, [](HexCoords h) {
            return h.neighborCoords();
        });
        auto flatInPlayerRange = engine::Utility::flattenVec2D(inPlayerRange);
        int casualties = 100;
        HexCoords bestTarget;
        for (auto option: flatInPlayerRange) {
            auto hitPositions = actions["aoe_attack"]->markTiles(option);
            if(hitPositions.empty()) continue;
            int potentialCasualties = 0;
            for (auto position: hitPositions) {
                if (Game::instance->getHexGrid()->getTileOrNull(position)->character) {
                    potentialCasualties++;
                }
            }
            if (potentialCasualties < casualties) {
                bestTarget = option;
                casualties = potentialCasualties;
            }
        }
        if(casualties != 100) {
            selectedAction = "aoe_attack";
            executeAction(bestTarget);
            return;
        }

        //Now try to get away from the player
        bestOption = {};
        distanceToPlayer = 0;
        for (auto option: moveOptions) {
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
