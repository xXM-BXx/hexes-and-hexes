#pragma once
#include <functional>
#include <array>
#include <list>
#include <numbers>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"


namespace hnh {
    ///A cubic/axial Hex Coordinate system, based of https://www.redblobgames.com/grids/hexagons/.
    struct HexCoords {
        enum class HexAxis { Q, R, S };

        int Q, R, S = 0;

        /// Construct from axial coords
        HexCoords(int q, int r) : Q(q), R(r), S(-q - r) {
        };

        HexCoords(int q, int r, int s) : Q(q), R(r), S(-q - r) {
        };

        HexCoords() : Q(0), R(0), S(0) {
        };

        //Operator
        HexCoords operator +(const HexCoords &b) const { return {this->Q + b.Q, this->R + b.R}; }
        HexCoords operator -(const HexCoords &b) const { return {this->Q - b.Q, this->R - b.R}; }
        HexCoords operator *(const HexCoords &b) const { return {this->Q * b.Q, this->R * b.R}; }
        HexCoords operator *(int scalar) const { return {this->Q * scalar, this->R * scalar}; }
        HexCoords operator /(int scalar) const { return hexRound({this->Q / scalar, this->R / scalar}); }
        bool operator ==(const HexCoords &b) const { return this->Q == b.Q && this->R == b.R; }

        bool operator <(const HexCoords &b) const;

        int hash();

        HexCoords &operator=(const HexCoords &b) = default;

        static std::array<HexCoords, 6> EdgeDirections;

        static std::array<HexCoords, 6> DiagonalDirections;

        /// Get the neighbors of a coordinate.
        /// @return Returns this plus each of the \ref EdgeDirections.
        [[nodiscard]] std::vector<HexCoords> neighborCoords() const;

        /// Get the distance between two points.
        int distanceTo(HexCoords &b) const;

        /// Get the length of a hex vector.
        int length() const;

        /// Returns the new position of a point after rotating it around the origin (0, 0).
        /// @param amount The amount to rotate by with each step being 60 degree clockwise.
        /// @return The rotated position.
        HexCoords rotatedBy(int amount);

        ///  Returns the new position of a point after rotating it around the pivot.
        /// @param amount The amount to rotate by with each step being 60 degree clockwise.
        /// @param aroundPivot The position of the pivot.
        /// @return The rotated position.
        HexCoords rotatedBy(int amount, HexCoords &aroundPivot);

        /// The product between two hex vectors.
        /// @param other The second hex vector.
        /// @return The dot product.
        [[nodiscard]] int dotProduct(const HexCoords &other) const;

        /// Get the result of a flood fill.
        /// @param startTile The starting position.
        /// @param depth The depth, with 1 being only the starting tile.
        /// @param reachableTiles A function to describe which tiles can be reached from another tile. E.g. [](HexCoords c){ return c.neighborCoords(); }.
        /// @return A two-dimensional vector of HexCoords, with each inner vector being the positions at one depth.
        static std::vector<std::vector<HexCoords> > floodFill(HexCoords startTile, int depth,
                                                              const std::function<std::vector<HexCoords>(
                                                                  const HexCoords &)> &reachableTiles);

        /// Find a path between points.
        /// @param startTile The start position.
        /// @param endTile The end position.
        /// @param eligibleTiles Which positions the path may pass through.
        /// @return A vector of the tiles that make up a path.
        static std::vector<HexCoords> findPath(HexCoords startTile, HexCoords endTile,
                                               std::vector<HexCoords> eligibleTiles);

        /// Draw a line between points.
        /// @param startTile The start position.
        /// @param endTile The end position.
        /// @return A vector of all positions which the line passes through.
        static std::vector<HexCoords> line(HexCoords startTile, HexCoords endTile);

        /// If the hex vector has a length of 1, find out in which direction it points.
        /// @return An int between 0 and 5, with 0 being up, going clockwise.
        int angleFromAxis() const;

        /// Get a vector with length 1, as needed for \ref HexCoords::angleFromAxis.
        /// @param origin The origin position.
        /// @param target The position which needs to be snapped.
        /// @return A vector with length 1 that points from the origin to the target.
        static HexCoords getNearestNormalizedAxis(const HexCoords &origin, const HexCoords &target);

        /// Follow a vectors direction, as long as the progressCondition is met.
        /// @param origin The starting point.
        /// @param direction The direction.
        /// @param progressCondition A function when to stop the line.
        /// @param maxDepth At what depth to stop the time at its latest.
        /// @return A vector of positions.
        static std::vector<HexCoords> followAxis(HexCoords origin, HexCoords direction,
                                                 const std::function<bool(const HexCoords &)> &progressCondition,
                                                 int maxDepth = 50);

        /// See if a line between to points is interrupted.
        /// @param origin The starting point.
        /// @param target The end point.
        /// @param coverCondition A function, what counts as an interruption for the line.
        /// @return If the line was not interrupted.
        static bool hasLineOfSight(HexCoords origin, HexCoords target,
                                   const std::function<bool(const HexCoords &)> &coverCondition);

        /// Method to convert the HexCoord position to a Vector2 for the purpose of visualization.
        /// @param hexSize The Size of the hexagon in pixels.
        /// Depends on 45-Degree corners of the hexagon to produce no overlap.
        [[nodiscard]] glm::vec2 toWorld(glm::ivec2 hexSize = glm::ivec2(32, 16)) const;

        /// Snap a world position to a hex coordinate.
        /// @param worldCoords The world position.
        /// @param hexSize The Size of the hexagon in pixels.
        /// Depends on 45-Degree corners of the hexagon to produce no overlap.
        /// @return The nearest HexCoords.
        static HexCoords fromWorld(glm::vec2 worldCoords, glm::ivec2 hexSize = glm::ivec2(32, 16));

    private:
        static glm::vec2 hexLerp(HexCoords a, HexCoords b, float t);

        static HexCoords hexRound(glm::dvec2 fracHexCoord);
    };
}

namespace std {
    ///A hash function required by some data structures.
    template<>
    struct hash<hnh::HexCoords> {
        std::size_t operator()(const hnh::HexCoords &hc) const noexcept {
            std::size_t h1 = std::hash<int>{}(hc.Q);
            std::size_t h2 = std::hash<int>{}(hc.R);
            // Combine the hashes using a simple hash-combine
            return h1 ^ (h2 << 1); // or use more robust mixing if needed
        }
    };
}
