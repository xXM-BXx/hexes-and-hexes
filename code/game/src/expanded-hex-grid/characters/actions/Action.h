//
// Created by mburc on 06.07.2025.
//
#pragma once
#include <string>
#include <engine/tilemap/HexCoords.h>


namespace hnh::game {
    class Character;
    class Action {
    protected:
        ///The character, that owns this attack. Needed to get the start position for many actions.
        Character *owner;
    public:
        ///Create the action with the provided owner.
        explicit Action(Character* owner);
        virtual ~Action()= default;

        ///The cooldown, before the action can be used again.
        unsigned int currentCooldown = 0;
        ///The cooldown to set, after completing an actions.
        unsigned int maxCooldown = 1;
        ///The name to display in the actions selection ui.
        std::string displayName = "Basic Name";
        ///The text to display in the actions selection tooltip.
        std::string displayDescription = "Does some amount of damage in some area with some cooldown";

        ///Preview, which tiles can be affected by an action. Also return this result.
        virtual std::vector<HexCoords> markTiles(HexCoords target) { return {}; };
        ///Execute the action and return if it was successfully, e.g. because the cooldown was not 0.
        virtual bool execute(HexCoords target) {return false;};
        ///Called by the owner. Decreases the cooldown.
        void onTurnEnd();

    };

}
