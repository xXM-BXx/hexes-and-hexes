//
// Created by mburc on 23.06.2025.
//

#include <engine/ui/UIObject.h>

#include "engine/core/Engine.h"

namespace gl3::engine::ui {
    UIObject::UIObject() = default;

    void UIObject::draw(glm::mat3 &parentTransform) const {
        if(!visible) return;
        auto transform = parentTransform * calculateLocalMatrix();
        for (auto &child: children) {
            child->draw(transform);
        }
    }

    bool UIObject::checkForUIInput(glm::mat3 &parentTransform, InputEvent* inputEvent, glm::vec2 &mousePos) {
        if (!visible) {
            return false;
        }
        bool intercepted = false;
        glm::mat3 transform = parentTransform * calculateLocalMatrix(); // includes position, scale
        if(canInteract) {
            // Convert mousePos to local space (optional: inverse transform)
            if (pointInBounds(transform, mousePos)) {
                if (!isHovered) {
                    if(!disabled) onEnter();
                    if(onEnterCallback) onEnterCallback();
                }
                intercepted = true;
                isHovered = true;

                if (auto buttonEvent = dynamic_cast<MouseButtonEvent*>(inputEvent)) {
                    if (buttonEvent->button == GLFW_MOUSE_BUTTON_LEFT && buttonEvent->action == GLFW_PRESS) {
                        if(!disabled) onClick();
                        if(onClickCallback) onClickCallback();
                    }
                    intercepted = true;
                }
            } else {
                if (isHovered) {
                    if(!disabled) onExit();
                    if(onExitCallback) onExitCallback();
                }
                isHovered = false;
            }
        }

        // Propagate to children
        for (auto &child: children) {
            if (child->checkForUIInput(transform, inputEvent, mousePos)) {
                intercepted = true;
            }
        }
        return intercepted;
    }

    void UIObject::setOnClickCallback(std::function<void()> callback) {
        onClickCallback = std::move(callback);
    }

    void UIObject::setOnEnterCallback(std::function<void()> callback) {
        onEnterCallback = std::move(callback);
    }

    void UIObject::setOnExitCallback(std::function<void()> callback) {
        onExitCallback = std::move(callback);
    }

    void UIObject::setDisabled(bool disabled) {
        this->disabled = disabled;
    }

    bool UIObject::getDisabled() const {
        return disabled;
    }

    glm::ivec2 UIObject::getSize() const {
        return size;
    }

    glm::mat3 UIObject::calculateLocalMatrix() const {
        auto transform = glm::mat3(1.0f);
        // Scale first (diagonal entries)
        transform[0][0] = scale; // X scale
        transform[1][1] = scale; // Y scale

        // Then translate (rightmost column)
        transform[2][0] = position.x;
        transform[2][1] = position.y;
        return transform;
    }

    bool UIObject::pointInBounds(glm::mat3 &transform, glm::vec2 &mousePos) const {
        auto scaleLessTransform = transform;
        scaleLessTransform[0][0] = 1;
        scaleLessTransform[1][1] = 1;
        // Convert the screen-space mouse position into local space
        glm::mat3 invTransform = inverse(scaleLessTransform);
        glm::vec3 localPos = invTransform * glm::vec3(mousePos, 1.0f);

        auto trueSize = getTrueSize(transform);

        return (
            localPos.x >= 0.0f && localPos.x <= trueSize.x &&
            localPos.y >= 0.0f && localPos.y <= trueSize.y
        );
    }

    glm::vec2 UIObject::getTrueSize(glm::mat3 &transform) const{
        float scale = transform[0][0];
        return (glm::vec2)size * scale;
    }
}
