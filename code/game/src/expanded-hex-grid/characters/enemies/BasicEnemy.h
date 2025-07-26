//
// Created by mburc on 07.07.2025.
//

#pragma once
#include "../Character.h"

namespace hnh::game {
    ///A basic enemy that is no different to a \ref Character, but it is the parent of all other enemies.
    class BasicEnemy : public Character {
    public:
        explicit BasicEnemy(std::shared_ptr<engine::AtlasTexture> atlasTexture);

        void decide() override;
    };
}
