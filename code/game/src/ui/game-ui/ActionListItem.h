//
// Created by mburc on 13.07.2025.
//

#pragma once
#include "../../expanded-hex-grid/characters/actions/Action.h"
#include "engine/ui/UIButton.h"
#include "engine/ui/UIObject.h"
#include "engine/ui/text/UIText.h"

namespace gl3::game {

class ActionListItem : public engine::ui::UIObject {

public:

    ///Takes an action, and creates a button with text and a tooltip box with description based on it.
    ///It creates a callback to set the players action later.
    explicit ActionListItem(gl3::game::Action* action, const std::string &onClickName, glm::ivec2 size);

    void addDescriptionText();

    void draw(glm::mat3 &parentTransform) const override;
    bool checkForUIInput(glm::mat3 &parentTransform, engine::InputEvent *inputEvent, glm::vec2 &mousePos) override;
    void updateUI();


private:
    engine::ui::UIText* descriptionText;
    void setupCallbacks();
    std::string onClickName;
    Action* action;
    engine::ui::UIButton button;
    engine::ui::UIText actionNameText;
    engine::ui::UIText actionCooldownText;
    engine::ui::UIBox description;
};

}
