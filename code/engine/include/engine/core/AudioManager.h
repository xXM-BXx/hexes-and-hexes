#pragma once
#include <string>
#include <unordered_map>
#include <miniaudio/miniaudio.h>

namespace hnh::engine::audio {
    class AudioManager {
    public:
        AudioManager();

        ~AudioManager();

        bool loadSound(const std::string &name, const std::string &path, bool looping = false);

        void play(const std::string &name);

        void stop(const std::string &name);

    private:
        ma_engine audioEngine;
        std::unordered_map<std::string, ma_sound> sounds;
    };
}

