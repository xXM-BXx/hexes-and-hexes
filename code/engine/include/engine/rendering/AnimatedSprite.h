#pragma once
#include <queue>
#include <engine/rendering/Sprite.h>
#include <engine/core/Events.h>

#include <engine/core/Engine.h>


namespace hnh::engine {
    /// The Data required for one frame of an \ref Animation.
    struct AnimationFrame {
        ///The Atlas Coordinate of this frame.
        glm::ivec2 atlasCoordinate;
        ///Duration in Seconds.
        float duration;

        AnimationFrame(glm::ivec2 coords, float duration)
            : atlasCoordinate(coords), duration(duration) {
        }

        // Default constructor
        AnimationFrame() = default;
    };

    ///A list of \ref AnimationFrame.
    struct Animation {
        ///The frames.
        std::vector<AnimationFrame> frames;
        ///If the Animation should loop forever.
        bool looping;

        // Default constructor
        Animation() = default;

        explicit Animation(std::vector<AnimationFrame> frames, bool looping = false)
            : frames(std::move(frames)), looping(looping) {
        }
    };

    ///A connection of an animations name, as set in \ref AnimatedSprite::addAnimation, and a callback.
    struct AnimationQueueItem {
        std::string animationName;
        std::function<void()> callback;
    };

    class AnimatedSprite : public Sprite{
    public:
        /// Create an animated sprite without any animations.
        /// @param atlasTexture A pointer to an \ref AtlasTexture as provided by the \ref hnh::engine::ResourceManager.
        /// @param position The position of the object.
        /// @param shader The shaders for the sprite.
        explicit AnimatedSprite(std::shared_ptr<AtlasTexture> atlasTexture,
                                const glm::vec2 &position = glm::vec2(0.0f, 0.0f),
                                Shader shader = Shader("shaders/sprite.vert", "shaders/sprite.frag"));


        // Copy Constructor
        AnimatedSprite(const AnimatedSprite &other) = delete;

        //Copy Operator
        AnimatedSprite &operator=(const AnimatedSprite &other) = delete;

        //Move Constructor
        AnimatedSprite(AnimatedSprite &&other) noexcept
            : Sprite(std::move(other)),
              onUpdateHandle(Engine::get().update.replaceListener(other.onUpdateHandle,
                                                                 [&](float _deltaTime) {
                                                                     this->onUpdate(_deltaTime);
                                                                 })),
              animations(std::move(other.animations)),
              currentAnimation(std::move(other.currentAnimation)),
              currentAnimationName(std::move(other.currentAnimationName)),
              currentFrame(other.currentFrame),
              elapsedTime(other.elapsedTime),
              animationQueue(std::move(other.animationQueue)),
              currentCallback(std::move(other.currentCallback)),
              animationFinished(other.animationFinished) {
        }

        //Define Move Operator
        AnimatedSprite &operator=(AnimatedSprite &&other) noexcept {
            if (this != &other) {
                Sprite::operator=(std::move(other));
                animations = std::move(other.animations);
                currentAnimation = std::move(other.currentAnimation);
                currentAnimationName = std::move(other.currentAnimationName);
                currentFrame = other.currentFrame;
                elapsedTime = other.elapsedTime;
                animationQueue = std::move(other.animationQueue);
                currentCallback = std::move(other.currentCallback);
                animationFinished = other.animationFinished;
                onUpdateHandle = Engine::get().update.replaceListener(other.onUpdateHandle,
                                                                     [&](float _deltaTime) {
                                                                         this->onUpdate(_deltaTime);
                                                                     });
            }
            return *this;
        }

        ~AnimatedSprite() override;

        /// Manually add an animation with a name.
        /// @param animationName The name, by which the animation can be played.
        /// @param animation The Animation, which should be player by the name.
        void addAnimation(const std::string &animationName, Animation animation);

        /// Add an animation to the list of enqueued animations, and a function to call at the end of the animation.
        /// @param name The name of the animation, that should be called. Will crash if no such animation exists.
        /// @param onFinished The function to call after the animation finished.
        void enqueueAnimationStep(const std::string &name, std::function<void()> onFinished = nullptr);

        /// Load a single animation from a .json file. Will crash, if the .json has the wrong format.
        /// @param jsonFilePath Path of the file originating from the /assets/ folder.
        void loadAnimation(const std::filesystem::path &jsonFilePath);

        /// Load many animations from a .json file. Will crash, if the .json has the wrong format.
        /// @param jsonFilePath Path of the file originating from the /assets/ folder.
        void loadAnimations(const std::filesystem::path &jsonFilePath);

        /// Stops all animations and clears the animation queue.
        void forceStopAnimation();

    private:
        [[nodiscard]] AnimationFrame getCurrentFrame() const;
        void onUpdate(float deltaTime);
        void play(const std::string &animationName);
        std::list<events::Event<Engine, float>::ListenerWrapper>::iterator onUpdateHandle;
        std::unordered_map<std::string, Animation> animations;
        Animation currentAnimation;
        std::string currentAnimationName;
        unsigned int currentFrame = 0;
        float elapsedTime = 0.0f;
        std::queue<AnimationQueueItem> animationQueue;
        std::function<void()> currentCallback = nullptr;
        bool animationFinished = false;
        std::string getCurrentAnimationName() const;
        glm::ivec2 getAtlasCoords() const;
        void playNextAnimation();
    };
}
