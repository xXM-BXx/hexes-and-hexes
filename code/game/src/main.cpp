#include <iostream>

#include <engine/core/Engine.h>
#include <engine/game/Game.h>

#include "Game.h"

#define GET_STRING(x) #x
#define GET_DIR(x) GET_STRING(x)

///Create an Engine and a game, then run the engine.
int main() {
    try {
        hnh::engine::Engine& engine = hnh::engine::Engine::get();
        engine.init(1920, 1080, "Hexes & Hexes");
        hnh::Game game = hnh::Game(&engine);
        engine.run();
    }
    catch(const std::exception &e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
    }

}
