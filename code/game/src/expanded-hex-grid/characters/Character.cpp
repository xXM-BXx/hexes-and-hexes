//
// Created by mburc on 06.07.2025.
//

#pragma once
#include "Character.h"
#include <utility>
#include <random>

#include "../../Game.h"
#include "actions/Action.h"
#include "actions/MoveAction.h"

namespace hnh::game {
    void Character::draw(glm::mat3 &parentTransform) const {
        auto transform = parentTransform * getTransform();
        characterSprite.draw(transform);
    }

    bool Character::executeAction(HexCoords target) {
        auto action = actions[selectedAction].get();
        if(action) {
            return actions[selectedAction]->execute(target);
        }
        return false;
    }
    void Character::markAction(HexCoords target) {
        actions[selectedAction]->markTiles(target);
    }

    void Character::setSelectedAction(const std::string &actionName) {
        selectedAction = actionName;
    }

    bool Character::getIsDead() const {
        return isDead;
    }

    void Character::onTurnEnd() {
        for (auto& action: actions) {
            action.second->onTurnEnd();
        }
    }

    glm::mat3 Character::getTransform() const {
        auto transform = glm::mat3(1.0f);
        // Scale first (diagonal entries)
        transform[0][0] = scale; // X scale
        transform[1][1] = scale; // Y scale

        // Then translate (rightmost column)
        transform[2][0] = position.x;
        transform[2][1] = position.y;
        return transform;
    }

    std::string Character::getAnimationNameForDirection(HexCoords from, HexCoords to) {
        auto direction = to - from;
        if(direction ==  HexCoords(0, -1)) {
            return "top";
        }
        if(direction ==  HexCoords(1, -1)) {
            return "top_right";
        }
        if(direction ==  HexCoords(1, 0)) {
            return "bottom_right";
        }
        if(direction ==  HexCoords(0, 1)) {
            return "bottom";
        }
        if(direction ==  HexCoords(-1, 1)) {
            return "bottom_left";
        }
        if(direction ==  HexCoords(-1, 0)) {
            return "top_left";
        }
        return "idle";
    }

    Character::Character(std::shared_ptr<engine::AtlasTexture> atlasTexture):
    characterSprite(std::move(atlasTexture)){
        actions["move"] = std::make_unique<MoveAction>(this);
        selectedAction = "move";
    }

    void Character::decide() {
        //printf("Decide was called on a character, that does not Decide");
        Game::instance->animationFinished();
    }

    int Character::getHealth() const {
        return health;
    }

    bool Character::takeDamage(int damage) {
        health -= damage;
        if(!isDead) Game::instance->playHurtSound();
        if(health <= 0) {
            //printf("Character died!");
            isDead = true;
            visible = false;
            return true;
        }
        if(health > maxHealth) {
            health = maxHealth;
        }
        return false;
    }

    void Character::checkForSuffocationDamage() {
        auto test = Game::instance->queue;
        if(!isDead && Game::instance->getHexGrid()->getTileOrNull(hexPosition)->isCover()) {
            //If in terrain, push out and do damage
            //check for increasing distances
            int maxDistance = 5;
            auto availableTiles = HexCoords::floodFill(hexPosition, maxDistance, [](const HexCoords &start) {
                return start.neighborCoords();
            });
            int depth = -1;
            for (const auto& ring: availableTiles) {
                depth++;
                for (auto position: ring) {
                    if(Game::instance->getHexGrid()->getTileOrNull(position)->isWalkable()) {
                        if(!takeDamage(depth)) teleport(position);
                        return;
                    }
                }
            }
            takeDamage(100);
        }
    }

    void Character::move(HexCoords targetPosition) {
        auto path = HexCoords::findPath(this->hexPosition, targetPosition, Game::instance->getMarkedTiles());
        characterSprite.forceStopAnimation();

        auto firstNextTile = Game::instance->getHexGrid()->getTileOrNull(path[1]);
        firstNextTile->characterShadow = this;
        Game::instance->playStepSound();
        for (size_t i = 1; i < path.size() - 1; ++i) {
            HexCoords from = path[i - 1];
            HexCoords to = path[i];
            std::string anim = getAnimationNameForDirection(from, to);
            auto nextTile = Game::instance->getHexGrid()->getTileOrNull(to);
            auto afterNextTile = Game::instance->getHexGrid()->getTileOrNull(path[i + 1]);
            characterSprite.enqueueAnimationStep(anim, [this, to, nextTile, afterNextTile]() {
                Game::instance->playStepSound();
                nextTile->characterShadow = nullptr;
                this->changeHexPosition(to);
                afterNextTile->characterShadow = this;
            });
        }
        HexCoords from = path[path.size() - 2];
        HexCoords to = path[path.size() - 1];

        std::string anim = getAnimationNameForDirection(from, to);

        characterSprite.enqueueAnimationStep(anim, [this, to]() {
            Game::instance->getHexGrid()->getTileOrNull(to)->characterShadow = nullptr;
            this->changeHexPosition(to);
            Game::instance->animationFinished();
        });
        characterSprite.enqueueAnimationStep("idle");
    }

    void Character::teleport(HexCoords targetPosition) {
        changeHexPosition(targetPosition);
    }

    void Character::animateSpell(std::function<void()> effect) {
        characterSprite.forceStopAnimation();
        characterSprite.enqueueAnimationStep("don_staff", []() {
            Game::instance->playCastSound();
        });
        characterSprite.enqueueAnimationStep("cast", std::move(effect));
        characterSprite.enqueueAnimationStep("doff_staff", [this]() {
            if (Game::instance) {
                Game::instance->animationFinished();
            }
        });
        characterSprite.enqueueAnimationStep("idle");
    }

    void Character::initHexPosition(HexCoords newPosition) {
        auto tile = Game::instance->getHexGrid()->getTileOrNull(newPosition);
        if(!tile) {
            //printf("Target Hex does not exist");
            return;
        }
        this->hexPosition = newPosition;
        characterSprite.setAtlasCoords({0, 0});
    }

    HexCoords Character::changeHexPosition(HexCoords newPosition) {
        auto currentTile = Game::instance->getHexGrid()->getTileOrNull(hexPosition);
        auto nextTile = Game::instance->getHexGrid()->getTileOrNull(newPosition);
        if(!nextTile) {
            //printf("Target Hex does not exist");
            return hexPosition;
        }
        nextTile->character = std::move(currentTile->character);
        hexPosition = newPosition;
        characterSprite.setAtlasCoords({0, 0});
        return hexPosition;
    }

    HexCoords Character::getHexPosition() const {
        return hexPosition;
    }

    engine::AnimatedSprite * Character::getCharacterSprite() {
        return &characterSprite;
    }
}
