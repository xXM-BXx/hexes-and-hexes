#pragma once
#include <engine/rendering/GameObject.h>

#include <engine/rendering/AnimatedSprite.h>

#include <engine/tilemap/HexCoords.h>
#include "actions/Action.h"

namespace hnh::game {
    ///An abstract class, which provides some shared functionality needed by all characters.
    class Character : public engine::GameObject {
    public:
        explicit Character(std::shared_ptr<engine::AtlasTexture> atlasTexture);

        virtual void decide();

        [[nodiscard]] int getHealth() const;
        ///Wrapper for the health, that can kill the character.
        virtual bool takeDamage(int damage);

        ///Pushes characters out of terrain, needed for the crystal actions.
        void checkForSuffocationDamage();

        ///Move to the target along an automatically generated path.
        void move(HexCoords targetPosition);
        ///Change the position of a character.
        void teleport(HexCoords targetPosition);
        ///Play the attack animation, before executing a callback.
        void animateSpell(std::function<void()> effect);

        ///Set the starting position once.
        void initHexPosition(HexCoords newPosition);

        ///Change the characters positions, but also move the character to belong to a new tile.
        HexCoords changeHexPosition(HexCoords newPosition);

        [[nodiscard]] HexCoords getHexPosition() const;

        engine::AnimatedSprite *getCharacterSprite();

        ///Draw the sprite.
        void draw(glm::mat3 &parentTransform) const override;

        ///Execute the currently selected \ref Action.
        bool executeAction(HexCoords target);

        ///Mark the currently selected \ref Action.
        void markAction(HexCoords target);

        [[nodiscard]] glm::mat3 getTransform() const;
        void setSelectedAction(const std::string &actionName);
        [[nodiscard]] bool getIsDead() const;
        ///Is called by the tile owning the character, and updates cooldowns of all actions.
        void onTurnEnd();

        ///Maps strings to \ref Action s because of convenience.
        std::unordered_map<std::string, std::unique_ptr<Action>> actions;
        ///How many points for the final score the character is worth if defeated.
        static const unsigned int scoreValue = 1;

        int remainingActions = 1;
        ///How many actions the character can execute, before their activation ends.
        int maxActions = 1;
    protected:
        bool isDead = false;
        std::string selectedAction;
        int health = 1;
        int maxHealth = 1;


    private:
        HexCoords hexPosition = {0, 0};

        engine::AnimatedSprite characterSprite;

        static std::string getAnimationNameForDirection(HexCoords from, HexCoords to);
    };
}
