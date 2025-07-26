#pragma once
#include <engine/ui/UIObject.h>

#include "ActionListItem.h"

namespace hnh::game {
    class Action;

    class ActionList : public engine::ui::UIObject {
    public:
        ActionList();

        ///Gets the players actions, and creates a list of \ref ActionListItem s based on it.
        ///It creates several pages if needed.
        void updateUI();

        ///Draws the list items and the page buttons with calculated transforms.
        void draw(glm::mat3 &parentTransform) const override;
        ///Checks the list items and the page buttons with calculated transforms.
        bool checkForUIInput(glm::mat3 &parentTransform, engine::InputEvent *inputEvent, glm::vec2 &mousePos) override;



    private:
        void setPageSwitchCallback();

        void startUpPageSwitchFix();
        unsigned int pageSize = 5;
        void getActions();
        void setupPageSwitching();

        unsigned int selectedPage = 0;
        unsigned int listVerticalOffset = 20;
        unsigned int width = 500;
        std::vector<std::vector<ActionListItem>> actionListItems;

        engine::ui::UIButton pageDownButton;
        engine::ui::UIButton pageUpButton;
    };
}
