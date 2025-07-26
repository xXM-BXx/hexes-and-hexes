#pragma once
#include "CompleteUI.h"

#include "../../Game.h"

namespace hnh::game {
    CompleteUI::CompleteUI() {
    }

    void CompleteUI::draw(glm::mat3 &parentTransform) const {
        UIObject::draw(parentTransform);
        auto transform = parentTransform * calculateLocalMatrix();
        healthVisualization.draw(transform);
        actionList.draw(transform);
        queueVisualization.draw(transform);
        mainMenu.draw(transform);
        pauseButton.draw(transform);
        pauseMenu.draw(transform);
        gameOverMenu.draw(transform);
    }

    void CompleteUI::updateUI() {
        actionList.updateUI();
        queueVisualization.updateUI();
        healthVisualization.updateUI();
    }

    bool CompleteUI::checkForUIInput(glm::mat3 &parentTransform, engine::InputEvent *inputEvent, glm::vec2 &mousePos) {
        if (!visible) {
            return false;
        }
        bool intercepted = false;
        if(UIObject::checkForUIInput(parentTransform, inputEvent, mousePos)) intercepted = true;
        glm::mat3 transform = parentTransform * calculateLocalMatrix(); // includes position, scale
        // Propagate to children
        if(actionList.checkForUIInput(transform, inputEvent, mousePos)) intercepted = true;
        if(queueVisualization.checkForUIInput(transform, inputEvent, mousePos)) intercepted = true;
        if(healthVisualization.checkForUIInput(transform, inputEvent, mousePos)) intercepted = true;
        if(mainMenu.checkForUIInput(transform, inputEvent, mousePos)) intercepted = true;
        if(pauseButton.checkForUIInput(transform, inputEvent, mousePos)) intercepted = true;
        if(pauseMenu.checkForUIInput(transform, inputEvent, mousePos)) intercepted = true;
        if(gameOverMenu.checkForUIInput(transform, inputEvent, mousePos)) intercepted = true;

        return intercepted;
    }

    void CompleteUI::setToMainMenu() {
        mainMenu.updateUI();
        mainMenu.visible = true;
        actionList.visible = false;
        queueVisualization.visible = false;
        healthVisualization.visible = false;
        pauseButton.visible = false;
        pauseMenu.visible = false;
        gameOverMenu.visible = false;
    }

    void CompleteUI::setToPlay() {
        mainMenu.visible = false;
        actionList.visible = true;
        queueVisualization.visible = true;
        healthVisualization.visible = true;
        pauseButton.visible = true;
        pauseMenu.visible = false;
    }

    void CompleteUI::setToPause() {
        mainMenu.visible = false;
        actionList.visible = false;
        queueVisualization.visible = false;
        healthVisualization.visible = false;
        pauseButton.visible = false;
        pauseMenu.visible = true;
    }

    void CompleteUI::setToGameOver() {
        mainMenu.visible = false;
        actionList.visible = false;
        queueVisualization.visible = false;
        healthVisualization.visible = false;
        pauseButton.visible = false;
        pauseMenu.visible = false;
        gameOverMenu.visible = true;
        gameOverMenu.updateUI();
    }
}
