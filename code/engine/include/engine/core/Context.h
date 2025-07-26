//
// Created by s453388 on 27.05.2025.
//

#pragma once
#include <glad/glad.h>
#define NOMINMAX
#include <windows.h>
#include <soloud.h>
#include <string>
#include <functional>
#include <GLFW/glfw3.h>

#include "glm/vec2.hpp"

namespace gl3::engine::context {
    class Context {
    public:
        using Callback = std::function<void(Context&)>;

        /// Context should only be created and accessed by the Engine.
        /// Will create a Windowed Fullscreen rendering close to the targetResolution.
        /// @param targetWidth Width, at which the game will render, regardless of the monitor size.
        /// @param targetHeight Deprecated, Height will be calculated based on the screen height and the target width.
        /// @param title The Name of the games' window.
        explicit Context(int targetWidth = 1280, int targetHeight = 720, const std::string &title = "Game");
        virtual ~Context();

        /// Gets a pointer to the window. Useful to find e.g. the windows resolution.
        /// @return A pointer to a GLFWwindow.
        [[nodiscard]] GLFWwindow *getWindow() const { return window; }

        /// Gets the windows render resolution.
        /// @return The resolution, the game renders at. Not the actual window size.
        [[nodiscard]] glm::ivec2 getRenderResolution() const {
            return renderResolution;
        }

    private:
        glm::ivec2 renderResolution;
        GLFWwindow *window = nullptr;
    };
}
