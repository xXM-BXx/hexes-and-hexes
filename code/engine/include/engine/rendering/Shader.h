//
// Created by mburc on 18.11.2024.
//

#pragma once

#include <string>
#include <filesystem>
#include "glad/glad.h"
#include "glm/glm.hpp"


namespace fs = std::filesystem;

namespace gl3::engine {
    class Shader {
    public:
        /// Create a shader from a .vert and a .frag file.
        /// @param vertexShaderPath Path to a .vert file with the root /assets/. Will crash, if the file doesn't exist.
        /// @param fragmentShaderPath Path to a .frag file with the root /assets/. Will crash, if the file doesn't exist.
        Shader(const fs::path &vertexShaderPath, const fs::path &fragmentShaderPath);

        ~Shader();

        // Delete copy constructor
        Shader(const Shader &shader) = delete;

        Shader operator=(const Shader &shader) = delete;

        // Explicit move constructor
        Shader(Shader &&other) noexcept {
            std::swap(this->shaderProgram, other.shaderProgram);
            std::swap(this->vertexShader, other.vertexShader);
            std::swap(this->fragmentShader, other.fragmentShader);
        }

        Shader &operator=(Shader &&other) noexcept {
            std::swap(this->shaderProgram, other.shaderProgram);
            std::swap(this->vertexShader, other.vertexShader);
            std::swap(this->fragmentShader, other.fragmentShader);
            return *this;
        }

        /// Set a shaders' uniform mat4 by name.
        void setMatrix(const std::string &uniformName, glm::mat4 matrix) const;
        /// Set a shaders' uniform mat3 by name.
        void setMatrix3(const std::string &uniformName, glm::mat3 matrix) const;
        /// Set a shaders' uniform vec3 by name.
        void setVector(const std::string &uniformName, glm::vec4 vector) const;
        /// Set a shaders' uniform vec2 by name.
        void setVector2(const std::string &uniformName, glm::vec2 vector) const;
        /// Set a shaders' uniform int by name.
        void setInteger(const std::string &uniformName, int value) const;
        /// Set a shaders' uniform float by name.
        void setFloat(const std::string &uniformName, float value) const;
        ///Wrapper for glUseProgram.
        void use() const;

    private:
        unsigned int loadAndCompileShader(GLuint shaderType, const fs::path &shaderPath);

        std::string readText(const fs::path &filePath);

        unsigned int shaderProgram = 0;
        unsigned int vertexShader = 0;
        unsigned int fragmentShader = 0;
    };
}


