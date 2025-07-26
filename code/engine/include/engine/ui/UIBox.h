//
// Created by mburc on 18.06.2025.
//

#pragma once
#include <engine/ui/UIObject.h>
#include <engine/rendering/Mesh.h>
#include <engine/rendering/Shader.h>
#include <engine/ui/UITexture.h>

namespace gl3::engine::ui {
    class UIBox : public UIObject {
    public:
        /// Create a UIBox with a 9-Patch \ref UITexture, as provided by \ref ResourceManager.
        /// @param uiTexture A reference to a texture.
        /// @param position The position of the object.
        /// @param size The size in pixels. Will be rounded up to automatically tile the texture.
        /// @param scale Will set the scale of the texture, but retain the size if possible.
        /// @param shader A shader for UIBoxes and Buttons.
        explicit UIBox(std::shared_ptr<UITexture> uiTexture, const glm::vec2 &position = glm::vec2(0.0f, 0.0f),
                       const glm::ivec2 &size = glm::ivec2(1, 1), float scale = 1.0f,
                       Shader shader = Shader("shaders/uiBox.vert", "shaders/uiBox.frag"));

        ///Draw first the box and then any children.
        void draw(glm::mat3& parentTransform) const override;
        ///Change the texture.
        void setTexture(std::shared_ptr<UITexture> uiTexture);

    protected:
        Shader shader;
        Mesh mesh;
        std::shared_ptr<UITexture> texture;
        glm::vec2 getTrueSize(glm::mat3 &transform) const override;
    };
}
