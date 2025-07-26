//
// Created by mburc on 13.07.2025.
//

#pragma once

#include "QueueVisualizationItem.h"

#include "../../Game.h"
#include "engine/ui/UIBox.h"

namespace hnh::game {
    QueueVisualizationItem::QueueVisualizationItem(Character *character):
    characterSprite(character->getCharacterSprite()->atlasTexture),
    characterPosition(character->getHexPosition()){
        this->character = character;
        auto box = std::make_unique<engine::ui::UIButton>(Game::instance->uiBoxTexture, glm::ivec2{0,0}, glm::ivec2((16 + 4) * 4, 32 * 4));
        box->setHoveredTexture(Game::instance->uiButtonHoveredTexture);
        addChild(std::move(box));
        updateUI();
    }

    void QueueVisualizationItem::draw(glm::mat3 &parentTransform) const {
        if (!visible) return;
        UIObject::draw(parentTransform);
        auto transform = parentTransform * calculateLocalMatrix();
        glm::mat3 offsetFix = {
            1, 0, 0,
            0, 1, 0,
            -14, -3, 1
        };
        auto spriteTransform = transform* offsetFix;
        characterSprite.draw(spriteTransform, true);
    }

    void QueueVisualizationItem::setCharacter(Character *character) {
        this->character = character;
    }

    void QueueVisualizationItem::updateUI() {
        children.front()->setOnEnterCallback([&]() {
            auto newPos =  Game::instance->getHexGrid()->gridToWorld(characterPosition);
            Game::instance->mouseHighlight->position = newPos;
        });
    }
}
