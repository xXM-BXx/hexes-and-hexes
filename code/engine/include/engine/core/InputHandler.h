#pragma once
#include <memory>
#include <engine/core/Events.h>
#include <engine/core/Context.h>

#include "glm/vec2.hpp"

namespace hnh::engine {
    ///The different types of input events.
    enum class EventType {
        Key,
        MouseButton,
        MouseMove,
        Scroll
    };

    /// A polymorphic wrapper class for GLFWs' input system.
    /// For more information on the different values of the events, refer to GLFWs' documentation. This just passes the through.
    class InputEvent {
    public:
        virtual ~InputEvent() = default;

        virtual EventType type() const = 0;
    };

    ///A keyboard input event.
    struct KeyEvent : public InputEvent {
        int key, scancode, action, mods;

        KeyEvent(int k, int s, int a, int m) : key(k), scancode(s), action(a), mods(m) {
        }

        EventType type() const override { return EventType::Key; }
    };

    ///A mouse button input event.
    struct MouseButtonEvent : public InputEvent {
        int button, action, mods;

        MouseButtonEvent(int b, int a, int m) : button(b), action(a), mods(m) {
        }

        EventType type() const override { return EventType::MouseButton; }
    };

    ///A mouse move input event.
    struct MouseMoveEvent : public InputEvent {
        double xpos, ypos;

        MouseMoveEvent(double x, double y) : xpos(x), ypos(y) {
        }

        EventType type() const override { return EventType::MouseMove; }
    };

    ///A mouse scroll input event.
    struct ScrollEvent : public InputEvent {
        double xoffset, yoffset;

        ScrollEvent(double x, double y) : xoffset(x), yoffset(y) {
        }

        EventType type() const override { return EventType::Scroll; }
    };

    class InputHandler {
    public:
        /// Create an input handler.
        /// @param newContext A reference to a \ref hnh::engine::context::Context, needed to bind some input event callbacks from GLFW.
        explicit InputHandler(context::Context* newContext);
        InputHandler() = default;

        /// This event is invoked, whenever some input was not intercepted by the \ref hnh::engine::ObjectManager and its \ref hnh::engine::ui::UIObject s first.
        events::Event<InputHandler, std::unique_ptr<InputEvent>> unhandledInputEvent;


    private:
        context::Context * context;
        glm::vec2 lastMousePos;

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        void onKey(int key, int scancode, int action, int mods);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        void onMouseButton(int button, int action, int mods);
        static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
        void onMouseMove(double x, double y);
        static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
        void onScroll(double xOffset, double yOffset);
    };
}
