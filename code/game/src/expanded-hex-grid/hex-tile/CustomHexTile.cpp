//
// Created by mburc on 24.06.2025.
//

#include "CustomHexTile.h"

#include <engine/core/Engine.h>

#include "../../Game.h"

namespace hnh::game {
    CustomHexTile::CustomHexTile(HexCoords hexPosition, engine::Sprite floorSprite): HexTile(hexPosition, std::move(floorSprite)),
    mark(engine::Engine::get().getResourceManager()->loadResource<engine::AtlasTexture>("sprites/PossibleMarker.png", glm::ivec2(32, 16))),
    obstacle(Obstacle(hexPosition))
    {
        mark.atlasTexture->MakeAtlasItem({0, 0}, {1, 1});
    }

    void CustomHexTile::draw(glm::mat3 &parentTransform) const {
        HexTile::draw(parentTransform);
        auto transform = parentTransform * getTransform();
        if(marked) {
            mark.draw(transform);
        }
        if(character) {
            character->draw(transform);
        }
        if(characterShadow) {
            auto shadowTile = Game::instance->getHexGrid()->getTileOrNull(characterShadow->getHexPosition());
            auto shadowTransform = parentTransform * shadowTile->getTransform();
            characterShadow->draw(shadowTransform);
        }
        obstacle.draw(transform);
    }

    Character* CustomHexTile::addCharacter(std::unique_ptr<Character> character) {
        this->character = std::move(character);
        return this->character.get();
    }

    void CustomHexTile::addCharacter(Character* character) {
        this->character.reset(character);
    }

    bool CustomHexTile::isWalkable() const {
        if((!character || character->getIsDead()) && terrain == 0 && obstacle.isWalkable()) {
            return true;
        }
        return false;
    }

    bool CustomHexTile::isCover() const {
        if(obstacle.isCover()) {
            return true;
        }
        return false;
    }

    glm::mat3 CustomHexTile::getTransform() const {
        auto transform = glm::mat3(1.0f);
        // Scale first (diagonal entries)
        transform[0][0] = scale; // X scale
        transform[1][1] = scale; // Y scale

        // Then translate (rightmost column)
        transform[2][0] = position.x;
        transform[2][1] = position.y;
        return transform;
    }

    Obstacle * CustomHexTile::getObstacle() {
        return &obstacle;
    }

    void CustomHexTile::onTurnEnd() {
        obstacle.onTurnEnd();
        if(character) character->onTurnEnd();

    }
}
