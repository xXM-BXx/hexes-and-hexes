//
// Created by mburc on 13.07.2025.
//

#pragma once
#include <engine/ui/UIObject.h>

#include "ActionList.h"
#include "GameOverMenu.h"
#include "MainMenu.h"
#include "PauseButton.h"
#include "PauseMenu.h"
#include "PlayerHealthVisualization.h"
#include "QueueVisualization.h"


namespace hnh::game {
    ///Manages the other UI components and transitions them during state changes.
    class CompleteUI : public engine::ui::UIObject {
    public:
        CompleteUI();

        void draw(glm::mat3 &parentTransform) const override;

        void updateUI();

        bool checkForUIInput(glm::mat3 &parentTransform, engine::InputEvent *inputEvent, glm::vec2 &mousePos) override;

        void setToMainMenu();

        void setToPlay();

        void setToPause();

        void setToGameOver();

    private:
        QueueVisualization queueVisualization;
        ActionList actionList;
        PlayerHealthVisualization healthVisualization;
        MainMenu mainMenu;
        PauseButton pauseButton;
        PauseMenu pauseMenu;
        GameOverMenu gameOverMenu;
    };
}
