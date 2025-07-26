#include "Tier2EnemyAttack.h"

#include "../../../Game.h"

namespace hnh::game {
    Tier2EnemyAttack::Tier2EnemyAttack(Character *owner)
        : Action(owner) {
        maxCooldown = 0;
    }

    std::vector<HexCoords> Tier2EnemyAttack::markTiles(HexCoords target) {
        bool hasLineOfSight = HexCoords::hasLineOfSight(owner->getHexPosition(), target, [](HexCoords coord) {
           return Game::instance->getHexGrid()->getTileOrNull(coord)->isCover();
        });
        if(hasLineOfSight) {
            Game::instance->markTiles({target});
            return{target};
        }
        Game::instance->markTiles(std::vector<HexCoords>());
        return {std::vector<HexCoords>()};
    }

    bool Tier2EnemyAttack::execute(HexCoords target) {
        auto targets = markTiles(target);
        if(targets.empty() || currentCooldown != 0) { return false; }
        owner->animateSpell([&, targets]() {
            for (auto position: targets) {
                auto tile = Game::instance->getHexGrid()->getTileOrNull(position);
                tile->getObstacle()->setObstacleType(Obstacle::Explosion);
                if(tile->character) tile->character->takeDamage(1);
            }
        });
        currentCooldown = maxCooldown;
        return true;
    }
}
