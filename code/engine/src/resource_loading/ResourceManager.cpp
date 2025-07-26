//
// Created by mburc on 19.11.2024.
//


#pragma once
#include <engine/resource_loading/ResourceManager.h>
#include <fstream>
#include <sstream>
#include <engine/Assets.h>
#include <stb_image/stb_image.h>
#include <engine/core/Context.h>

namespace hnh::engine {

    std::string ResourceManager::readText(const fs::path &filePath) {
        std::ifstream sourceFile(resolveAssetPath(filePath));
        std::stringstream buffer;
        buffer << sourceFile.rdbuf();
        return buffer.str();
    }

    void ResourceManager::writeText(const fs::path &filePath, const std::string &text) {
        std::ofstream targetFile(resolveAssetPath(filePath));
        targetFile << text;
    }

    Texture ResourceManager::loadTextureFromFile(const char *file, bool alpha)
    {
        auto fullPath = resolveTexturePath(file);
        // create texture object
        Texture texture;
        if (alpha)
        {
            texture.Internal_Format = GL_RGBA;
            texture.Image_Format = GL_RGBA;
        }
        // load image
        int width, height, nrChannels;
        unsigned char* data = stbi_load(fullPath, &width, &height, &nrChannels, 0);
        // now generate texture
        texture.Generate(width, height, data);
        // and finally free image data
        stbi_image_free(data);
        return texture;
    }
    Texture ResourceManager::loadTexture(const char *file, bool alpha)
    {
        return loadTextureFromFile(file, alpha);
    }

}
