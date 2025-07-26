#pragma once

#include <filesystem>

#define GET_STRING(x) #x
#define GET_DIR(x) GET_STRING(x)

namespace fs = std::filesystem;

namespace hnh {
    ///Provides the root for asset loading.
    inline fs::path resolveAssetPath(const fs::path &relativeAssetPath) {
        auto mergedPath = ("./assets" / relativeAssetPath).make_preferred();
        return fs::canonical(mergedPath);
    }
    ///Provides the root for texture loading.
    inline const char* resolveTexturePath(const char* relativeTexturePath) {
        static std::string fullPath;
        const std::string root = "./assets/";
        fullPath = root + relativeTexturePath;
        return fullPath.c_str();
    }
}
