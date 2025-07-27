#include <engine/game/Game.h>

namespace hnh::engine {
    Game::Game(Engine *newEngine): engine(newEngine) {
        engine->update.addListener([&](float _deltaTime) {
            onUpdate(_deltaTime);
        });
        engine->start.addListener([&] {
            onStart();
        });
        engine->getInputHandler()->unhandledInputEvent.addListener([&](std::unique_ptr<InputEvent> _event) {
            onUnhandledInputEvent(std::move(_event));
        });
    }
}
