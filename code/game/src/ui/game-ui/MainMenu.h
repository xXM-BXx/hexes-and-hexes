//
// Created by mburc on 15.07.2025.
//

#pragma once

#include <engine/ui/UIObject.h>
#include <engine/ui/text/UIText.h>

#include <engine/rendering/Sprite.h>
#include <engine/ui/UIButton.h>

namespace hnh::game {

    ///Has buttons to start and end the game, and a button to toggle the visibility of the tutorial box and text.
    class MainMenu : public engine::ui::UIObject {
    public:
        MainMenu();

        void toggleTutorial();
        void updateUI();

        void draw(glm::mat3 &parentTransform) const override;

    private:
        engine::Sprite titleCard;
        engine::ui::UIBox tutorialBox;
        engine::ui::UIText highscoreText;
        bool showTutorial = false;
        int buttonWidth = 128;
        const std::u32string tutorialTextContent = U"This is a Highscore Game, so your goal is to defeat as many\n"
                                                   "Enemies as possible.\n"
                                                   "To do so you can use a variety of different Attacks, which can be selected across several pages on the left side.\n"
                                                   "They have differing cooldowns, which are measured in Activa-\n"
                                                   "tions of Characters, so not complete rotations through all Characters.\n"
                                                   "A description of an Action is shown when hovering the button.\n"
                                                   "After selecting an Action you can see the affected tiles, when moving your mouse to a target.\n"
                                                   "You can complete 2 Actions, before your Activation ends.\n"
                                                   "Your HP and current Score can be seen in the top left.\n"
                                                   "Enemies come in several variations, and have a vastly reduced\n"
                                                   "and weakened selection of actions.\n"
                                                   "They have only few HP, but you will need to track your dealt\n"
                                                   "damage yourself, if you did not defeat them in one hit.\n"
                                                   "Enemies will respawn somewhat randomly at the outside of the\n"
                                                   "game field.\n"
                                                   "Some attacks may require line of sight. Other Characters do\n"
                                                   "not block this, but some obstacles, e.g. the Crystals created\n"
                                                   "by some attacks do provide Cover.";

    };
}
