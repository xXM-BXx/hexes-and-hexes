//
// Created by mburc on 22.11.2024.
//

#pragma once
#include <filesystem>
#include <map>
#include <vector>

#include "Texture.h"
#include "glm/vec2.hpp"
#include "glm/detail/type_vec1.hpp"
#include "../utility/Utility.h"

namespace gl3::engine {
    ///One item in a \ref AtlasTexture
    struct AtlasItem {
        ///The cell coordinates, at which the item starts.
        glm::ivec2 AtlasCoords; //In Cells, Starting from Top Left
        ///The size of the item in cells.
        glm::ivec2 Size; //In Cells
        ///The origin off the item in pixels, starting from the top left.
        glm::ivec2 OriginOffset; //In Pixels, Starting from Top Left

        AtlasItem(glm::ivec2 coords = glm::ivec2(0, 0),
                  glm::ivec2 size = glm::ivec2(1, 1),
                  glm::ivec2 origin_offset = glm::ivec2(0, 0)) : AtlasCoords(coords),
                                                                 Size(size),
                                                                 OriginOffset(origin_offset) {
        }
    };

    class AtlasTexture {
        ///A map of \ref AtlasItem, with their \ref AtlasItem::AtlasCoords as the key.
        std::map<glm::ivec2, AtlasItem, CompareIvec2> AtlasItems;
        ///The \ref Texture, that the AtlasTexture uses.
        Texture Texture;
        ///The size of the cells in pixels.
        glm::ivec2 CellSize = glm::ivec2(1, 1);

    public:
        /// Create an Atlas Texture based on a path to a png. Should only be used by the \ref ResourceManager
        /// @param texturePath A path to a .png with /assets/ as its root. Will crash, if the file doesn't exist.
        /// @param cellSize Optionally set the cell size in pixels.
        /// @param jsonPath A path to a .json with /assets/ as its root. Will crash, if the .json is formated incorrectly.
        explicit AtlasTexture(const char *texturePath, glm::ivec2 cellSize = glm::ivec2(16, 16), const char* jsonPath = nullptr);

        ~AtlasTexture();

        /// Overrides an atlas items from an internal map.
        /// @param atlasItem The added \ref AtlasItem.
        void AddOrSetAtlasItem(AtlasItem atlasItem);

        /// Create and add an \ref AtlasItem.
        /// @param AtlasCoords The \ref AtlasItem::AtlasCoords.
        /// @param Size The \ref AtlasItem::Size.
        void MakeAtlasItem(glm::ivec2 AtlasCoords, glm::ivec2 Size);

        /// Create and add an \ref AtlasItem.
        /// @param AtlasCoords The \ref AtlasItem::AtlasCoords.
        /// @param Size The \ref AtlasItem::Size.
        /// @param OriginOffset The \ref AtlasItem::OriginOffset.
        void MakeAtlasItem(glm::ivec2 AtlasCoords, glm::ivec2 Size, glm::ivec2 OriginOffset);

        /// Create and add many AtlasItems, with identical size and origin.
        /// @param AtlasCoords A vector of \ref AtlasItem::AtlasCoords to add.
        /// @param Size The \ref AtlasItem::Size.
        /// @param OriginOffset The \ref AtlasItem::OriginOffset.
        void MakeAtlasItems(std::vector<glm::ivec2> AtlasCoords, glm::ivec2 Size, glm::ivec2 OriginOffset);

        /// Create and add many AtlasItems, with identical size and origin. May produce some \ref AtlasItem, which maps to no texture, but is easy to use.
        /// @param rows A vector of rows (\ref AtlasItem::AtlasCoords y-value) to add.
        /// @param width The amount of \ref AtlasItem to add per row.
        /// @param Size The \ref AtlasItem::Size.
        /// @param OriginOffset The \ref AtlasItem::OriginOffset.
        void MakeAtlasItems(std::vector<unsigned int> rows, unsigned int width, glm::ivec2 Size,
                            glm::ivec2 OriginOffset);

        /// Return if the given AtlasCoord exist in the \ref AtlasTexture.
        /// @param AtlasCoords The AtlasCoords to check.
        /// @return If the map contains this key.
        bool Contains(glm::ivec2 AtlasCoords);

        /// Get the \ref AtlasItem with the given coordinate. Will crash, if none are found.
        /// @param AtlasCoords The AtlasCoords to check.
        /// @return The \ref AtlasItem with the coordinate.
        AtlasItem GetAtlasItem(glm::ivec2 AtlasCoords);

        /// Get the cell size.
        /// @return Cell Size in pixels.
        auto GetCellSize() -> glm::ivec2;

        /// Get the textures size in pixels.
        /// @return The textures size in pixels.
        [[nodiscard]] glm::ivec2 GetTextureSize() const;

        /// Bind the texture. Called before drawing an object.
        void bindTexture() const;

        /// Load many \ref AtlasItem s from a .json file.
        /// @param texturePath The path of the .json file with the root /assets/. Will crash if the file is defined wrong.
        void loadAtlasTexture(const std::filesystem::path &texturePath);
    };
}
