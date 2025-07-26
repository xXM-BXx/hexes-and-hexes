//
// Created by s453388 on 27.05.2025.
//

#pragma once
#include <memory>
#include <vector>
#include <engine/rendering/GameObject.h>
#include <engine/ui/UIObject.h>
#include <engine/rendering/Shader.h>
#include "glad/glad.h"

namespace hnh::engine {
    class ObjectManager {
    public:
        /// Create an object manager.
        ObjectManager();
        /// This draws the added \ref GameObject s and \ref UIObject s to two different textures and then layers them on top of each other.
        void draw();
        ///Sorts the game objects by their z-Index.
        void sortObjects();

        /// Add a game object to an internal list.
        /// @tparam ObjectType The type of object added, needs to extend \ref GameObject.
        /// @tparam Args The arguments, the objects constructor requires.
        /// @param args The required arguments.
        /// @return A reference to the newly added object.
        template<typename ObjectType, typename ...Args>
        ObjectType *addObject(Args&&... args);

        /// Add a ui object to an internal list.
        /// @tparam ObjectType The type of ui object to add, needs to extend \ref UIObject.
        /// @tparam Args The arguments, the ui objects constructor requires.
        /// @param args The required arguments.
        /// @return A reference to the newly added ui object.
        template<typename ObjectType, typename... Args>
        ObjectType *addUIObject(Args&&... args);

        /// Reparent a UIObject from the top level to any other UIObject once. This will then render the child with the parent's transforms. Will lose access to the child afterwards.
        /// The better approach is to have your own class that extends \ref UIObject, and handle children and drawing there manually.
        /// @param newParent A pointer to the new parent object.
        /// @param child A pointer to the new child object.
        void reparentUIObject(ui::UIObject* newParent, ui::UIObject* child);

        /// See if any UIObjects intercept an \ref InputEvent. Should only be used by the \ref InputHandler.
        /// @param inputEvent A pointer to a \ref InputEvent.
        /// @param mousePos The mouse position, when the inputEvent happened.
        /// @return A bool if the event was intercepted.
        bool checkUIInput(InputEvent* inputEvent, glm::vec2 &mousePos);

        ///Should only be called by the \ref Context if the window size changes.
        void updateFBOs();

    private:
        std::vector<std::shared_ptr<GameObject> > objects;
        std::vector<std::unique_ptr<ui::UIObject> > ui;


        void updateFBO(GLuint &fbo, GLuint &tex) const;
        Shader fullScreenShader;
        GLuint fbo1, tex1, fbo2, tex2 = 0;
        GLuint fullscreenQuadVAO, fullscreenQuadVBO = 0;
        void createFBO(GLuint &fbo, GLuint &tex) const;
    };

    template<typename ObjectType, typename... Args>
    ObjectType *ObjectManager::addObject(Args&&... args) {
        static_assert(std::is_base_of<GameObject, ObjectType>::value, "T must derive from GameObject");

        auto obj = std::make_shared<ObjectType>(std::forward<Args>(args)...);
        ObjectType *ptr = obj.get();
        objects.push_back(obj);

        return ptr;
    }

    template<typename UIObjectType, typename... Args>
    UIObjectType *ObjectManager::addUIObject(Args&&... args) {
        static_assert(std::is_base_of<ui::UIObject, UIObjectType>::value, "T must derive from UIObject");

        auto obj = std::make_unique<UIObjectType>(std::forward<Args>(args)...);
        UIObjectType *ptr = obj.get();
        ui.push_back(std::move(obj));
        return ptr;
    }

}
