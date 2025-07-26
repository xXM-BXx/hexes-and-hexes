#pragma once
#include <map>
#include <array>

#include "glm/vec2.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <filesystem>

class path;

using json = nlohmann::json;
namespace fs = std::filesystem;

namespace hnh::engine {
    ///A terrain stores a map of neighbor relations and texture addresses .
    class AutoTileTerrain {
        public:
            struct TextureAddress {
                ///The id of an \ref AtlasTexture as set in \ref HexGrid.
                unsigned int textureID;
                ///The AtlasCoordinates in the texture.
                glm::ivec2 atlasCoords;
                TextureAddress(unsigned int textureId, glm::ivec2 atlasCoords) : textureID(textureId), atlasCoords(atlasCoords) {}
                TextureAddress() : textureID(0), atlasCoords(glm::ivec2()) {}
            };
            ///An identifier of for the terrain.
            unsigned int terrainId;
            ///The map to find the needed texture for a relation.
            std::map<uint8_t, TextureAddress> NeighborRelations;

            /// Create a terrain.
            /// @param terrainId Set the id. Manually track which ids' are still available.
            explicit AutoTileTerrain(int terrainId);

            /// Add a relation based on the neighbors.
            /// @param neighbors An array, if the texture expects a neighbor of the same terrain in a direction, starting from the top, going clockwise.
            /// @param textureId The \ref TextureAddress::textureID.
            /// @param atlasCoords The \ref TextureAddress::atlasCoords.
            void addNeighborRelation(std::array<bool, 6> neighbors, unsigned int textureId, glm::ivec2 atlasCoords);

            /// Load many relations from a .json.
            /// @param jsonFilePath The path to the .json with root /assets/. Will crash if the .json is defined wrong.
            void loadNeighborRelation(const fs::path &jsonFilePath);
    };
}

