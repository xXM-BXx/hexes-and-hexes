#pragma once
#include <engine/tilemap/HexTile.h>

#include "Obstacle.h"
#include "../characters/Character.h"

namespace hnh::game {
    class CustomHexTile : public engine::HexTile {
    public:
        ///Create a normal tile and initialize some custom components.
        CustomHexTile(HexCoords hexPosition, engine::Sprite floorSprite);

        ///First draw the tile, then the mark, then the character and character shadow and then the obstacle.
        void draw(glm::mat3 &parentTransform) const override;

        /// If the mark should be drawn.
        bool marked = false;

        ///Move a \ref Character to this tile and return a pointer to it.
        Character *addCharacter(std::unique_ptr<Character> character);

        ///Add a \ref Character with a normal pointer.
        void addCharacter(Character *character);

        ///Here the \ref Character is actually stored.
        std::unique_ptr<Character> character = nullptr;

        ///If the tile contains a \ref Character or blocking \ref Obstacle.
        bool isWalkable() const;
        ///If the \rec Obstacle is cover.
        bool isCover() const;

        ///This is needed, so the character can be drawn at two depths at the same time, so it doesn't clip behind tiles while animating the move.
        Character *characterShadow = nullptr;

        glm::mat3 getTransform() const;

        ///Access the obstacle.
        Obstacle *getObstacle();

        ///Pass on to character and obstacle, that the turn ended.
        void onTurnEnd();

    private:
        engine::Sprite mark;
        Obstacle obstacle;
    };
}
