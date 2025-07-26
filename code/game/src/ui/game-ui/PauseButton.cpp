//
// Created by mburc on 16.07.2025.
//

#pragma once
#include "PauseButton.h"

#include "../../Game.h"
#include "engine/core/Engine.h"
#include "engine/ui/UIButton.h"

namespace gl3::game {
    PauseButton::PauseButton(): pauseSymbol(
        engine::Engine::get().getResourceManager()->loadResource<engine::AtlasTexture>(
            "sprites/PauseSymbol.png", glm::ivec2{6, 10})) {
        engine::Engine::get().getResourceManager()->loadResource<engine::AtlasTexture>(
            "sprites/PauseSymbol.png")->MakeAtlasItem({0, 0}, {1, 1});

        scale = 4;

        auto size = glm::ivec2{60, 60};
        auto position = glm::ivec2(engine::Engine::get().getRenderResolution().x - size.x, 0);
        this->position = position - glm::ivec2{6, -4};

        auto pauseButton = std::make_unique<engine::ui::UIButton>(Game::instance->uiBoxTexture);
        pauseButton->setOnclickSound("audio/ButtonPress.wav");
        pauseButton->setHoveredTexture(Game::instance->uiButtonHoveredTexture);
        pauseButton->size = size;
        pauseButton->setOnClickCallback([]() {
            Game::instance->pauseGame();
        });
        addChild(std::move(pauseButton));

        pauseSymbol.position = {5, 3};
    }

    void PauseButton::draw(glm::mat3 &parentTransform) const {
        if(!visible) return;
        UIObject::draw(parentTransform);
        auto tranform = parentTransform * calculateLocalMatrix();
        pauseSymbol.draw(tranform, true);
    }
}
