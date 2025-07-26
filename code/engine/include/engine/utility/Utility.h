//
// Created by mburc on 22.11.2024.
//

#pragma once
#include <vector>
#include <numbers>
#include "glm/vec2.hpp"


namespace gl3::engine {

    ///Needed for \ref AtlasTexture.
    struct CompareIvec2 {
        bool operator()(const glm::ivec2 &a, const glm::ivec2 &b) const {
            // Compare first by x, then by y
            return (a.x < b.x) || (a.x == b.x && a.y < b.y);
        }
    };

    class Utility {
    public:
        ///Turn a two-dimensional vector into a one-dimensional vector.
        template<typename T>
        static std::vector<T> flattenVec2D(const std::vector<std::vector<T> > &vecOfVecs) {
            std::vector<T> result;

            // Pre-allocate total size
            size_t totalSize = 0;
            for (const auto &v: vecOfVecs) {
                totalSize += v.size();
            }
            result.reserve(totalSize);

            // Copy elements
            for (const auto &v: vecOfVecs) {
                result.insert(result.end(), v.begin(), v.end());
            }

            return result;
        }
    };
}
