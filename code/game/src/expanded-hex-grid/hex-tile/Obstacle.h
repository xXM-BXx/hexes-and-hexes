#pragma once
#include <soloud_wav.h>
#include <engine/rendering/GameObject.h>

#include <engine/rendering/AnimatedSprite.h>

#include "engine/tilemap/HexCoords.h"

namespace hnh::game {
    class Obstacle : public engine::GameObject {
    public:
        ///The different types of obstacles.
        enum ObstacleType {
            Empty,
            Crystal,
            Explosion,
            Lifesteal,
            Heal
        };


        explicit Obstacle(HexCoords position);

        void draw(glm::mat3 &parentTransform) const override;

        engine::AnimatedSprite *getSprite();

        /// If the Obstacle is a Crystal
        bool isWalkable() const;
        /// If the Obstacle is a Crystal
        bool isCover() const;

        ///Reduce the current obstacles' lifetime.
        ///If the lifetime is 0, the obstacle will be set to empty.
        void onTurnEnd();

        unsigned int remainingLifetime;

        ///Set the obstacle to a new type. This will cause a corresponding animation and sound to play, and set the correct lifetime.
        void setObstacleType(ObstacleType newObstacle);

        HexCoords hexPosition;

    private:
        ObstacleType currentObstacle = Empty;
        engine::AnimatedSprite sprite;
    };
}
