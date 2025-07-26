//
// Created by s453388 on 17.06.2025.
//

#include "../../include/engine/core/InputHandler.h"

#include <nlohmann/detail/input/parser.hpp>

#include "engine/core/Engine.h"
#include "glm/vec2.hpp"

namespace hnh::engine {

    InputHandler::InputHandler(context::Context* newContext): context(newContext) {
        glfwSetWindowUserPointer(context->getWindow(), this);
        glfwSetKeyCallback(context->getWindow(), &keyCallback);
        glfwSetMouseButtonCallback(context->getWindow(), &mouseButtonCallback);
        glfwSetCursorPosCallback(context->getWindow(), &mouseMoveCallback);
        glfwSetScrollCallback(context->getWindow(), &scrollCallback);
    }

    void InputHandler::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto* self = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
        if (self) self->onKey(key, scancode, action, mods);
    }

    void InputHandler::onKey(int key, int scancode, int action, int mods) {
        auto event = std::make_unique<KeyEvent>(key, scancode, action, mods);
        unhandledInputEvent.invoke(std::move(event));
    }

    void InputHandler::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
        auto* self = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
        if (self) self->onMouseButton(button, action, mods);
    }

    void InputHandler::onMouseButton(int button, int action, int mods) {
        auto event = MouseButtonEvent(button, action, mods);
        if(Engine::get().getObjectManager()->checkUIInput(&event, lastMousePos)) {
            return;
        }
        auto eventPtr = std::make_unique<MouseButtonEvent>(button, action, mods);
        unhandledInputEvent.invoke(std::move(eventPtr));
    }

    void InputHandler::mouseMoveCallback(GLFWwindow *window, double xpos, double ypos) {
        auto* self = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
        if (self) self->onMouseMove(xpos, ypos);
    }

    void InputHandler::onMouseMove(double x, double y) {
        int windowWidth, windowHeight;
        glfwGetWindowSize(Engine::get().getWindow(), &windowWidth, &windowHeight);
        // Normalize
        auto normalX = (x / windowWidth) * Engine::get().getRenderResolution().x;
        auto normalY = (y / windowHeight) * Engine::get().getRenderResolution().y;
        lastMousePos = glm::vec2{normalX, normalY};
        auto event = MouseMoveEvent(lastMousePos.x, lastMousePos.y);
        if(Engine::get().getObjectManager()->checkUIInput(&event, lastMousePos)) {
            return;
        }
        auto eventPtr = std::make_unique<MouseMoveEvent>(lastMousePos.x, lastMousePos.y);
        unhandledInputEvent.invoke(std::move(eventPtr));
    }

    void InputHandler::scrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
        auto* self = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
        if (self) self->onScroll(xOffset, yOffset);
    }

    void InputHandler::onScroll(double xOffset, double yOffset) {
        auto event = std::make_unique<ScrollEvent>(xOffset, yOffset);
        unhandledInputEvent.invoke(std::move(event));
    }

}
