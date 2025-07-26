//
// Created by mburc on 19.11.2024.
//

#pragma once
#include <string>
#include <filesystem>
#include <engine/rendering/Texture.h>
#include <map>
#include <engine/rendering/AtlasTexture.h>
#include <engine/ui/UITexture.h>
#include <engine/ui/text/BitmapFont.h>

namespace fs = std::filesystem;

namespace hnh::engine {
    class ResourceManager {


    public:
        /// Get the content of a file.
        /// @param filePath The path of the file with root /assets/. Will crash if no file is found.
        /// @return The content of the file as a std::string.
        static std::string readText(const fs::path &filePath);

        /// Write text to a file.
        /// @param filePath The path of the file with root /assets/. Will crash if no file is found.
        /// @param text The contents to be written.
        static void writeText(const fs::path &filePath, const std::string &text);

        /// Load a resource, or create it if the provided file was not previously loaded.
        /// @tparam T Type of resource needs to be \ref AtlasTexture, \ref BitmapFont or \ref UITexture.
        /// @tparam Args The arguments required by the types constructor.
        /// @param file The file path with root /assets/. Will crash if no file is found.
        /// @param args The arguments required by the types constructor.
        /// @return A reference to the loaded resource.
        template<typename T, typename... Args>
        std::shared_ptr<T> loadResource(const char *file, Args &&... args);

        /// Load a texture. Used by resources like \ref AtlasTexture, \ref BitmapFont, \ref UITexture.
        /// @param file The path of the file with root /assets/. Will crash if no file is found.
        /// @param alpha If alpha should be enabled.
        /// @return A \ref Texture.
        static Texture loadTexture(const char *file, bool alpha);

    private:
        static Texture loadTextureFromFile(const char *file, bool alpha);
        std::map<std::filesystem::path, std::shared_ptr<AtlasTexture> > atlasTextures;
        std::map<std::filesystem::path, std::shared_ptr<ui::UITexture> > uiTextures;
        std::map<std::filesystem::path, std::shared_ptr<ui::BitmapFont> > fonts;
    };

    template<typename T, typename... Args>
    std::shared_ptr<T> ResourceManager::loadResource(const char *file, Args &&... args) {
        static_assert(
            (std::is_same_v<T, AtlasTexture> || std::is_same_v<T, ui::UITexture> || std::is_same_v<T, ui::BitmapFont>),
            "T needs to be an AtlasTexture, UI Texture, Font"); //Add code for UI Texture etc  later
        auto path = std::filesystem::path(file);
        if constexpr (std::is_same_v<T, AtlasTexture>) {
            if (atlasTextures.contains(std::filesystem::path(file))) {
                return std::static_pointer_cast<T>(atlasTextures.at(path));
            }
            auto atlasTexture = std::make_shared<T>(file, std::forward<Args>(args)...);
            atlasTextures.try_emplace(std::filesystem::path(file), atlasTexture);
            return atlasTexture;
        }
        if constexpr (std::is_same_v<T, ui::UITexture>) {
            if (uiTextures.contains(std::filesystem::path(file))) {
                return std::static_pointer_cast<T>(uiTextures.at(path));
            }
            auto uiTexture = std::make_shared<T>(file, std::forward<Args>(args)...);
            uiTextures.try_emplace(std::filesystem::path(file), uiTexture);
            return uiTexture;
        }
        if constexpr (std::is_same_v<T, ui::BitmapFont>) {
            if (fonts.contains(std::filesystem::path(file))) {
                return std::static_pointer_cast<T>(fonts.at(path));
            }
            auto font = std::make_shared<T>(file, std::forward<Args>(args)...);
            fonts.try_emplace(std::filesystem::path(file), font);
            return font;
        }
        return nullptr;
    }
}
