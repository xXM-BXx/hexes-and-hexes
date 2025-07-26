#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>
#include <miniaudio/miniaudio.h>

namespace hnh::engine::audio {
    class AudioManager {
    public:
        AudioManager();

        ~AudioManager();

        bool loadSound(const std::string &name, const std::filesystem::path& filePath, bool looping = false);

        void play(const std::string &name);
        void playDirect(const std::string &path);

        void stop(const std::string &name);

        void setGlobalVolume(float volume);

    private:
        ma_engine audioEngine;
        std::unordered_map<std::string, std::unique_ptr<ma_sound>> sounds;
    };
}

