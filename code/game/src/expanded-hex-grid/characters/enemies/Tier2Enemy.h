#pragma once
#include "BasicEnemy.h"

namespace hnh::game {

    ///Medium strength enemy.
    class Tier2Enemy : public BasicEnemy {
    public:
        ///Gets the \ref Tier2EnemyAttack.
        explicit Tier2Enemy();

        static const unsigned int scoreValue = 1;

        ///Will first try shooting the player.
        ///Will then try reaching a tile, from which the player can be shot.
        ///Will then run away.
        void decide() override;
    };
}
