//
// Created by mburc on 18.11.2024.
//

#pragma once
#include <vector>

namespace hnh::engine {
    ///A mesh as created in the courses beginning.
    class Mesh {
    public:
        /// Create a mesh with vertices, indices and optional uvs.
        /// @param vertices 3 floats per vertex.
        /// @param indices 3 floats per tri.
        /// @param uvs 2 floats per vertex.
        explicit Mesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices,
                      const std::vector<float> &uvs = {0});

        /// Draws the mesh. Textures etc. need to be set beforehand.
        void draw() const;

        ~Mesh();

        // Delete copy constructor
        Mesh(const Mesh &other) = delete;

        Mesh operator=(const Mesh &other) = delete;

        // Explicit move constructor
        Mesh(Mesh &&other) noexcept {
            std::swap(this->VBO, other.VBO);
            std::swap(this->EBO, other.EBO);
            std::swap(this->VAO, other.VAO);
            std::swap(this->uvVBO, other.uvVBO);
            std::swap(this->hasUVs, other.hasUVs);
            std::swap(this->numberOfIndices, other.numberOfIndices);
        }

        Mesh &operator=(Mesh &&other) noexcept {
            std::swap(this->VBO, other.VBO);
            std::swap(this->EBO, other.EBO);
            std::swap(this->VAO, other.VAO);
            std::swap(this->uvVBO, other.uvVBO);
            std::swap(this->hasUVs, other.hasUVs);
            std::swap(this->numberOfIndices, other.numberOfIndices);
            return *this;
        }

        ///Update the mesh if e.g. the size should change with the new values.
        void updateMesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices,
                        const std::vector<float> &uvs = {0});

    private:
        unsigned int VAO = 0;;
        unsigned int VBO = 0;
        unsigned int EBO = 0;
        unsigned int numberOfIndices = 0;
        unsigned int uvVBO = 0;
        bool hasUVs = false;
    };
};

