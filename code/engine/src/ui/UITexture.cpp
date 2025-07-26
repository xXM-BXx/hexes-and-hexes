#include <engine/ui/UITexture.h>

#include <engine/resource_loading/ResourceManager.h>

namespace hnh::engine::ui {
    UITexture::UITexture(const char *texturePath, glm::ivec4 newBorder) :
    texture(ResourceManager::loadTexture(texturePath, true)),
    border(newBorder) {

    }

    void UITexture::bindTexture() {
        texture.bind();
    }
}
