#include <engine/core/AudioManager.h>

namespace hnh::engine::audio {
    AudioManager::AudioManager() : audioEngine() {
        ma_engine_init(nullptr, &audioEngine);
    }

    AudioManager::~AudioManager() {
        for (auto& sound : sounds)
            ma_sound_uninit(&sound.second);
        ma_engine_uninit(&audioEngine);
    }

    bool AudioManager::loadSound(const std::string &name, const std::string &path, bool looping) {
        ma_sound sound;
        if (ma_sound_init_from_file(&audioEngine, path.c_str(), 0, nullptr, nullptr, &sound) != MA_SUCCESS)
            return false;
        ma_sound_set_looping(&sound, looping);
        sounds[name] = sound;
        return true;
    }

    void AudioManager::play(const std::string &name) {
        auto sound = sounds.find(name);
        if (sound != sounds.end())
            ma_sound_start(&sound->second);
    }

    void AudioManager::stop(const std::string &name) {
        auto sound = sounds.find(name);
        if (sound != sounds.end())
            ma_sound_stop(&sound->second);
    }
}
