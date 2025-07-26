//
// Created by mburc on 18.11.2024.
//

#pragma once
#include <vector>
#include "glm/fwd.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "AtlasTexture.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Shader.h"
#include "Texture.h"

//#pragma warning(4:4596)

namespace fs = std::filesystem;

namespace gl3::engine {

    class Sprite : public GameObject {


    public:
        /// Create a sprite from an \ref AtlasTexture.
        /// @param atlasTexture Reference to a \ref AtlasTexture as provided by \ref ResourceManager.
        /// @param position The position of the sprite.
        /// @param shader The \ref Shader of the sprite
        explicit Sprite(std::shared_ptr<AtlasTexture> atlasTexture, const glm::vec2 &position = glm::vec2(0.0f, 0.0f),
                        Shader shader = Shader("shaders/sprite.vert", "shaders/sprite.frag"));

        /// Draw a sprite by binding texture, passing transforms, selecting texture regions and drawing the mesh.
        /// @param parentTransform The origin position and scale, which is multiplied with the sprites own transform.
        void draw(glm::mat3 &parentTransform) const override;
        void draw(glm::mat3 &parentTransform, bool isUI) const;

        // Copy Constructor
        Sprite(const Sprite &other) = delete;
        //Copy Operator delete
        Sprite& operator= (const Sprite &other) = delete;

        //Move Constructor
        Sprite(Sprite &&other) noexcept
            : shader(std::move(other.shader)),
              mesh(std::move(other.mesh)),
              atlasTexture(other.atlasTexture),
              zIndex(other.zIndex),
              atlasCoords(other.atlasCoords)
        {
            scale = other.scale;
        }
        //Define Move Operator
        Sprite& operator= (Sprite &&other) noexcept {
            shader = std::move(other.shader);
            mesh = std::move(other.mesh);
            atlasTexture = other.atlasTexture;
            zIndex = other.zIndex;
            scale = other.scale;
            atlasCoords = other.atlasCoords;
            return *this;
        }

        ///Set the sprites scale.
        void setScale(float scale);
        ///Set the sprites z-index.
        void setZIndex(float zIndex);
        ///get the sprites z-index.
        [[nodiscard]] float getZIndex() const override;
        ///Set the \ref AtlasTexture s \ref AtlasItem::AtlasCoords.
        void setAtlasCoords(const glm::ivec2 &atlasCoords);

        ///Reference to an \ref AtlasTexture.
        std::shared_ptr<AtlasTexture> atlasTexture;
        ///If the sprite should be drawn.
        bool visible = true;
    protected:
        Shader shader;
        Mesh mesh;

        //Typical 2D Transforms
        int zIndex = 0;

        glm::ivec2 atlasCoords = glm::ivec2(0,0);

    };
}
