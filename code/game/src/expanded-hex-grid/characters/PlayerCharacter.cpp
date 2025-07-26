//
// Created by mburc on 07.07.2025.
//

#pragma once
#include "PlayerCharacter.h"
#include "Character.h"
#include "actions/Action.h"
#include "actions/MoveAction.h"
#include "actions/LineAttack.h"

#include <utility>

#include "../../Game.h"
#include "actions/Lifesteal.h"
#include "actions/PlayerAoE.h"
#include "actions/PlayerSmallAoE.h"
#include "actions/SingleAttack.h"
#include "actions/WallAction.h"


namespace hnh::game {

    PlayerCharacter::PlayerCharacter() : Character(engine::Engine::get().getResourceManager()->loadResource<engine::AtlasTexture>(
            "sprites/PlayerCharacter.png", glm::ivec2(16, 16), "json/atlas-textures/CharacterAtlas.json")) {
        //Create Players available Actions

        actions["move"] = std::make_unique<MoveAction>(this, 4);
        actions["line_attack"] = std::make_unique<LineAttack>(this);
        actions["wall"] = std::make_unique<WallAction>(this);
        actions["single_target"] = std::make_unique<SingleAttack>(this);
        actions["lifesteal"] = std::make_unique<Lifesteal>(this);
        actions["aoe"] = std::make_unique<PlayerAoE>(this);
        actions["small_aoe"] = std::make_unique<PlayerSmallAoE>(this);

        health = 10;
        maxHealth = 99;
        maxActions = 2;
        remainingActions = 2;

    }

    bool PlayerCharacter::takeDamage(int damage) {
        auto died = Character::takeDamage(damage);
        if(died) {
            Game::instance->setGameOver();
        }
        return died;
    }
}
