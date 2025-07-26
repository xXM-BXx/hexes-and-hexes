#include "MainMenu.h"

#include <engine/core/Engine.h>
#include <engine/ui/text/UIText.h>

#include "../../Game.h"

namespace hnh::game {
    MainMenu::MainMenu()
        : titleCard(
              engine::Engine::get().getResourceManager()->loadResource<engine::AtlasTexture>(
                  "sprites/TitleCard.png", glm::ivec2(128, 128))),
          tutorialBox(
              engine::Engine::get().getResourceManager()->loadResource<engine::ui::UITexture>(
                  "sprites/UIButtonDisabled.png")),
          highscoreText(Game::instance->defaultFont, U"Highscore: ?", {4, 1}) {
        auto titleAtlas = engine::Engine::get().getResourceManager()->loadResource<engine::AtlasTexture>(
            "sprites/TitleCard.png");
        titleAtlas->MakeAtlasItem({0, 0}, {1, 1});
        scale = 4;


        int approxHeight = (128 + 60) * scale;
        int yPos = engine::Engine::get().getRenderResolution().y / 2 - approxHeight / 2;
        position = {10, yPos};
        titleCard.position = {0, 0};


        //Create Highscore Box
        auto highscoreBox = std::make_unique<engine::ui::UIBox>(Game::instance->uiBoxTexture);
        auto highscorePosition = glm::ivec2(0, 128);
        highscoreBox->position = highscorePosition;
        highscoreBox->size = {buttonWidth * scale, 50};
        highscoreText.position += highscorePosition;
        addChild(std::move(highscoreBox));


        //Create Start Game Button
        std::unique_ptr<engine::ui::UIButton> startButton = std::make_unique<engine::ui::UIButton>(
            Game::instance->uiBoxTexture);
        startButton->setHoveredTexture(Game::instance->uiButtonHoveredTexture);
        std::unique_ptr<engine::ui::UIText> startText = std::make_unique<engine::ui::UIText>(
            Game::instance->defaultFont, U"Start Game", glm::ivec2(4, 1));
        startButton->addChild(std::move(startText));
        startButton->position = {0, 148};
        startButton->size = {buttonWidth * scale, 50};
        startButton->setOnClickCallback([]() {
            Game::instance->startGame();
        });
        startButton->setOnclickSound(Game::instance->defaultButtonClickSound);
        addChild(std::move(startButton));

        //Create Tutorial Button
        std::unique_ptr<engine::ui::UIButton> tutorialButton = std::make_unique<engine::ui::UIButton>(
            Game::instance->uiBoxTexture);
        tutorialButton->setHoveredTexture(Game::instance->uiButtonHoveredTexture);
        std::unique_ptr<engine::ui::UIText> hoverText = std::make_unique<engine::ui::UIText>(
            Game::instance->defaultFont, U"Tutorial", glm::ivec2(4, 1));
        tutorialButton->addChild(std::move(hoverText));
        tutorialButton->position = {0, 168};
        tutorialButton->size = {buttonWidth * scale, 50};
        tutorialButton->setOnClickCallback([&]() {
            toggleTutorial();
        });
        tutorialButton->setOnclickSound(Game::instance->defaultButtonClickSound);
        addChild(std::move(tutorialButton));

        //Create Close Game Button
        std::unique_ptr<engine::ui::UIButton> closeButton = std::make_unique<engine::ui::UIButton>(
            Game::instance->uiBoxTexture);
        closeButton->setHoveredTexture(Game::instance->uiButtonHoveredTexture);
        std::unique_ptr<engine::ui::UIText> closeText = std::make_unique<engine::ui::UIText>(
            Game::instance->defaultFont, U"Close Game", glm::ivec2(4, 1));
        closeButton->addChild(std::move(closeText));
        closeButton->position = {0, 188};
        closeButton->size = {buttonWidth * scale, 50};
        closeButton->setOnClickCallback([]() {
            glfwSetWindowShouldClose(engine::Engine::get().getWindow(), true);
        });
        closeButton->setOnclickSound(Game::instance->defaultButtonClickSound);
        addChild(std::move(closeButton));

        //Setup Tutorial Box
        glm::ivec2 topLeftMargin = {5, 3};
        glm::ivec2 boxPos = {buttonWidth + topLeftMargin.x, -this->position.y / scale + topLeftMargin.y};
        tutorialBox.position = boxPos;
        glm::ivec2 bottomRightMargin = {3, 3};
        auto screensize = engine::Engine::get().getRenderResolution();
        glm::ivec2 boxSize = {
            (screensize.x - boxPos.x * scale - position.x * scale - bottomRightMargin.x * scale),
            (screensize.y - topLeftMargin.y * scale - bottomRightMargin.y * scale)
        };
        tutorialBox.size = boxSize;
        auto tutorialText = std::make_unique<engine::ui::UIText>(
            Game::instance->defaultFont, tutorialTextContent, glm::ivec2(3, 1));
        tutorialText->scale = 0.75f;
        tutorialText->setMaxWidth(boxSize.x / scale / tutorialText->scale);
        tutorialBox.addChild(std::move(tutorialText));
    }

    void MainMenu::toggleTutorial() {
        if (showTutorial) {
            showTutorial = false;
        } else {
            showTutorial = true;
        }
    }

    void MainMenu::updateUI() {
        int score = Game::instance->highScore;
        auto wide = std::to_wstring(score);
        std::u32string scoreString = U"Highscore:" + std::u32string(wide.begin(), wide.end());
        highscoreText.setText(scoreString);
    }

    void MainMenu::draw(glm::mat3 &parentTransform) const {
        if (!visible) return;
        auto transform = parentTransform * calculateLocalMatrix();
        UIObject::draw(parentTransform);
        if (showTutorial) tutorialBox.draw(transform);
        highscoreText.draw(transform);
        titleCard.draw(transform, true);
    }
}
