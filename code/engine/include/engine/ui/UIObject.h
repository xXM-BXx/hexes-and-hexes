#pragma once
#include <memory>
#include <vector>

#include "glm/fwd.hpp"
#include <glm/vec2.hpp>
#include <glm/mat3x3.hpp>

#include <engine/core/InputHandler.h>

namespace hnh::engine::ui {
    class UIObject {
    public:
        UIObject();

        virtual ~UIObject() = default;

        // Prevent copies
        UIObject(const UIObject &) = delete;

        UIObject &operator=(const UIObject &) = delete;

        // Allow moves
        UIObject(UIObject &&) noexcept = default;

        UIObject &operator=(UIObject &&) noexcept = default;

        ///Draw the object, and then any children.
        virtual void draw(glm::mat3 &parentTransform) const;

        /// Check for input in itself, and then it's children.
        /// @param inputEvent The input event.
        /// @param mousePos The mouse position at the time of the input event.
        /// @return If the object or any children intercepted the input.
        virtual bool checkForUIInput(glm::mat3 &parentTransform, InputEvent *inputEvent, glm::vec2 &mousePos);

        ///Add a child.
        UIObject* addChild(std::unique_ptr<UIObject> child) {
            children.push_back(std::move(child));
            return children.back().get();
        }

        void setOnClickCallback(std::function<void()> callback);

        void setOnEnterCallback(std::function<void()> callback);

        void setOnExitCallback(std::function<void()> callback);

        bool visible = true;
        glm::vec2 position = glm::vec2(0, 0);
        float scale = 1;
        ///Set if the object should no longer intercept do internal onEnter etc. Will still call manually set callbacks.
        virtual void setDisabled(bool disabled);
        [[nodiscard]] bool getDisabled() const;
        [[nodiscard]] glm::ivec2 getSize() const;
        glm::ivec2 size = glm::ivec2(0, 0);
    protected:
        std::vector<std::unique_ptr<UIObject> > children;
        bool isHovered = false;
        bool canInteract = true;
        bool disabled = false;

        virtual void onEnter() {
        }

        virtual void onExit() {
        }

        virtual void onClick() {
        }

        std::function<void()> onClickCallback;
        std::function<void()> onEnterCallback;
        std::function<void()> onExitCallback;

        [[nodiscard]] glm::mat3 calculateLocalMatrix() const;

        bool pointInBounds(glm::mat3 &transform, glm::vec2 &mousePos) const;

        virtual glm::vec2 getTrueSize(glm::mat3 &transform) const;
    };
}
