#include "BasicEnemy.h"

#include <utility>

#include "../../../Game.h"
#include "../actions/MoveAction.h"


namespace hnh::game {
    BasicEnemy::BasicEnemy(std::shared_ptr<engine::AtlasTexture> atlasTexture): Character(std::move(atlasTexture)) {
    }

    void BasicEnemy::decide() {
        //printf("Enemy Turn");
        Game::instance->animationFinished();
    }
}
