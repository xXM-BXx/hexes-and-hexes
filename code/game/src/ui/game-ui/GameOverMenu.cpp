//
// Created by mburc on 17.07.2025.
//

#pragma once
#include "GameOverMenu.h"

#include "../../Game.h"

namespace gl3::game {
    GameOverMenu::GameOverMenu()
        : gameOverText(Game::instance->defaultFont, U"Placeholder", {4, 1}) {
        scale = 4;

        glm::ivec2 boxSize = {500, 400};
        auto box = std::make_unique<engine::ui::UIBox>(Game::instance->uiBoxTexture);
        box->size = boxSize;
        addChild(std::move(box));

        this->position = engine::Engine::get().getRenderResolution() / 2 - boxSize / 2;

        //Create Main Menu Button with Text
        auto returnButton = std::make_unique<engine::ui::UIButton>(Game::instance->uiBoxTexture);
        returnButton->setHoveredTexture(Game::instance->uiButtonHoveredTexture);
        returnButton->setOnclickSound("audio/ButtonPress.wav");
        returnButton->size = {420, 50};
        returnButton->position = {10, 80};
        auto returnText = std::make_unique<engine::ui::UIText>(Game::instance->defaultFont, U"To Main Menu",
                                                               glm::ivec2{4, 1});
        returnButton->addChild(std::move(returnText));
        returnButton->setOnClickCallback([]() {
            Game::instance->toMainMenu();
        });
        addChild(std::move(returnButton));
    }

    void GameOverMenu::draw(glm::mat3 &parentTransform) const {
        if (!visible) return;
        UIObject::draw(parentTransform);
        auto transform = parentTransform * calculateLocalMatrix();
        gameOverText.draw(transform);
    }

    void GameOverMenu::updateUI() {
        int currentScore = Game::instance->getScore();
        int highScore = Game::instance->highScore;
        std::u32string message = U"Message empty";
        std::wstring wideScore = std::to_wstring(currentScore);

        if (currentScore > highScore) {
            message = U"Game Over!\n"
                      "-----------------\n"
                      "You set a new\n"
                      "Highscore of "
                      + std::u32string{wideScore.begin(), wideScore.end()} +
                      U"!";
        } else {
            message = U"Game Over!\n"
                      "-----------------\n"
                      "You died with a\n"
                      "Score of "
                      + std::u32string{wideScore.begin(), wideScore.end()} +
                      U"!\n";
        }

        gameOverText.setText(message);
    }
}
