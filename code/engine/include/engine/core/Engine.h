//
// Created by s453388 on 27.05.2025.
//

#pragma once
#include <engine/core/Context.h>
#include <engine/core/Events.h>
#include "glm/ext/vector_int2.hpp"
#include <engine/resource_loading/ResourceManager.h>
#include <engine/core/InputHandler.h>
#include <engine/core/ObjectManager.h>
#include "glm/ext/matrix_float4x4.hpp"

namespace hnh::engine{
    class Engine{
    public:
        /// Gets or creates the Engine, if it doesn't exist yet.
        /// @return A Reference to an Engine object.
        static Engine& get();

        /// Sets up the Engine with some managers. Should only be called once after creation of the Engine, e.g. in main.cpp.
        /// @param width The targetWidth as described in \ref hnh::engine::context::Context::Context.
        /// @param height The targetHeight as described in \ref hnh::engine::context::Context::Context.
        /// @param title The title as described in \ref hnh::engine::context::Context::Context.
        void init(int width, int height, const std::string &title);

        /// An update event, that will be called each frame with the delta time.
        events::Event<Engine, float> update;
        /// An event, that will be called once before starting the game loop.
        events::Event<Engine> start;

        /// Constructor, that initializes some members, that do not rely on the Engines' context.
        Engine();

        /// Provides access to the Engines' objectManager.
        /// @return A reference to a \ref hnh::engine::ObjectManager.
        ObjectManager* getObjectManager();
        /// Provides access to the Engines' resourceManager.
        /// @return A reference to a \ref hnh::engine::ResourceManager.
        ResourceManager* getResourceManager();

        /// Provides access to the Engines' inputHandler.
        /// Needed so the Game can link be linked to the Input Events.
        /// @return A reference to a \ref hnh::engine::InputHandler.
        InputHandler* getInputHandler();
        /// Start the Engines' update loop. Should only be called once after initializing the Engine and creating the Game, e.g. in main.cpp.
        void run();

        virtual ~Engine();

        /// Returns the window of the Engines' context.
        /// @return A pointer to a GLFWwindow.
        GLFWwindow *getWindow() { return context->getWindow(); }
        /// Returns the render resolution of the Engines' context.
        /// @return The rendered resolution as a \ref glm::ivec2.
        [[nodiscard]] glm::ivec2 getRenderResolution() const { return context->getRenderResolution(); }

        /// Returns the mouses world position.
        /// @param mousePos The mouse position, normalized to the render resolution.
        /// @return The world position, as a \ref glm::ivec2.
        [[nodiscard]] glm::ivec2 screenToWorld(glm::vec2 mousePos) const;

        /// Get a projection matrix for e.g. \ref hnh::engine::GameObject, with the size of the render resolution centered on (0,0).
        /// @return The projection matrix.
        [[nodiscard]] glm::mat4 getSceneProjectionMatrix() const;
        /// Get a projection matrix for e.g. \ref hnh::engine::UIObject, with the size of the render resolution with (0,0) being the top left corner.
        /// @return The projection matrix.
        [[nodiscard]] glm::mat4 getUIProjectionMatrix() const;

        /// Set the background color, that will show if nothing is rendering over it.
        glm::vec4 backgroundColor = {0.5f, 0.0f, 0.5f, 1.0f};

        /// The Engines audio manager.
        SoLoud::Soloud audioManager;
        /// Set the audio managers volume.
        void setVolume(float volume);
    private:
        std::unique_ptr<context::Context> context;
        std::unique_ptr<ObjectManager> objectManager;
        ResourceManager resourceManager;
        std::unique_ptr<InputHandler> inputHandler;

        void updateDeltaTime();

        // Prevent copies
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

        void draw();

        float lastFrameTime = 1.0f/60;
        float deltaTime = 1.0f/60;
    };
}
