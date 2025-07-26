#include <numbers>
#include <valarray>
#include <engine/tilemap/HexCoords.h>
#include <queue>

namespace hnh {
    //Start at top, Clockwise
    std::array<HexCoords, 6> HexCoords::EdgeDirections = {
        HexCoords(0, -1, +1), HexCoords(+1, -1, 0), HexCoords(+1, 0, -1),
        HexCoords(0, +1, -1), HexCoords(-1, +1, 0), HexCoords(-1, 0, +1),
    };

    std::array<HexCoords, 6> HexCoords::DiagonalDirections = {
        HexCoords(+2, -1, -1), HexCoords(+1, -2, +1), HexCoords(-1, -1, +2),
        HexCoords(-2, +1, +1), HexCoords(-1, +2, -1), HexCoords(+1, +1, -2)
    };

    bool HexCoords::operator<(const HexCoords &b) const {
        return this->length() < b.length();
    }

    int HexCoords::hash() {
        int h1 = std::hash<int>{}(this->Q);
        int h2 = std::hash<int>{}(this->R);
        return h1 ^ (h2 << 1);
    }

    std::vector<HexCoords> HexCoords::neighborCoords() const {
        std::vector<HexCoords> neighbors;
        for (auto &edgeDirection: EdgeDirections) {
            neighbors.push_back(*this + edgeDirection);
        }
        return neighbors;
    }

    int HexCoords::distanceTo(HexCoords &b) const {
        return (*this - b).length();
    }

    int HexCoords::length() const {
        return (std::abs(this->Q) + std::abs(this->R) + std::abs(this->S)) / 2;
    }

    HexCoords HexCoords::rotatedBy(int amount) {
        auto point = this;
        if (amount > 0) {
            for (int i = 1; i <= amount; i++) *point = HexCoords(-point->R, -point->S, -point->Q);
        } else if (amount < 0) {
            for (int i = -1; i >= amount; i--) *point = HexCoords(-point->S, -point->Q, -point->R);
        }
        return *point;
    }

    HexCoords HexCoords::rotatedBy(int amount, HexCoords &aroundPivot) {
        auto translated = *this - aroundPivot;

        auto rotated = translated.rotatedBy(amount);
        auto translatedBack = rotated + aroundPivot;
        return translatedBack;
    }



    int HexCoords::dotProduct(const HexCoords &other) const {
        return Q * other.Q + R * other.R + S * other.S;
    }

    std::vector<std::vector<HexCoords> > HexCoords::floodFill(HexCoords startTile, int depth,
                                                              const std::function<std::vector<HexCoords>(const HexCoords &)>
                                                              &reachableTiles) {
        auto visited = std::vector<HexCoords>();
        auto tilesAtDepth = std::vector<std::vector<HexCoords> >{{startTile}};
        for (auto currentDepth = 1; currentDepth < depth; currentDepth++) {
            tilesAtDepth.emplace_back();
            auto previous = tilesAtDepth[currentDepth - 1];
            for (auto tile: previous) {
                auto reachable = reachableTiles(tile);
                for (auto neighbor: reachable) {
                    if (std::find(visited.begin(), visited.end(), neighbor) != visited.end()) continue;
                    visited.push_back(neighbor);
                    tilesAtDepth[currentDepth].push_back(neighbor);
                }
            }
        }
        return tilesAtDepth;
    }

    std::vector<HexCoords> HexCoords::findPath(HexCoords startTile, HexCoords endTile,
                                               std::vector<HexCoords> eligibleTiles) {
        auto frontier = std::queue<HexCoords>();
        frontier.push(startTile);
        std::unordered_map<HexCoords, HexCoords> cameFrom;
        cameFrom[startTile] = startTile;

        while (frontier.size() != 0) {
            auto current = frontier.front();
            frontier.pop();
            for (auto next: current.neighborCoords()) {
                if (std::find(eligibleTiles.begin(), eligibleTiles.end(), next) != eligibleTiles.end() && !(
                        cameFrom.count(next) > 0)) {
                    frontier.push(next);
                    cameFrom[next] = current;
                }
            }
        }

        auto currentPosition = endTile;
        auto path = std::vector<HexCoords>();
        while (currentPosition != startTile) {
            path.push_back(currentPosition);
            currentPosition = cameFrom[currentPosition];
        }

        path.push_back(startTile);
        std::reverse(path.begin(), path.end());
        return path;
    }

    std::vector<HexCoords> HexCoords::line(HexCoords startTile, HexCoords endTile) {
        auto length = startTile.distanceTo(endTile);
        std::vector<HexCoords> results;
        for (int i = 0; i < length; ++i) {
            results.push_back(hexRound(hexLerp(startTile, endTile, 1.0f/length * i)));
        }
        results.push_back(endTile);
        return results;
    }

    glm::vec2 HexCoords::hexLerp(HexCoords a, HexCoords b, float t) {
        return {
            std::lerp(a.Q, b.Q, t),
            std::lerp(a.R, b.R, t)
        };
    }

    int HexCoords::angleFromAxis() const {
        if(*this == HexCoords(0, -1)) {
            return 0;
        }
        if(*this == HexCoords(1, -1)) {
            return 1;
        }
        if(*this == HexCoords(1, 0)) {
            return 2;
        }
        if(*this == HexCoords(0, 1)) {
            return 3;
        }
        if(*this == HexCoords(-1, 1)) {
            return 4;
        }
        if(*this == HexCoords(-1, 0)) {
            return 5;
        }
        return 0;
    }

    HexCoords HexCoords::getNearestNormalizedAxis(const HexCoords &origin, const HexCoords &target) {
        HexCoords v = target - origin;

        const std::array<HexCoords, 6> directions = {
            HexCoords(1, -1, 0),
            HexCoords(1, 0, -1),
            HexCoords(0, 1, -1),
            HexCoords(-1, 1, 0),
            HexCoords(-1, 0, 1),
            HexCoords(0, -1, 1)
        };

        HexCoords bestDir = directions[0];
        int bestDot = v.dotProduct(directions[0]);

        for (size_t i = 1; i < directions.size(); ++i) {
            int dot = v.dotProduct(directions[i]);
            if (dot > bestDot) {
                bestDot = dot;
                bestDir = directions[i];
            }
        }

        return bestDir;
    }

    std::vector<HexCoords> HexCoords::followAxis(HexCoords origin, HexCoords direction,
                                                 const std::function<bool(const HexCoords &)> &progressCondition, int maxDepth) {
        std::vector<HexCoords> result = {};
        if(direction.length() != 1) {
            //printf("Direction must be normalized");
            return result;
        }
        bool shouldContinue = true;
        auto lastPos = origin;
        int currentDepth = 0;
        while (shouldContinue) {
            auto nextPos = lastPos + direction;
            if (progressCondition(nextPos)) {
                result.push_back(nextPos);
                lastPos = nextPos;
            }else {
                shouldContinue = false;
            }
            currentDepth++;
            if(currentDepth >= maxDepth) {
                shouldContinue = false;
            }
        }
        return result;
    }

    bool HexCoords::hasLineOfSight(HexCoords origin, HexCoords target,
        const std::function<bool(const HexCoords &)> &coverCondition) {
        for (auto tile: HexCoords::line(origin, target)) {
            if (coverCondition(tile)) {
                return false;
            }
        }
        return true;
    }


    glm::vec2 HexCoords::toWorld(glm::ivec2 hexSize) const {
        float height = hexSize.y;
        float width = hexSize.x;

        auto rStep = glm::ivec2(0, height);
        auto qStep = glm::ivec2(width * 0.75f, height / 2);

        return qStep * this->Q + rStep * this->R;
    }

    HexCoords HexCoords::fromWorld(glm::vec2 worldPos, glm::ivec2 hexSize) {
        float height = hexSize.y;
        float width = hexSize.x;

        auto rStep = glm::ivec2(0, height);
        auto qStep = glm::ivec2(width * 0.75f, height / 2);

        //Inverse transform
        float inverse = 1.0f / (qStep.x * rStep.y - rStep.x * qStep.y);

        auto qFloat = (worldPos.x * rStep.y - worldPos.y * rStep.x) * inverse;
        auto rFloat = (worldPos.y * qStep.x - worldPos.x * qStep.y) * inverse;

        return hexRound(glm::vec2(qFloat, rFloat));
    }

    HexCoords HexCoords::hexRound(glm::dvec2 fracHexCoord) {
        // Convert to cube and round
        double qFrac = fracHexCoord.x;
        double rFrac = fracHexCoord.y;
        double sFrac = -qFrac - rFrac;

        int q = std::round(qFrac);
        int r = std::round(rFrac);
        int s = std::round(sFrac);

        double qDiff = std::abs(q - qFrac);
        double rDiff = std::abs(r - rFrac);
        double sDiff = std::abs(s - sFrac);

        if (qDiff > rDiff && qDiff > sDiff) q = -r - s;
        else if (rDiff > sDiff) r = -q - s;
        else s = -q - r;

        return {q, r, s};
    }
}
