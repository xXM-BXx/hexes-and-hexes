//
// Created by mburc on 03.12.2024.
//

#pragma once
#include <engine/tilemap/HexCoords.h>
#include <engine/rendering/GameObject.h>
#include <engine/rendering/Sprite.h>

namespace gl3::engine {

class HexTile : public GameObject {

    public:
    ///Creates a Tile with a hex position and a sprite.
    ///Sets position based on hex position.
    explicit HexTile(HexCoords hexPosition, Sprite floorSprite);

    HexTile(const HexTile &other) = delete;
    //Copy Operator delete
    HexTile& operator= (const HexTile &other) = delete;

    HexTile(HexTile &&other) noexcept :
        HexPosition(other.HexPosition),
        useTerrain(other.useTerrain),
        terrain(other.terrain),
        zIndex(other.zIndex),
        floorSprite(std::move(other.floorSprite))
    {
        position = other.position;
    }
    //Define Move Operator
    HexTile& operator= (HexTile &&other) noexcept
    {
        HexPosition = other.HexPosition;
        useTerrain = other.useTerrain;
        terrain = other.terrain;
        zIndex = other.zIndex;
        position = other.position;
        floorSprite = std::move(other.floorSprite);
        return *this;
    }

    ///Draws the sprite with parentTransforms and own transforms.
    void draw(glm::mat3 &parentTransform) const override;

    ///Provides access to the sprite.
    Sprite* getFloorSprite(){ return &floorSprite; }

    ///The hex position.
    HexCoords HexPosition;

    ///Set the z-index.
    void setZIndex(int zIndex) {
        this->zIndex = zIndex;
        floorSprite.setZIndex(zIndex);
    }
    ///If terrain should be used.
    bool useTerrain = true;
    ///Which terrain should be used.
    unsigned int terrain = 0;
private:
    int zIndex = 0;

    Sprite floorSprite;
};


}
