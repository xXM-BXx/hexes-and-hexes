//
// Created by mburc on 11.07.2025.
//

#include "Obstacle.h"
#include "../../Game.h"
#include "engine/Assets.h"

namespace gl3::game {
    Obstacle::Obstacle(HexCoords position): sprite(engine::AnimatedSprite(
                                                engine::Engine::get().getResourceManager()->loadResource<
                                                    engine::AtlasTexture>(
                                                    "sprites/ParticleEffects.png", glm::ivec2(32, 32)))),
                                            hexPosition(position),
                                            remainingLifetime(0) {
        std::vector<unsigned int> rows = {0, 1, 2, 3, 4};
        sprite.atlasTexture->MakeAtlasItems(rows, 8, {1, 1}, {16, 24});
        sprite.loadAnimations("json/animations/ParticleEffects.json");
        sprite.enqueueAnimationStep("empty");

    }

    void Obstacle::draw(glm::mat3 &parentTransform) const {
        auto transform = parentTransform * calculateLocalMatrix();
        sprite.draw(transform);
    }

    engine::AnimatedSprite *Obstacle::getSprite() {
        return &sprite;
    }

    bool Obstacle::isWalkable() const {
        if(currentObstacle == Crystal) {
            return false;
        }
        return true;
    }

    bool Obstacle::isCover() const {
        if(currentObstacle == Crystal) return true;
        return false;
    }

    void Obstacle::onTurnEnd() {
        if (remainingLifetime > 0) {
            remainingLifetime--;
        }
        if (remainingLifetime == 0) {
            setObstacleType(Empty);
        }
    }

    void Obstacle::setObstacleType(ObstacleType newObstacle) {
        auto oldObstacle = currentObstacle;
        currentObstacle = newObstacle;
        if(oldObstacle == Crystal) {
            Game::instance->playCrystalBreakSound();
        }
        switch (currentObstacle) {
            default: break;
            case Empty:
                sprite.forceStopAnimation();
                sprite.enqueueAnimationStep("empty");
                break;
            case Crystal:
                Game::instance->playCrystalGrowSound();
                sprite.forceStopAnimation();
                remainingLifetime = 10;
                sprite.enqueueAnimationStep("crystal_spawn");
                break;
            case Explosion:
                Game::instance->playExplosionSound();
                sprite.forceStopAnimation();
                remainingLifetime = 0;
                sprite.enqueueAnimationStep("explosion");
                break;
            case Heal:
                Game::instance->playHealSound();
                sprite.forceStopAnimation();
                remainingLifetime = 0;
                sprite.enqueueAnimationStep("heal");
                break;
            case Lifesteal:
                Game::instance->playLifeStealSound();
                sprite.forceStopAnimation();
                remainingLifetime = 0;
                sprite.enqueueAnimationStep("lifesteal");
                break;
        }
    }
}
