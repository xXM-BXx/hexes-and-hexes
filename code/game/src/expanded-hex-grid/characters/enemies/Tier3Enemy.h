//
// Created by mburc on 18.07.2025.
//

#pragma once
#include "BasicEnemy.h"

namespace gl3::game {

    ///Strongest enemy.
class Tier3Enemy : public BasicEnemy {
public:
    ///Gets the \ref Tier3EnemyAoE attack, and the \ref Tier3EnemySingle attack.
    ///Can attack twice.
    Tier3Enemy();
    static const unsigned int scoreValue = 10;

    ///Will first try shooting the player.
    ///Will then try hitting the player indirectly with minimum casualties.
    ///Will then try to find a tile, from which the player can be shot.
    ///Will then run away.
    void decide() override;
};

}
