//
// Created by s453388 on 27.05.2025.
//

#pragma once
#include <memory>
#include <engine/core/Engine.h>
#include <engine/core/InputHandler.h>

namespace hnh::engine {
    /// A template for any actual games. Already links some \ref Event s from the \ref Engine.
    class Game {
    public:
        Game() = delete;

        /// Creates a game with some events from the provided engine.
        /// @param newEngine Reference to the engine, which events are listened to.
        explicit Game(Engine* newEngine);
        virtual ~Game() = default;

    protected:
        Engine* engine;

    private:
        virtual void onUpdate(float deltaTime){}
        virtual void onStart(){}
        virtual void onUnhandledInputEvent(std::unique_ptr<InputEvent> event){}
    };
}
