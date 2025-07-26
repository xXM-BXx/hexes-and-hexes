//
// Created by mburc on 23.06.2025.
//

#pragma once
#include <engine/ui/text/BitmapFont.h>

#include <engine/resource_loading/ResourceManager.h>
#include <codecvt>
#include <nlohmann/json.hpp>

namespace gl3::engine::ui {
    BitmapFont::BitmapFont(const char *texturePath, const char *jsonPath): texture(
        ResourceManager::loadTexture(texturePath, true)) {
        parseJson(std::filesystem::path(jsonPath));
    }

    const Glyph &BitmapFont::getGlyph(char32_t c) const {
        return glyphs.at(c);
    }

    int BitmapFont::getLineHeight() const {
        return lineHeight;
    }

    void BitmapFont::bindTexture() {
        texture.bind();
    }

    glm::ivec2 BitmapFont::getTextureSize() const {
        return {texture.Width, texture.Height};
    }

    void BitmapFont::parseJson(std::filesystem::path path) {
        auto jsonString = ResourceManager::readText(path);
        auto font = nlohmann::json::parse(jsonString);
        lineHeight = font["lineHeight"];

        auto defaultSize = glm::vec2(font["defaultSize"][0], font["defaultSize"][1]);
        auto defaultOffset = glm::vec2(font["defaultOffset"][0], font["defaultOffset"][1]);
        auto defaultAdvance = font["defaultAdvance"];


        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
        for (auto& [charKey, glyph] : font["glyphs"].items()) {
            char32_t c = converter.from_bytes(charKey)[0];; // Get the first character of the key string
            Glyph newGlyph;
            newGlyph.atlasPos = glm::ivec2(glyph["atlasPos"][0], glyph["atlasPos"][1]);
            if (glyph["size"] != nullptr) {
                newGlyph.size = glm::ivec2(glyph["size"][0], glyph["size"][1]);
            } else {
                newGlyph.size = defaultSize;
            }
            if (glyph["offset"] != nullptr) {
                newGlyph.offset = glm::ivec2(glyph["offset"][0], glyph["offset"][1]);
            } else {
                newGlyph.offset = defaultOffset;
            }
            if (glyph["advance"] != nullptr) {
                newGlyph.advance = glyph["advance"];
            } else {
                newGlyph.advance = defaultAdvance;
            }
            glyphs[c] = newGlyph;
        }

    }
}
