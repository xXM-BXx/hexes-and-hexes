//
// Created by mburc on 23.06.2025.
//

#pragma once
#include <memory>

#include <engine/ui/text/BitmapFont.h>

#include <engine/rendering/Shader.h>

#include <engine/ui/UIObject.h>

#include <engine/rendering/Mesh.h>

namespace gl3::engine::ui {
    class UIText : public UIObject {
    public:
        /// Create a text.
        /// @param font The font as provided by \ref ResourceManager.
        /// @param text The text content.
        /// @param position the position of the text.
        /// @param scale The scale of the text.
        /// @param shader The shader of the text.
        explicit UIText(std::shared_ptr<BitmapFont> font, const std::u32string &text = U"",
                        const glm::vec2 &position = glm::vec2(0.0f, 0.0f), float scale = 1.0f,
                        Shader shader = Shader("shaders/text.vert", "shaders/text.frag"));

        ///Create letters based on the stored text.
        void generateText();

        ///Save a text and update the displayed one.
        void setText(std::u32string text);

        ///Change the font.
        void setFont(std::shared_ptr<BitmapFont> font);

        ///Set the max width, before letter based line wrapping occurs.
        void setMaxWidth(unsigned int maxWidth);

        ///Draw first the text, and then any potential children.
        void draw(glm::mat3& parentTransform) const override;

    protected:
        std::shared_ptr<BitmapFont> font;
        std::u32string text;
        Shader shader;
        Mesh mesh;
        glm::vec4 color = glm::vec4(0, 0, 0, 1);
        unsigned int maxWidth = -1;
    };
}

