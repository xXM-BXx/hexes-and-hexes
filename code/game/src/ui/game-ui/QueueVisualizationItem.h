//
// Created by mburc on 13.07.2025.
//

#pragma once
#include "../../expanded-hex-grid/characters/Character.h"
#include "engine/ui/UIBox.h"
#include "engine/ui/UIObject.h"

namespace hnh::game {
    ///A small box, with a character sprite in it and a reference to a character.
    ///When hovered it will move the mouse marker in the game to characters position.
    class QueueVisualizationItem : public engine::ui::UIObject {
    public:
        explicit QueueVisualizationItem(Character *character);

        void draw(glm::mat3 &parentTransform) const override;
        void setCharacter(Character *character);
        void updateUI();

    private:
        engine::Sprite characterSprite;
        Character* character;
        HexCoords characterPosition;
    };
}
