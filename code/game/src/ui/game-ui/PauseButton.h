#pragma once
#include "engine/rendering/Sprite.h"
#include "engine/ui/UIObject.h"

namespace hnh::game {
    ///A button to set the game to pause.
    class PauseButton : public engine::ui::UIObject {
    public:
        PauseButton();

        void draw(glm::mat3 &parentTransform) const override;

    private:
        engine::Sprite pauseSymbol;
    };
}
