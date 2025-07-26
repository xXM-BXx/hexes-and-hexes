//
// Created by mburc on 13.07.2025.
//

#pragma once
#include "PlayerHealthVisualization.h"

#include "../../Game.h"
#include "engine/core/Engine.h"

namespace gl3::game {
    PlayerHealthVisualization::PlayerHealthVisualization() : healthText(Game::instance->defaultFont, U"20"),
                                                             scoreText(Game::instance->defaultFont, U"Score:?"),
                                                             heartSprite(
                                                                 engine::Engine::get().getResourceManager()->
                                                                 loadResource<engine::AtlasTexture>(
                                                                     "sprites/Heart.png", glm::ivec2(12, 12))) {
        scale = 4;

        heartSprite.position = {3, 2};
        healthText.position = {16, 1};


        std::unique_ptr<engine::ui::UIBox> scoreBox = std::make_unique<engine::ui::UIBox>(
            Game::instance->uiBoxTexture, glm::ivec2{32, 0}, glm::ivec2{300, 50});
        scoreText.position = scoreBox->position + glm::vec2{4, 1};
        addChild(std::move(scoreBox));


        engine::Engine::get().getResourceManager()->loadResource<engine::AtlasTexture>("sprites/Heart.png")->
                MakeAtlasItem({0, 0}, {1, 1});
        std::unique_ptr<engine::ui::UIBox> healthBox = std::make_unique<engine::ui::UIBox>(
            Game::instance->uiBoxTexture, glm::ivec2{0, 0}, glm::ivec2{120, 50});
        addChild(std::move(healthBox));
    }

    void PlayerHealthVisualization::draw(glm::mat3 &parentTransform) const {
        if (!visible) return;

        UIObject::draw(parentTransform);

        auto transform = parentTransform * calculateLocalMatrix();
        healthText.draw(transform);
        heartSprite.draw(transform, true);
        scoreText.draw(transform);
    }

    void PlayerHealthVisualization::updateUI() {
        int health = 0;
        if (Game::instance->getPlayerCharacter())
            health = Game::instance->getPlayerCharacter()->getHealth();
        auto wide = std::to_wstring(health);
        healthText.setText({wide.begin(), wide.end()});

        int score = Game::instance->getScore();
        wide = std::to_wstring(score);
        std::u32string scoreString = U"Score:" + std::u32string(wide.begin(), wide.end());
        scoreText.setText(scoreString);
    }
}
