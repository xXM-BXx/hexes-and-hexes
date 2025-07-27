#include "Tier3EnemySingle.h"

#include "../../../Game.h"

namespace hnh::game {
    Tier3EnemySingle::Tier3EnemySingle(Character *owner) : Action(owner) {
        maxCooldown = 0;
    }

    std::vector<HexCoords> Tier3EnemySingle::markTiles(HexCoords target) {
        bool hasLineOfSight = HexCoords::hasLineOfSight(owner->getHexPosition(), target, [](HexCoords coord) {
           return Game::instance->getHexGrid()->getTileOrNull(coord)->isCover();
        });
        if(hasLineOfSight) {
            Game::instance->markTiles({target});
            return{target};
        }
        std::vector<HexCoords> emptyResult;
        Game::instance->markTiles(emptyResult);
        return {emptyResult};
    }

    bool Tier3EnemySingle::execute(HexCoords target) {
        auto targets = markTiles(target);
        if(targets.empty() || currentCooldown != 0) { return false; }
        owner->animateSpell([&, targets]() {
            for (auto position: targets) {
                auto tile = Game::instance->getHexGrid()->getTileOrNull(position);
                tile->getObstacle()->setObstacleType(Obstacle::Explosion);
                if(tile->character) tile->character->takeDamage(2);
            }
        });
        currentCooldown = maxCooldown;
        return true;
    }
}
