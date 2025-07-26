//
// Created by mburc on 13.07.2025.
//

#pragma once
#include <engine/ui/UIObject.h>

#include <engine/ui/text/UIText.h>

#include "engine/rendering/Sprite.h"

namespace hnh::game {
    ///Has a text for displaying health and score, which will be updated by the game regularly.
    class PlayerHealthVisualization : public engine::ui::UIObject {
    public:
        PlayerHealthVisualization();
        void draw(glm::mat3 &parentTransform) const override;

        void updateUI();

    private:
        engine::ui::UIText healthText;
        engine::ui::UIText scoreText;
        engine::Sprite heartSprite;
    };
}
