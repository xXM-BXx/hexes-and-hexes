//
// Created by mburc on 18.07.2025.
//

#pragma once
#include "ExplodingEnemy.h"

#include "../../../Game.h"
#include "../actions/Explode.h"
#include "../actions/MoveAction.h"

namespace hnh::game {
    ExplodingEnemy::ExplodingEnemy() : BasicEnemy(engine::Engine::get().getResourceManager()->loadResource<engine::AtlasTexture>(
            "sprites/ExplodingEnemy.png", glm::ivec2(16, 16), "json/atlas-textures/CharacterAtlas.json")) {
        actions["move"] = std::make_unique<MoveAction>(this, 3);
        actions["explode"] = std::make_unique<Explode>(this);
    }

    void ExplodingEnemy::decide() {
        HexCoords playerPos;
        if(Game::instance->getPlayerCharacter())
        playerPos = Game::instance->getPlayerCharacter()->getHexPosition();

        //First Try to explode
        if((playerPos - getHexPosition()).length() <= 1) {
            selectedAction = "explode";
            executeAction(getHexPosition());
            return;
        }

        //Get as close as possible to the player(run against wall)
        auto distanceToPlayer = (playerPos - getHexPosition()).length();
        auto availableMovement = actions["move"]->markTiles(getHexPosition());
        HexCoords bestPos = getHexPosition();
        for (auto option: availableMovement) {
            if((playerPos - option).length() < distanceToPlayer) {
                distanceToPlayer = (playerPos - option).length();
                bestPos = option;
            }
        }
        if(bestPos != getHexPosition()) {
            selectedAction = "move";
            executeAction(bestPos);
            return;
        }
        selectedAction = "explode";
        executeAction(getHexPosition());
    }
}
