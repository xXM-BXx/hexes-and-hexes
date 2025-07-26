#include <iostream>

#include <engine/core/Engine.h>
#include <engine/game/Game.h>

#include "Game.h"

#define GET_STRING(x) #x
#define GET_DIR(x) GET_STRING(x)

///Create an Engine and a game, then run the engine.
int main() {
    try {
        gl3::engine::Engine& engine = gl3::engine::Engine::get();
        engine.init(1920, 1080, "Hexes & Hexes");
        gl3::Game game = gl3::Game(&engine);
        engine.run();
    }
    catch(const std::exception &e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
    }

}
