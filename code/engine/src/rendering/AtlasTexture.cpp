//
// Created by mburc on 22.11.2024.
//


#include <engine/rendering/AtlasTexture.h>
#include <engine/resource_loading/ResourceManager.h>
#include <nlohmann/json.hpp>


namespace hnh::engine {
    AtlasTexture::AtlasTexture(const char *texturePath, glm::ivec2 cellSize, const char *jsonPath) :
    Texture(ResourceManager::loadTexture(texturePath, true)),
    CellSize(cellSize){
        if(jsonPath) {
            loadAtlasTexture("json/atlas-textures/CharacterAtlas.json");
        }
    }

    AtlasTexture::~AtlasTexture() = default;

    void AtlasTexture::AddOrSetAtlasItem(AtlasItem atlasItem) {
        AtlasItems.insert_or_assign(atlasItem.AtlasCoords, atlasItem);
    }

    void AtlasTexture::MakeAtlasItem(glm::ivec2 AtlasCoords, glm::ivec2 Size) {
        auto offset = glm::ivec2(Size.x * CellSize.x / 2, Size.y * CellSize.y / 2);
        AtlasItems.insert_or_assign(AtlasCoords, AtlasItem(AtlasCoords, Size, offset));
    }

    void AtlasTexture::MakeAtlasItem(glm::ivec2 AtlasCoords, glm::ivec2 Size, glm::ivec2 OriginOffset) {
        AtlasItems.insert_or_assign(AtlasCoords, AtlasItem(AtlasCoords, Size, OriginOffset));
    }

    /// Creates similar Atlas Coords from vector
    /// @param AtlasCoords
    /// @param Size
    /// @param OriginOffset
    void AtlasTexture::MakeAtlasItems(std::vector<glm::ivec2> AtlasCoords, glm::ivec2 Size, glm::ivec2 OriginOffset) {
        for (auto AtlasCoord: AtlasCoords) {
            MakeAtlasItem(AtlasCoord, Size, OriginOffset);
        }
    }

    void AtlasTexture::loadAtlasTexture(const std::filesystem::path &texturePath) {
        auto jsonString = ResourceManager::readText(texturePath);
        auto json = nlohmann::json::parse(jsonString);
        auto defaultSize = glm::ivec2(json["defaultSize"][0], json["defaultSize"][1]);
        auto defaultOffset = glm::ivec2(json["defaultOffset"][0], json["defaultOffset"][1]);
        for (auto item: json["atlasItems"]) {
            auto atlasCoordinates = glm::ivec2(item["atlasCoordinates"][0], item["atlasCoordinates"][1]);
            glm::ivec2 size = defaultSize;
            if(item["size"] != nullptr) {
                size = glm::ivec2(item["size"][0], item["size"][1]);
            }
            glm::ivec2 offset = defaultOffset;
            if(item["offset"] != nullptr) {
                offset = glm::ivec2(item["offset"][0], item["offset"][1]);
            }
            MakeAtlasItem(atlasCoordinates, size, offset);
        }
    }

    void AtlasTexture::MakeAtlasItems(std::vector<unsigned int> rows, unsigned int width, glm::ivec2 Size,
                                      glm::ivec2 OriginOffset) {
        for (auto row: rows) {
            for (int i = 0; i < width; ++i) {
                MakeAtlasItem(glm::ivec2(i, row), Size, OriginOffset);
            }
        }
    }

    bool AtlasTexture::Contains(glm::ivec2 AtlasCoords) {
        return AtlasItems.find(AtlasCoords) != AtlasItems.end();
    }

    AtlasItem AtlasTexture::GetAtlasItem(glm::ivec2 AtlasCoords) {
        return AtlasItems.at(AtlasCoords);
    }

    glm::ivec2 AtlasTexture::GetCellSize(){
        return CellSize;
    }

    glm::ivec2 AtlasTexture::GetTextureSize() const {
        return {Texture.Width, Texture.Height};
    }


    void AtlasTexture::bindTexture() const {
        Texture.bind();
    }
}
