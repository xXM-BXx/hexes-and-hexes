//
// Created by mburc on 12.05.2025.
//

#include <engine/tilemap/AutoTileTerrain.h>
#include <engine/Assets.h>

#include <engine/resource_loading/ResourceManager.h>

namespace gl3::engine {
    AutoTileTerrain::AutoTileTerrain(int terrainId) :
        terrainId(terrainId)
    {
    }

    void AutoTileTerrain::addNeighborRelation(std::array<bool, 6 * sizeof(bool)> neighbors, unsigned int textureId,
        glm::ivec2 atlasCoords) {
        uint8_t bitmask = 0;
        for (unsigned int i = 0; i < neighbors.size(); i++) {
            if (neighbors[i]) {
                bitmask = bitmask | (1 << i);
            }
        }
        NeighborRelations.emplace(bitmask, TextureAddress(textureId, atlasCoords));
    }

    void AutoTileTerrain::loadNeighborRelation(const fs::path &jsonFilePath) {
        auto jsonString = engine::ResourceManager::readText(jsonFilePath);

        auto json = json::parse(jsonString);

        auto relations = json["relations"];
        int defaultTextureId = json["defaultTextureId"];


        for (auto relation: relations) {
            auto atlasCoords = glm::ivec2(std::stoi(to_string(relation["atlasCoords"][0])), std::stoi(to_string(relation["atlasCoords"][1])));
            int textureId;
            if(relation["textureId"] != nullptr) {
                textureId = relation["textureId"];
            }else {
                textureId = defaultTextureId;
            }
            auto neighbors = std::array<bool, 6 * sizeof(bool)>();
            for (const auto& neighbor: relation["neighbors"]) {
                neighbors[stoi(to_string(neighbor))] = true ;
            }
            addNeighborRelation(neighbors, textureId ,atlasCoords);
        }
    }
}

