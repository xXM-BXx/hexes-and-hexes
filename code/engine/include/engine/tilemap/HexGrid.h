#pragma once
#include <map>

#include <engine/tilemap/AutoTileTerrain.h>
#include <engine/tilemap/HexTile.h>
#include <engine/tilemap/HexCoords.h>

namespace hnh::engine {
    ///A collection of any child of \ref HexTile.
    template<typename HexTileType>
    class HexGrid : public GameObject {
        static_assert(std::is_base_of_v<HexTile, HexTileType>, "T must derive from HexTile");

    public:
        ///Create the HexGrid with a world position and the tile size.
        explicit HexGrid(glm::vec2 position = glm::vec2(0, 0), glm::ivec2 tileSize = glm::ivec2(32, 16));

        ~HexGrid() override = default ;

        HexGrid(const HexGrid &other) = default;

        /// Draw the HexGrid, by drawing all tiles from top to bottom.
        /// @param parentTransform The potential parent transform of the hexgrid.
        void draw(glm::mat3 &parentTransform) const override;

        /// Add a tile by the texture directly.
        /// @param position The hex position of the tile.
        /// @param textureID The id for the texture as set in \ref HexGrid.
        /// @param atlasCoords The atlasCoords of the texture.
        void addOrOverwriteTile(HexCoords position, unsigned int textureID, glm::ivec2 atlasCoords);

        /// Add a tile by terrain, and let the grid handle auto tiling and connected textures.
        /// @param position The hex position of the tile.
        /// @param terrainID The terrain id of the terrain as previously set.
        void addOrOverwriteTile(HexCoords position, unsigned int terrainID);

        ///Remove a tile by position.
        void removeTile(HexCoords position);

        /// Get a reference to a tile by position.
        /// @param position The searched position.
        /// @return Reference to the tile. Nullptr if no tile found.
        HexTileType *getTileOrNull(HexCoords position);

        /// Get all tiles in the grid.
        /// @return A vector of references to all tiles in the grid.
        std::vector<HexTileType*> getAllTiles();

        /// Get the Hex Coordinate, in which the position lies.
        /// Will fail, if the HexGrid is drawn with a parent transform, which is not the default transform.
        /// @param pos The world position.
        /// @return The nearest HexCoordinate.
        HexCoords worldToGrid(glm::ivec2 pos);

        /// Get world position of a hex position.
        /// Will fail, if the HexGrid is drawn with a parent transform, which is not the default transform.
        /// @param pos The hex position.
        /// @return The world position at the center of the hex.
        glm::vec2 gridToWorld(HexCoords pos);

        ///Add a texture with id. Needed for e.g. \ref AutoTileTerrain.
        void addTexture(unsigned int textureID, std::shared_ptr<AtlasTexture> atlasTexture);

        ///Get the z-index.
        [[nodiscard]] float getZIndex() const override;

        ///Map of Terrains and ids. Needed for e.g. \ref AutoTileTerrain.
        std::map<unsigned int, AutoTileTerrain *> terrains;

    private:
        AutoTileTerrain::TextureAddress findTextureByTerrain(HexCoords position, unsigned int terrainID);
        bool ySort = true;
        void setZIndex(float index);

        struct HexCoordsCompare {
            bool operator()(const HexCoords &a, const HexCoords &b) const {
                return std::tie(a.Q, a.R) < std::tie(b.Q, b.R);
            }
        };

        float zIndex = 0;
        glm::vec2 worldPosition;
        glm::ivec2 tileSize;

        std::map<HexCoords, HexTileType, HexCoordsCompare> tiles;
        std::vector<HexTileType *> drawList;

        std::map<unsigned int, std::shared_ptr<AtlasTexture> > textures;

        void updateDrawList();
    };


    //I need to implement the whole class in the Header, due to templating.


    template<typename HexTileType>
    HexGrid<HexTileType>::HexGrid(glm::vec2 position, glm::ivec2 tileSize) : worldPosition(position),
                                                                             tileSize(tileSize) {
    }

    template<typename HexTileType>
    void HexGrid<HexTileType>::draw(glm::mat3 &parentTransform) const {
        auto transform = parentTransform * calculateLocalMatrix();

        if (ySort) {
            for (auto &val: drawList) {
                val->draw(transform);
            }
        }
    }

    template<typename HexTileType>
    void HexGrid<HexTileType>::addOrOverwriteTile(HexCoords position, unsigned int textureID, glm::ivec2 atlasCoords) {
        if (tiles.contains(position)) {
            //printf("Tile is being overwritten! \n");
        }
        auto texture = textures[textureID];
        auto floorSprite = Sprite(texture, glm::vec2(0, 0));
        floorSprite.setAtlasCoords(atlasCoords);

        auto newTile = HexTileType(position, std::move(floorSprite));

        newTile.useTerrain = false;
        tiles.emplace(position, std::move(newTile));
        updateDrawList();
    }

    template<typename HexTileType>
    void HexGrid<HexTileType>::addOrOverwriteTile(HexCoords position, unsigned int terrainID) {
        auto textureAdress = findTextureByTerrain(position, terrainID);

        addOrOverwriteTile(position, textureAdress.textureID, textureAdress.atlasCoords);

        tiles.at(position).useTerrain = true;
        tiles.at(position).terrain = terrainID;
        //Update Neighbors
        //Get all relevant neighbors
        std::array<HexTile *, 6> neighbors = {};
        for (int i = 0; i < 6; ++i) {
            auto neighbor = getTileOrNull(position + HexCoords::EdgeDirections[i]);
            if (neighbor != nullptr && neighbor->useTerrain && neighbor->terrain == terrainID) {
                neighbors[i] = neighbor;
            }
        }
        for (int i = 0; i < 6; ++i) {
            if (neighbors[i] != nullptr) {
                auto textureAddress = findTextureByTerrain(neighbors[i]->HexPosition, terrainID);
                neighbors[i]->getFloorSprite()->setAtlasCoords(textureAddress.atlasCoords);
                neighbors[i]->getFloorSprite()->atlasTexture = textures[textureAddress.textureID];
            }
        }
    }


    template<typename HexTileType>
    AutoTileTerrain::TextureAddress HexGrid<HexTileType>::findTextureByTerrain(
        HexCoords position, unsigned int terrainID) {
        std::array<bool, 6> neighbors = {{false, false, false, false, false, false}};
        //Get all relevant neighbors
        for (int i = 0; i < 6; ++i) {
            auto neighbor = getTileOrNull(position + HexCoords::EdgeDirections[i]);
            if (neighbor != nullptr && neighbor->useTerrain && neighbor->terrain == terrainID) {
                neighbors[i] = true;
            }
        }

        //Convert bool array to bitmask
        uint8_t bitmask = 0;
        for (unsigned int i = 0; i < neighbors.size(); i++) {
            if (neighbors[i]) {
                bitmask = bitmask | (1 << i);
            }
        }
        //Find Texture by bitmask
        return terrains.at(terrainID)->NeighborRelations[bitmask];
    }


    template<typename HexTileType>
    HexTileType* HexGrid<HexTileType>::getTileOrNull(HexCoords position) {
        auto tile = tiles.find(position);
        if (tile != tiles.end()) {
            return &tile->second;
        } else {
            return nullptr;
        }
    }

    template<typename HexTileType>
    std::vector<HexTileType *> HexGrid<HexTileType>::getAllTiles() {
        std::vector<HexTileType*> tilePointers;
        tilePointers.reserve(tiles.size()); // Optional but efficient
        for (auto& pair : tiles) {
            tilePointers.push_back(&pair.second);
        }
        return tilePointers;
    }

    template<typename HexTileType>
    HexCoords HexGrid<HexTileType>::worldToGrid(glm::ivec2 pos) {
        return HexCoords::fromWorld(pos - (glm::ivec2)position, glm::vec2(tileSize.x * scale, tileSize.y * scale));
    }

    template<typename HexTileType>
    glm::vec2 HexGrid<HexTileType>::gridToWorld(HexCoords pos) {
        return (pos.toWorld(tileSize) * scale) + position ;
    }

    template<typename HexTileType>
    void HexGrid<HexTileType>::addTexture(unsigned int textureID, std::shared_ptr<AtlasTexture> atlasTexture) {
        textures.emplace(textureID, atlasTexture);
    }

    template<typename HexTileType>
    float HexGrid<HexTileType>::getZIndex() const {
        return zIndex;
    }

    template<typename HexTileType>
    void HexGrid<HexTileType>::setZIndex(const float index) {
        zIndex = index;
    }

    template<typename HexTileType>
    void HexGrid<HexTileType>::updateDrawList() {
        for (auto &pair: tiles) {
            auto y = pair.first.toWorld().y;
            pair.second.setZIndex(y);
        }
        drawList.clear();
        for (auto &tile: tiles | std::ranges::views::values) {
            drawList.push_back(&tile);
        }
        std::sort(drawList.begin(), drawList.end(),
                  [](const HexTile *a, const HexTile *b) {
                      return a->HexPosition.toWorld().y < b->HexPosition.toWorld().y;
                  });
    }

    template<typename HexTileType>
    void HexGrid<HexTileType>::removeTile(HexCoords position) {
        if (tiles.contains(position)) {
            auto terrainID = getTileOrNull(position)->terrain;

            tiles.erase(position);
            updateDrawList();


            //Fix Autotiling for adjacent tiles
            std::array<HexTile *, 6> neighbors = {};
            for (int i = 0; i < 6; ++i) {
                auto neighbor = getTileOrNull(position + HexCoords::EdgeDirections[i]);
                if (neighbor != nullptr && neighbor->useTerrain && neighbor->terrain == terrainID) {
                    neighbors[i] = neighbor;
                }
            }
            for (int i = 0; i < 6; ++i) {
                if (neighbors[i] != nullptr) {
                    auto textureAddress = findTextureByTerrain(neighbors[i]->HexPosition, terrainID);
                    neighbors[i]->getFloorSprite()->setAtlasCoords(textureAddress.atlasCoords);
                    neighbors[i]->getFloorSprite()->atlasTexture = textures[textureAddress.textureID];
                }
            }
        }
    }
}
