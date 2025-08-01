#pragma once
#include <engine/ui/UIObject.h>

#include <engine/ui/text/UIText.h>

namespace hnh::game {
    ///Is shown at game over, and displays the score then.
    ///Has a button to return to the main menu.
    class GameOverMenu : public engine::ui::UIObject {
    public:
        GameOverMenu();

        void draw(glm::mat3 &parentTransform) const override;

        void updateUI();

    private:
        engine::ui::UIText gameOverText;
    };
}
