//
// Created by mburc on 17.11.2024.
//

#pragma once

#include <engine/game/Game.h>

#include <engine/tilemap/HexGrid.h>

#include "expanded-hex-grid/characters/PlayerCharacter.h"
#include "expanded-hex-grid/hex-tile/CustomHexTile.h"
#include "ui/game-ui/CompleteUI.h"

namespace gl3 {
    class Game : public engine::Game {
        ///Turn states for the state machine, that runs the game.
        enum class TurnState {
            PlayerTurn,
            EnemyTurn,
            ProcessingLeftClick,
            ProcessingMouseMove,
            WaitingForInput,
            WaitingForAnimationFinish,
            AnimationFinished,
            Waiting,
            ReturningToMainMenu,
            MainMenu,
            StartingGame,
            Pausing,
            Paused,
            Unpausing,
            ToGameOver,
            GameOver
        };

    public:
        ///This uses the Constructor defined by the engine.
        using engine::Game::Game;
        ///A globally accessible instance of the game.
        static Game *instance;

        engine::HexGrid<game::CustomHexTile> *getHexGrid();

        ///Set several tiles by position to be marked.
        void markTiles(const std::vector<HexCoords> &toMark);


        std::vector<HexCoords> getMarkedTiles();

        /// A Callback, that all animations are finished and the game logic may continue.
        void animationFinished();
        void startGame();
        void pauseGame();

        void unpauseGame();

        void toMainMenu();

        void setGameOver();

        ///Will get a pointer to the player, or a nullptr, if no player is found.
        ///Users of this function check if a player was found.
        game::PlayerCharacter* getPlayerCharacter();

        void playCrystalBreakSound();
        void playCrystalGrowSound();
        void playHurtSound();
        void playCastSound();
        void playStepSound();
        void playExplosionSound();
        void playLifeStealSound();
        void playHealSound();

        std::shared_ptr<engine::ui::BitmapFont> defaultFont;
        std::shared_ptr<engine::ui::UITexture> uiBoxTexture;
        std::shared_ptr<engine::ui::UITexture> uiButtonHoveredTexture;

        [[nodiscard]] unsigned int getScore() const { return score; }
        std::list<game::Character *> queue = {};

        ///This is the red marker following the mouse.
        engine::Sprite *mouseHighlight;
        HexCoords mouseGridPosition = HexCoords(0, 0);
        int highScore = 0;
        int currentTurn;

        std::vector<HexCoords> getWalkableHexes() const;
    private:
        int timeSinceLastSpawn = 5;
        int spawnInterval = 5;

        SoLoud::Wav crystalBreakSound;
        SoLoud::Wav crystalGrowSound;
        SoLoud::Wav hurtSound;
        SoLoud::Wav castSound;
        SoLoud::Wav stepSound;
        SoLoud::Wav explosionSound;
        SoLoud::Wav backgroundMusic;
        SoLoud::Wav healSound;
        SoLoud::Wav lifestealSound;
        const std::string persistentDataFile = "json/PersistentData.json";
        bool paused = false;



        void onStart() override;


        int score = 0;

        int loadHighscore() const;
        void saveHighscore();

        template<typename CharacterType>
        void spawnCharacter(HexCoords position) {
            static_assert((std::is_base_of_v<game::Character, CharacterType> && !std::is_same_v<CharacterType, game::Character>), "CharacterType must be derived from game::Character");
            auto character = std::make_unique<CharacterType>();
            character->getCharacterSprite()->loadAnimations("json/animations/CharacterAnimations.json");
            character->getCharacterSprite()->enqueueAnimationStep("idle");
            character->initHexPosition(position);
            auto tile = hexGrid->getTileOrNull(position);
            auto ptr = tile->addCharacter(std::move(character));
            queue.push_back(ptr);
            updateUI();
        }

        void resolveMouseMove();

        void onUpdate(float deltaTime) override;

        void decideSpawning();

        void onUnhandledInputEvent(std::unique_ptr<engine::InputEvent> event) override;

        void progressQueue();

        void onActionEnd();

        void onTurnEnd();

        void removeDefeatedCharacters();

        void updateTurnState();

        void updateUI();

        void resolvePlayerLeftClick();

        void clearMarkedTiles();

        void resetBoard();

        engine::HexGrid<game::CustomHexTile> *hexGrid;
        game::CompleteUI* gameUI;
        TurnState currentTurnState = TurnState::ReturningToMainMenu;
        std::vector<HexCoords> markedTiles;
    };
}
