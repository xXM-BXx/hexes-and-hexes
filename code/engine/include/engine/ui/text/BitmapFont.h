#pragma once
#include <filesystem>
#include <map>

#include "engine/rendering/Texture.h"
#include "glm/vec2.hpp"

namespace hnh::engine::ui {
    ///A symbol in a font. Specifies data needed for text rendering.
    struct Glyph {
        glm::ivec2 atlasPos; // Top-left position in the texture (in pixels)
        glm::ivec2 size; // Width and height (in pixels)
        glm::ivec2 offset; // Offset to apply from pen position (in pixels)
        int advance; // How much to move the pen after this character (in pixels)
    };

    class BitmapFont {
    public:
        /// Create a font from a texture and a json.
        /// @param texturePath Path to the texture with root /asset/. Will crash if no .png is found.
        /// @param jsonPath Path to the .json with root /asset/. Will crash if .json is formatted wrong.
        BitmapFont(const char *texturePath, const char *jsonPath);
        ///Get a glyph corresponding to the char.
        ///@param c The character. Supports Unicode symbols.
        const Glyph& getGlyph(char32_t c) const;
        ///Get line height of the font.
        int getLineHeight() const;
        ///Bind the texture.
        void bindTexture();
        ///Get the texture size.
        glm::ivec2 getTextureSize() const;

    private:
        std::map<char32_t, Glyph> glyphs;
        Texture texture;
        int lineHeight;
        void parseJson(std::filesystem::path path);
    };
}
