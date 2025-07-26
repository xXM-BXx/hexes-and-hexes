//
// Created by mburc on 18.06.2025.
//

#pragma once
#include <engine/rendering/Texture.h>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>

namespace hnh::engine::ui {
    ///A 9-Patch texture.
    class UITexture {
    public:
        ///Create a new UITexture from a texture path and a border.
        ///@param texturePath The path to the texture with root /asset/. Will fail, when no .png is found.
        ///@param newBorder Defines after how many pixels from each side the repeating section begins.
        explicit UITexture(const char *texturePath, glm::ivec4 newBorder = glm::ivec4(4));
        ~UITexture() = default;
        void bindTexture();
        glm::ivec4 getBorder() {
            return border;
        }
        glm::ivec2 getTexSize() {
            return {texture.Width, texture.Height};
        }
    private:
        Texture texture;
        glm::ivec4 border;
    };
}
