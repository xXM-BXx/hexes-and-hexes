#include <engine/ui/UIBox.h>

#include <utility>

#include "engine/core/Engine.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"


namespace hnh::engine::ui {
    UIBox::UIBox(std::shared_ptr<UITexture> uiTexture, const glm::vec2 &position, const glm::ivec2 &size, float scale,
                 Shader shader): texture(std::move(uiTexture)),
                                 shader(std::move(shader)),
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
        this->size = size;
        this->position = position;
        this->scale = scale;
    }

    void UIBox::draw(glm::mat3 &parentTransform) const {
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
        //Part for the 9-Slice-Scaling
        shader.setVector2("uBoxSize", size);
        shader.setVector2("uTextureSize", texture->getTexSize());
        shader.setVector("uBorder", texture->getBorder());

        //Bind Texture
        glActiveTexture(GL_TEXTURE0);
        shader.setInteger("uTexture", 0);
        texture->bindTexture();

        shader.use();
        mesh.draw();

        for (const auto& child: children) {
            child->draw(transform);
        }
    }

    void UIBox::setTexture(std::shared_ptr<UITexture> uiTexture) {
        this->texture = std::move(uiTexture);
    }

    glm::vec2 UIBox::getTrueSize(glm::mat3 &transform) const {
        auto scale = transform[0][0];
        auto border = texture->getBorder();
        auto borderSize = glm::vec2(border.x + border.y, border.z + border.w) * scale;
        auto tileSize = ((glm::vec2)texture->getTexSize() - glm::vec2(border.x + border.y, border.z + border.w)) * scale;
        auto centerArea = max(glm::vec2(0.0), (glm::vec2)size - borderSize);

        //Enforce minimum center size: at least one tile
        glm::vec2 minCenter = tileSize;
        centerArea = max(centerArea, minCenter);
        //Round up so all tiles fit
        glm::vec2 snappedCenter = ceil(centerArea / tileSize) * tileSize;

        return snappedCenter + borderSize;
    }
}
