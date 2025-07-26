//
// Created by mburc on 18.11.2024.
//

#include <iostream>
#include <engine/rendering/Sprite.h>

#include <engine/rendering/AtlasTexture.h>
#include <engine/Assets.h>

#include <utility>

#include <engine/core/Engine.h>
#include "GLFW/glfw3.h"
#include "glm/fwd.hpp"
#include "glm/ext/matrix_clip_space.hpp"


namespace gl3::engine {
    Sprite::Sprite(std::shared_ptr<AtlasTexture> atlasTexture, const glm::vec2 &position,
                   Shader shader): shader(std::move(shader)),
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
                                             })),
                                   atlasTexture(std::move(atlasTexture)) {
        this->position = position;
    }


    void Sprite::draw(glm::mat3 &parentTransform, bool isUI) const {
        if(!visible) return;
        //Enable Sprite Alpha Channel
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //Set Filtering while scaling to nearest. Pixelate
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        auto atlasItem = atlasTexture->GetAtlasItem(atlasCoords);

        shader.use();

        auto projection = engine::Engine::get().getSceneProjectionMatrix();
        if(isUI) projection = Engine::get().getUIProjectionMatrix();
        shader.setMatrix("uProjection", projection);

        auto transform = parentTransform * calculateLocalMatrix();
        shader.setMatrix3("uTransform", transform);

        shader.setVector2("uRegionSize", atlasItem.Size * atlasTexture->GetCellSize());
        shader.setVector2("uRegionOrigin", atlasItem.OriginOffset);
        if(isUI) shader.setVector2("uRegionOrigin", {0, 0});
        shader.setVector2("uAtlasOffset", atlasCoords * atlasTexture->GetCellSize());
        shader.setVector2("uAtlasSize", atlasTexture->GetTextureSize());

        glActiveTexture(GL_TEXTURE0);
        shader.setInteger("uTexture", 0);
        atlasTexture->bindTexture();


        shader.use();
        mesh.draw();
    }

    void Sprite::draw(glm::mat3 &parentTransform) const {
        if(!visible) return;
        //Enable Sprite Alpha Channel
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //Set Filtering while scaling to nearest. Pixelate
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        auto atlasItem = atlasTexture->GetAtlasItem(atlasCoords);

        shader.use();

        auto projection = engine::Engine::get().getSceneProjectionMatrix();
        shader.setMatrix("uProjection", projection);

        auto transform = parentTransform * calculateLocalMatrix();
        shader.setMatrix3("uTransform", transform);

        shader.setVector2("uRegionSize", atlasItem.Size * atlasTexture->GetCellSize());
        shader.setVector2("uRegionOrigin", atlasItem.OriginOffset);
        shader.setVector2("uAtlasOffset", atlasCoords * atlasTexture->GetCellSize());
        shader.setVector2("uAtlasSize", atlasTexture->GetTextureSize());

        glActiveTexture(GL_TEXTURE0);
        shader.setInteger("uTexture", 0);
        atlasTexture->bindTexture();


        shader.use();
        mesh.draw();
    }

    void Sprite::setScale(float scale) {
        this->scale = scale;
    }

    void Sprite::setZIndex(float zIndex) {
        this->zIndex = zIndex;
    }

    float Sprite::getZIndex() const {
        return zIndex;
    }

    void Sprite::setAtlasCoords(const glm::ivec2 &atlasCoords) {
        if (this->atlasCoords == atlasCoords) {
            //printf("This Atlas Coord is already set!\n");
            return;
        }
        if (atlasTexture->Contains(atlasCoords)) {
            this->atlasCoords = atlasCoords;
            return;
        }
        //printf("This Atlas Coord is not defined for the Atlas!\n");
    }
}
