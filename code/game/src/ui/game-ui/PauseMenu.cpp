#include "PauseMenu.h"

#include "../../Game.h"

namespace hnh::game {
    PauseMenu::PauseMenu() {
        scale = 4;

        //Create Big Box
        glm::ivec2 bigBoxSize = {500, 250};
        auto bigBox = std::make_unique<engine::ui::UIBox>(Game::instance->uiBoxTexture);
        bigBox->size = bigBoxSize;
        addChild(std::move(bigBox));


        this->position = engine::Engine::get().getRenderResolution()/2 - bigBoxSize/2;

        //Create Resume Button with Text
        auto resumeButton = std::make_unique<engine::ui::UIButton>(Game::instance->uiBoxTexture);
        resumeButton->setHoveredTexture(Game::instance->uiButtonHoveredTexture);
        resumeButton->setOnclickSound(Game::instance->defaultButtonClickSound);
        resumeButton->size = {420, 50};
        resumeButton->position = {10, 15};
        auto resumeText = std::make_unique<engine::ui::UIText>(Game::instance->defaultFont, U"Resume", glm::ivec2{4, 1});
        resumeButton->addChild(std::move(resumeText));
        resumeButton->setOnClickCallback([]() {
            Game::instance->unpauseGame();
        });
        addChild(std::move(resumeButton));

        //Create Main Menu Button with Text
        auto returnButton = std::make_unique<engine::ui::UIButton>(Game::instance->uiBoxTexture);
        returnButton->setHoveredTexture(Game::instance->uiButtonHoveredTexture);
        returnButton->setOnclickSound(Game::instance->defaultButtonClickSound);
        returnButton->size = {420, 50};
        returnButton->position = {10, 35};
        auto returnText = std::make_unique<engine::ui::UIText>(Game::instance->defaultFont, U"To Main Menu", glm::ivec2{4, 1});
        returnButton->addChild(std::move(returnText));
        returnButton->setOnClickCallback([]() {
            Game::instance->toMainMenu();
        });
        addChild(std::move(returnButton));
    }
}
