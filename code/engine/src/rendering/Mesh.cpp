#include <iostream>
#include <ostream>
#include <engine/rendering/Mesh.h>
#include "glad/glad.h"



namespace hnh::engine {
    template<typename T>
    GLuint createBuffer(GLuint bufferType, const std::vector<T> &bufferData) {
        unsigned int buffer = 0;
        glGenBuffers(1, &buffer);
        glBindBuffer(bufferType, buffer);
        glBufferData(bufferType, bufferData.size() * sizeof(T), bufferData.data(), GL_STATIC_DRAW);
        return buffer;
    }

    Mesh::Mesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, const std::vector<float> &uvs):
            numberOfIndices(indices.size())
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Bind VBO
        VBO = createBuffer(GL_ARRAY_BUFFER, vertices);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);

        if(uvs.size() == vertices.size() * 2/3) {
            // Bind UVs
            uvVBO = createBuffer(GL_ARRAY_BUFFER, uvs);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(1);
            hasUVs = true;
        }

        // Bind EBO
        EBO = createBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);

        glBindVertexArray(0);
    }

    void Mesh::draw() const {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    Mesh::~Mesh() {
        glDeleteBuffers(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void Mesh::updateMesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, const std::vector<float> &uvs)
    {
        numberOfIndices = indices.size();

        glBindVertexArray(VAO);

        VBO = createBuffer(GL_ARRAY_BUFFER, vertices);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);

        if(uvs.size() == vertices.size() * 2/3) {
            // Bind UVs
            uvVBO = createBuffer(GL_ARRAY_BUFFER, uvs);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(1);
            hasUVs = true;
        }

        EBO = createBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);

        glBindVertexArray(0);
    }
}
