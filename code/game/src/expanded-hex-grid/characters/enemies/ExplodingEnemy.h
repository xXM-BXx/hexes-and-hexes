//
// Created by mburc on 18.07.2025.
//

#pragma once
#include "BasicEnemy.h"

namespace hnh::game {

    ///Weakest enemy.
    class ExplodingEnemy : public BasicEnemy {
    public:
        ///Gets the \ref Explode action.
        ExplodingEnemy();

        static const unsigned int scoreValue = 1;

        ///Will try to get closer to the player, and failing that, will explode.
        void decide() override;
    };
}
