//
// Created by mburc on 06.07.2025.
//

#pragma once
#include <engine/rendering/AnimatedSprite.h>

#include <utility>
#include <nlohmann/json.hpp>

#include <engine/resource_loading/ResourceManager.h>

#include <engine/core/Engine.h>

namespace hnh::engine {
    AnimatedSprite::AnimatedSprite(std::shared_ptr<AtlasTexture> atlasTexture, const glm::vec2 &position,
                                   Shader shader) : Sprite(std::move(atlasTexture), position, std::move(shader)),
                                                    currentAnimation(std::move(Animation({
                                                            AnimationFrame(glm::ivec2(0, 0), 1.0f)
                                                        }))
                                                    ) {
        Engine* engine = &Engine::get();
        onUpdateHandle = engine->update.addListener([&](float _deltaTime){
            this->onUpdate(_deltaTime);
        });
    }

    AnimatedSprite::~AnimatedSprite() {
        Engine* engine = &Engine::get();
        engine->update.removeListener(onUpdateHandle);
    }

    void AnimatedSprite::play(const std::string &animationName) {
        {
            if (animationName != currentAnimationName) {
                if(!animations.contains(animationName)) {
                    throw std::runtime_error("Animation not found: " + animationName);
                }
                currentAnimation = animations[animationName];
                currentFrame = 0;
                elapsedTime = 0.0f;
                currentAnimationName = animationName;
            }
        }
    }

    void AnimatedSprite::onUpdate(float deltaTime) {
        if (animationFinished) {
            if (currentCallback) {
                currentCallback();
                currentCallback = nullptr;
            }
            playNextAnimation();
            return;
        }

        elapsedTime += deltaTime;

        const auto& frame = currentAnimation.frames[currentFrame];
        if (elapsedTime >= frame.duration) {
            elapsedTime = 0.0f;
            currentFrame++;

            if (currentFrame >= currentAnimation.frames.size()) {
                if (currentAnimation.looping) {
                    currentFrame = 0;
                } else {
                    animationFinished = true;
                    return;
                }
            }
            const auto &newFrame = currentAnimation.frames[currentFrame];
            atlasCoords = newFrame.atlasCoordinate;
        }
    }

    AnimationFrame AnimatedSprite::getCurrentFrame() const {
        return currentAnimation.frames[currentFrame];
    }

    void AnimatedSprite::addAnimation(const std::string &animationName, Animation animation) {
        animations[animationName] = animation;
    }

    void AnimatedSprite::enqueueAnimationStep(const std::string &name, std::function<void()> onFinished) {
        animationQueue.push({name, std::move(onFinished)});
    }

    void AnimatedSprite::loadAnimation(const std::filesystem::path &jsonFilePath) {
        auto jsonString = ResourceManager::readText(jsonFilePath);
        auto animationJson = nlohmann::json::parse(jsonString);
        auto animation = Animation();
        animation.looping = animationJson["looping"].get<bool>();
        for (const auto &animationJsonItem : animationJson["frames"]) {
            auto atlasCoordinates = glm::ivec2(animationJsonItem["atlasCoordinate"][0], animationJsonItem["atlasCoordinate"][1]);
            animation.frames.push_back(AnimationFrame(atlasCoordinates, animationJsonItem["duration"].get<float>()));
        }
        animations[animationJson["animationName"].get<std::string>()] = animation;
    }

    void AnimatedSprite::loadAnimations(const std::filesystem::path &jsonFilePath) {
        auto jsonString = ResourceManager::readText(jsonFilePath);
        auto animationsJson = nlohmann::json::parse(jsonString);
        for (auto animationJson: animationsJson) {
            auto newAnimation = Animation();
            newAnimation.looping = animationJson["looping"].get<bool>();
            for (auto animationFrameJson : animationJson["frames"]) {
                auto atlasCoordinates = glm::ivec2(animationFrameJson["atlasCoordinate"][0], animationFrameJson["atlasCoordinate"][1]);
                newAnimation.frames.push_back(AnimationFrame(atlasCoordinates, animationFrameJson["duration"].get<float>()));
            }
            animations[animationJson["animationName"].get<std::string>()] = newAnimation;
        }
    }

    void AnimatedSprite::playNextAnimation() {
        if (!animationQueue.empty()) {
            AnimationQueueItem step = animationQueue.front();
            animationQueue.pop();

            const auto& name = step.animationName;
            if (!animations.contains(name)) {
                throw std::runtime_error("Animation not found: " + name);
            }

            currentAnimation = animations.at(name);
            currentCallback = std::move(step.callback);
            currentFrame = 0;
            elapsedTime = 0.0f;
            animationFinished = false;
        } else {
            currentCallback = nullptr;
        }
    }

    void AnimatedSprite::forceStopAnimation() {
        animationQueue = {};              // clear the queue
        animationFinished = true;        // mark current as done
        currentCallback = nullptr;       // optional: skip callback
    }

    std::string AnimatedSprite::getCurrentAnimationName() const{
        return currentAnimationName;
    }

    glm::ivec2 AnimatedSprite::getAtlasCoords() const {
        return atlasCoords;
    }
}
