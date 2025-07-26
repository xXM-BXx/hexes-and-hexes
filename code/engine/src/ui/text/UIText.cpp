#pragma once
#include <engine/ui/text/UIText.h>

#include <engine/core/Engine.h>

namespace hnh::engine::ui {
    UIText::UIText(std::shared_ptr<BitmapFont> font, const std::u32string &text, const glm::vec2 &position, float scale,
                   Shader shader) : shader(std::move(shader)),
                                    font(std::move(font)),
                                    text(text),
                                    mesh(Mesh({
                                                  0, 0, 0,
                                                  1, 0, 0,
                                                  1, 1, 0,
                                                  0, 1, 0
                                              },
                                              {
                                                  0, 1, 2,
                                                  2, 3, 0
                                              }, {
                                                  0.0f, 0.0f,
                                                  1.0f, 0.0f,
                                                  1.0f, 1.0f,
                                                  0.0f, 1.0f
                                              })) {
        this->text = text;
        this->position = position;
        this->scale = scale;
        this->canInteract = false;
        generateText();
    }

    void UIText::generateText() {
        std::vector<float> vertices;
        std::vector<float> uvs;
        std::vector<unsigned int> indices;

        if (!font || text.empty()) return;

        glm::vec2 cursor(0.0f);
        unsigned int indexOffset = 0;
        auto atlasSize = font->getTextureSize();

        auto highestWidth = 0.0f;

        for (char32_t c: text) {
            if (c == '\n') {
                // Line break
                highestWidth = std::max(highestWidth, cursor.x);
                cursor.x = 0.0f;
                cursor.y += font->getLineHeight();
                continue;
            }

            auto glyph = font->getGlyph(c);

            // Line wrapping check
            if (maxWidth > 0 && cursor.x + glyph.size.x > maxWidth) {
                highestWidth = std::max(highestWidth, cursor.x);
                cursor.x = 0.0f;
                cursor.y += font->getLineHeight();
            }

            auto pos = cursor + glm::vec2(glyph.offset);
            auto size = glm::vec2(glyph.size);

            // Vertex positions (pixel space)
            glm::vec2 topLeft = pos;
            glm::vec2 topRight = pos + glm::vec2(size.x, 0.0f);
            glm::vec2 bottomRight = pos + size;
            glm::vec2 bottomLeft = pos + glm::vec2(0.0f, size.y);

            // Push vertices as flat floats (x, y)
            vertices.insert(vertices.end(), {topLeft.x, topLeft.y, 0.0f});
            vertices.insert(vertices.end(), {topRight.x, topRight.y, 0.0f});
            vertices.insert(vertices.end(), {bottomRight.x, bottomRight.y, 0.0f});
            vertices.insert(vertices.end(), {bottomLeft.x, bottomLeft.y, 0.0f});

            // UVs
            glm::vec2 uvTopLeft = glm::vec2(glyph.atlasPos) / glm::vec2(atlasSize);
            glm::vec2 uvBottomRight = (glm::vec2(glyph.atlasPos + glyph.size)) / glm::vec2(atlasSize);
            glm::vec2 uvTopRight = glm::vec2(uvBottomRight.x, uvTopLeft.y);
            glm::vec2 uvBottomLeft = glm::vec2(uvTopLeft.x, uvBottomRight.y);

            // Push UVs as flat floats (u, v)
            uvs.insert(uvs.end(), {uvTopLeft.x, uvTopLeft.y});
            uvs.insert(uvs.end(), {uvTopRight.x, uvTopRight.y});
            uvs.insert(uvs.end(), {uvBottomRight.x, uvBottomRight.y});
            uvs.insert(uvs.end(), {uvBottomLeft.x, uvBottomLeft.y});

            // Indices (two triangles)
            indices.insert(indices.end(), {
                               indexOffset + 0, indexOffset + 1, indexOffset + 2,
                               indexOffset + 2, indexOffset + 3, indexOffset + 0
                           });

            indexOffset += 4;

            // Move cursor
            cursor.x += glyph.advance;
        }
        mesh.updateMesh(vertices, indices, uvs);

        // Save final bounding box (rounded up to int)
        size = glm::ivec2(static_cast<int>(std::ceil(highestWidth)),
                                 static_cast<int>(std::ceil(cursor.y + font->getLineHeight())));
    }

    void UIText::setText(std::u32string text) {
        this->text = std::move(text);
        generateText();
    }

    void UIText::setFont(std::shared_ptr<BitmapFont> font) {
        this->font = std::move(font);
        generateText();
    }

    void UIText::setMaxWidth(unsigned int maxWidth) {
        this->maxWidth = maxWidth;
        generateText();
    }

    void UIText::draw(glm::mat3 &parentTransform) const {
        if (!visible) {
            return;
        }
        //Set Filtering while scaling to nearest. Pixelate
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        auto projection = Engine::get().getUIProjectionMatrix();

        auto transform = parentTransform * calculateLocalMatrix();

        // Before drawing:
        shader.use();
        shader.setMatrix("uProjection", projection);
        shader.setMatrix3("uTransform", transform);
        shader.setVector("uColor", color);


        //Bind Texture
        glActiveTexture(GL_TEXTURE0);
        shader.setInteger("uTexture", 0);
        font->bindTexture();

        shader.use();
        mesh.draw();

        for (const auto& child: children) {
            child->draw(transform);
        }
    }
}
