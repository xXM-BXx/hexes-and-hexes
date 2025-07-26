//
// Created by mburc on 18.11.2024.
//

#include <engine/rendering/Shader.h>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <engine/Assets.h>


namespace gl3::engine {

    struct glStatusData {
        int success;
        const char *shaderName;
        char infoLog[GL_INFO_LOG_LENGTH];
    };

    Shader::Shader(const fs::path &vertexShaderPath, const fs::path &fragmentShaderPath) {
        vertexShader = loadAndCompileShader(GL_VERTEX_SHADER, vertexShaderPath);
        fragmentShader = loadAndCompileShader(GL_FRAGMENT_SHADER, fragmentShaderPath);
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glDetachShader(shaderProgram, vertexShader);
        glDetachShader(shaderProgram, fragmentShader);
    }

    unsigned int Shader::loadAndCompileShader(GLuint shaderType, const fs::path &shaderPath) {
        auto shaderSource = readText(shaderPath);
        auto source = shaderSource.c_str();
        auto shaderID = glCreateShader(shaderType);
        glShaderSource(shaderID, 1, &source, nullptr);
        glCompileShader(shaderID);

        glStatusData compilationStatus{};
        compilationStatus.shaderName = shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment";
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compilationStatus.success);
        if(compilationStatus.success == GL_FALSE) {
            glGetShaderInfoLog(shaderID, GL_INFO_LOG_LENGTH, nullptr, compilationStatus.infoLog);
            throw std::runtime_error("ERROR: " + std::string(compilationStatus.shaderName) + " shader compilation failed:\n" +
            std::string(compilationStatus.infoLog));
        }

        return shaderID;
    }

    std::string Shader::readText(const fs::path &filePath) {
        std::ifstream sourceFile(resolveAssetPath(filePath));
        std::stringstream buffer;
        buffer << sourceFile.rdbuf();
        return buffer.str();
    }

    void Shader::setMatrix(const std::string &uniformName, glm::mat4 matrix) const {
        auto uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void Shader::setMatrix3(const std::string &uniformName, glm::mat3 matrix) const {
        auto uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
        glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
    }


    void Shader::setVector(const std::string &uniformName, glm::vec4 vector) const {
        auto uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
        glUniform4fv(uniformLocation, 1, glm::value_ptr(vector));
    }

    void Shader::setVector2(const std::string &uniformName, glm::vec2 vector) const {
        auto uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
        glUniform2fv(uniformLocation, 1, glm::value_ptr(vector));
    }

    void Shader::setInteger(const std::string &uniformName, int value) const
    {
        auto uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
        glUniform1i(uniformLocation, value);
    }

    void Shader::setFloat(const std::string &uniformName, float value) const {
        auto uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
        glUniform1f(uniformLocation, value);
    }

    void Shader::use() const {
        glUseProgram(shaderProgram);
    }

    Shader::~Shader() {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }


}
