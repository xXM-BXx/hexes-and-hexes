#include <iostream>
#include <engine/core/AudioManager.h>

#include "engine/Assets.h"

namespace hnh::engine::audio {
    AudioManager::AudioManager() : audioEngine() {
        ma_engine_init(NULL, &audioEngine);
    }

    AudioManager::~AudioManager() {
        for (auto& sound : sounds)
            ma_sound_uninit(&sound.second);
        ma_engine_uninit(&audioEngine);
    }

    bool AudioManager::loadSound(const std::string &name, const std::filesystem::path& filePath, bool looping) {
        if (!std::filesystem::exists(filePath)) {
            std::cerr << "[SoundManager] File does not exist: " << filePath << std::endl;
            return false;
        }



        ma_sound sound;
        ma_result result = ma_sound_init_from_file(&audioEngine, filePath.string().c_str(), 0, NULL, NULL, &sound);
        if (result != MA_SUCCESS) {
            std::cerr << "[SoundManager] Failed to load sound: " << filePath << " (Error: " << result << ")" << std::endl;
            return false;
        }
        ma_sound_set_looping(&sound, looping);
        sounds[name] = sound;
        return true;
    }

    void AudioManager::play(const std::string &name) {
        auto sound = sounds.find(name);
        if (sound != sounds.end())
            ma_sound_start(&sound->second);
    }

    void AudioManager::playDirect(const std::string &path) {
        ma_engine_play_sound(&audioEngine, resolveAssetPath(path).string().c_str(), NULL);
    }

    void AudioManager::stop(const std::string &name) {
        auto sound = sounds.find(name);
        if (sound != sounds.end())
            ma_sound_stop(&sound->second);
    }

    void AudioManager::setGlobalVolume(float volume) {
        ma_engine_set_volume(&audioEngine, volume);
    }
}
