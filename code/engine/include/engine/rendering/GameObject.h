//
// Created by mburc on 03.12.2024.
//

#pragma once
#include <memory>
#include <vector>
#include <glm/mat3x3.hpp>

namespace hnh::engine {
    ///Abstract parent class for all Game Objects.
    class GameObject {
    public:
        ///Needed for sorting by z-Index.
        bool operator <(const GameObject &b) const {
            return this->getZIndex() < b.getZIndex();
        };

        GameObject() = default;

        virtual ~GameObject() = default;

        // Prevent copies
        GameObject(const GameObject &) = delete;

        GameObject &operator=(const GameObject &) = delete;

        // Allow moves
        GameObject(GameObject &&) noexcept = default;

        GameObject &operator=(GameObject &&) noexcept = default;

        virtual void draw(glm::mat3 &parentTransform) const = 0;

        [[nodiscard]] virtual float getZIndex() const { return 0; }

        glm::vec2 position = glm::vec2(0, 0);
        float scale = 1;

        ///Get the local transform matrix. Scale will always keep aspect ratio.
        [[nodiscard]] glm::mat3 calculateLocalMatrix() const {
            auto transform = glm::mat3(1.0f);
            // Scale first (diagonal entries)
            transform[0][0] = scale; // X scale
            transform[1][1] = scale; // Y scale

            // Then translate (rightmost column)
            transform[2][0] = position.x;
            transform[2][1] = position.y;
            return transform;
        }
    protected:
        bool visible = true;
    };
}
