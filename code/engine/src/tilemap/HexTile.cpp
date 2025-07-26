//
// Created by mburc on 03.12.2024.
//

#include <engine/tilemap/HexTile.h>

namespace hnh::engine {
    HexTile::HexTile(HexCoords hexPosition, Sprite sprite) :
        HexPosition(hexPosition),
        floorSprite(std::move(sprite))
    {
        this->position = hexPosition.toWorld();
    }


    void HexTile::draw(glm::mat3 &parentTransform) const {
        auto transform = parentTransform * calculateLocalMatrix();
        floorSprite.draw(transform);
    }
}
