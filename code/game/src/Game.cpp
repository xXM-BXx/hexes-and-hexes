#include "Game.h"

#include <iostream>

#include <engine/rendering/AnimatedSprite.h>
#include <vector>
#include <algorithm> // for std::shuffle
#include <random>    // for std::default_random_engine
#include <chrono>    // for seeding RNG

#include <engine/Assets.h>
#include "expanded-hex-grid/characters/PlayerCharacter.h"
#include "expanded-hex-grid/characters/enemies/BasicEnemy.h"
#include "expanded-hex-grid/characters/enemies/ExplodingEnemy.h"
#include "expanded-hex-grid/characters/enemies/Tier2Enemy.h"
#include "expanded-hex-grid/characters/enemies/Tier3Enemy.h"
#include "ui/game-ui/CompleteUI.h"

namespace hnh {
    engine::HexGrid<game::CustomHexTile> *Game::getHexGrid() {
        return hexGrid;
    }

    void Game::markTiles(const std::vector<HexCoords> &toMark) {
        clearMarkedTiles();
        for (auto position: toMark) {
            auto tile = hexGrid->getTileOrNull(position);
            if (tile)tile->marked = true;
        }
        markedTiles = toMark;
    }

    Game *Game::instance = nullptr;

    std::vector<HexCoords> Game::getMarkedTiles() {
        return markedTiles;
    }

    void Game::animationFinished() {
        currentTurnState = TurnState::AnimationFinished;
    }

    void Game::startGame() {
        currentTurnState = TurnState::StartingGame;
    }

    void Game::pauseGame() {
        currentTurnState = TurnState::Pausing;
    }

    void Game::unpauseGame() {
        currentTurnState = TurnState::Unpausing;
    }

    void Game::toMainMenu() {
        currentTurnState = TurnState::ReturningToMainMenu;
    }

    void Game::setGameOver() {
        currentTurnState = TurnState::ToGameOver;
    }

    game::PlayerCharacter *Game::getPlayerCharacter() {
        //Find Player Character
        auto it = std::find_if(queue.begin(), queue.end(), [](game::Character *c) {
            return dynamic_cast<game::PlayerCharacter *>(c);
        });
        if(it != queue.end()) {
            return dynamic_cast<game::PlayerCharacter *>(*it);
        }
        return nullptr;
    }

    void Game::playCrystalBreakSound() {
        engine::Engine::get().audioManager.play("crystalBreakSound");
    }

    void Game::playCrystalGrowSound() {
        engine::Engine::get().audioManager.play("crystalGrowSound");
    }
    void Game::playHurtSound() {
        engine::Engine::get().audioManager.play("hurtSound");
    }
    void Game::playCastSound(){
        engine::Engine::get().audioManager.play("castSound");
    }
    void Game::playStepSound() {
        engine::Engine::get().audioManager.play("stepSound");
    }
    void Game::playExplosionSound() {
        engine::Engine::get().audioManager.play("explosionSound");
    }
    void Game::playLifeStealSound() {
        engine::Engine::get().audioManager.play("lifestealSound");
    }
    void Game::playHealSound() {
        engine::Engine::get().audioManager.play("healSound");
    }

    void Game::onStart() {
        highScore = loadHighscore();
        defaultFont = engine->getResourceManager()->loadResource<engine::ui::BitmapFont>(
            "sprites/minogram_6x10.png", "json/minogram_6x10.json");
        uiBoxTexture = engine->getResourceManager()->loadResource<engine::ui::UITexture>("sprites/UIBox.png");
        uiButtonHoveredTexture = engine->getResourceManager()->loadResource<engine::ui::UITexture>(
            "sprites/UIButtonHover.png");

        engine::Engine::get().audioManager.loadSound("crystalBreakSound", resolveAssetPath("audio/CrystalBreak.wav"));

        engine::Engine::get().audioManager.loadSound("crystalGrowSound", resolveAssetPath("audio/CrystalGrow.wav"));

        engine::Engine::get().audioManager.loadSound("hurtSound", resolveAssetPath("audio/Hurt.wav"));

        engine::Engine::get().audioManager.loadSound("castSound", resolveAssetPath("audio/Cast.wav"));

        engine::Engine::get().audioManager.loadSound("stepSound", resolveAssetPath("audio/Step.wav"));

        engine::Engine::get().audioManager.loadSound("explosionSound", resolveAssetPath("audio/Explosion.wav"));

        engine::Engine::get().audioManager.loadSound("lifestealSound", resolveAssetPath("audio/Lifesteal.wav"));

        engine::Engine::get().audioManager.loadSound("healSound", resolveAssetPath("audio/Heal.wav"));

        engine::Engine::get().audioManager.loadSound("backgroundMusic", resolveAssetPath("audio/Fantasy Choir 1.wav").string(), true);
        engine::Engine::get().setVolume(0.3f);

        instance = this;

        engine::Engine::get().backgroundColor = glm::vec4(0.11f, 0.039f, 0.008f, 1.0f);

        hexGrid = engine->getObjectManager()->addObject<engine::HexGrid<game::CustomHexTile>>();

        //Add new Atlas Textures
        std::vector<unsigned int> rows = {0, 3, 5, 8, 10, 12, 15, 17, 20, 23};
        auto grassTileAtlas = engine->getResourceManager()->loadResource<engine::AtlasTexture>(
            "sprites/GrassTileSet.png", glm::ivec2(32, 16));
        grassTileAtlas->MakeAtlasItems(rows, 8, glm::ivec2(1, 2), glm::ivec2(16, 24));
        hexGrid->addTexture(0, grassTileAtlas);
        auto dirtPitTileAtlas = engine->getResourceManager()->loadResource<engine::AtlasTexture>(
            "sprites/DirtPitTileSet.png", glm::ivec2(32, 16));
        dirtPitTileAtlas->MakeAtlasItems(rows, 8, glm::ivec2(1, 2), glm::ivec2(16, 8));
        hexGrid->addTexture(1, dirtPitTileAtlas);

        //Add new Terrains
        auto grassTerrain = new engine::AutoTileTerrain(0);
        grassTerrain->loadNeighborRelation("json/terrain-sets/GrassTerrainSet.json");
        hexGrid->terrains.emplace(grassTerrain->terrainId, grassTerrain);
        auto dirtPitTerrain = new engine::AutoTileTerrain(1);
        dirtPitTerrain->loadNeighborRelation("json/terrain-sets/DirtPitTerrainSet.json");
        hexGrid->terrains.emplace(dirtPitTerrain->terrainId, dirtPitTerrain);


        //Create starting Map
        auto startupHexList = HexCoords::floodFill({0, 0}, 17, [](const HexCoords &c) {
            return c.neighborCoords();
        });
        for (int i = 0; i < startupHexList.size(); ++i) {
            if (i < 7) {
                for (auto tile: startupHexList[i]) {
                    hexGrid->addOrOverwriteTile(tile, 0);
                }
            } else {
                for (auto tile: startupHexList[i]) {
                    hexGrid->addOrOverwriteTile(tile, 1);
                }
            }
        }
        hexGrid->scale = 4;
        hexGrid->position = {250, 0};


        //Setup Mouse Marker
        auto mouseTileAtlas = engine->getResourceManager()->loadResource<engine::AtlasTexture>(
            "sprites/MouseMarker.png", glm::ivec2(32, 16));
        mouseTileAtlas->MakeAtlasItem(glm::ivec2(0, 0), glm::ivec2(1, 1));
        mouseHighlight = engine->getObjectManager()->addObject<engine::Sprite>(mouseTileAtlas);
        mouseHighlight->setZIndex(100);
        mouseHighlight->scale = 4;

        gameUI = engine->getObjectManager()->addUIObject<game::CompleteUI>();

        engine::Engine::get().audioManager.play("backgroundMusic");
    }

    std::vector<HexCoords> Game::getWalkableHexes() const {
        auto allTiles = hexGrid->getAllTiles();
        std::vector<HexCoords> result;
        for (auto tile: allTiles) {
            if(tile->isWalkable()) {
                result.push_back(tile->HexPosition);
            }
        }
        return result;
    }

    int Game::loadHighscore() const {
        auto string = engine::ResourceManager::readText(persistentDataFile);
        auto json = nlohmann::json::parse(string);
        int highScore = json["highScore"];
        return highScore;
    }

    void Game::saveHighscore() {
        auto string = engine::ResourceManager::readText(persistentDataFile);
        auto json = nlohmann::json::parse(string);
        json["highScore"] = highScore;
        auto outString = json.dump();
        engine::ResourceManager::writeText(persistentDataFile, outString);
    }

    void Game::onUpdate(float deltaTime) {
        switch (currentTurnState) {
            case TurnState::PlayerTurn:
                decideSpawning();
                queue.front()->markAction(mouseGridPosition);
                currentTurnState = TurnState::WaitingForInput;
                break;
            case TurnState::ProcessingLeftClick:
                // Apply the results of the player's action
                resolvePlayerLeftClick();
                break;
            case TurnState::ProcessingMouseMove:
                resolveMouseMove();
                break;
            case TurnState::EnemyTurn:
                if(paused) break;
                currentTurnState = TurnState::WaitingForAnimationFinish;
                queue.front()->decide();
                break;
            case TurnState::WaitingForInput:
                // Do nothing; wait for input handler to change state
                break;
            case TurnState::WaitingForAnimationFinish:
                //Do nothing. Wait for something to continue the game
                break;
            case TurnState::Waiting:
                //Do nothing. Wait.
                break;
            case TurnState::AnimationFinished:
                progressQueue();
                updateTurnState();
                //Do nothing Wait for something to continue the game
                break;
            case TurnState::ReturningToMainMenu:
                mouseHighlight->visible = false;
                paused = false;
                resetBoard();
                gameUI->setToMainMenu();
                currentTurnState = TurnState::Waiting;
                break;
            case TurnState::MainMenu:
                break;
            case TurnState::StartingGame:
                decideSpawning();
                mouseHighlight->visible = true;
                gameUI->setToPlay();
                updateTurnState();
                break;
            case TurnState::Pausing:
                gameUI->setToPause();
                paused = true;
                break;
            case TurnState::Unpausing:
                gameUI->setToPlay();
                paused = false;
                updateTurnState();
                break;
            case TurnState::ToGameOver:
                removeDefeatedCharacters();
                gameUI->setToGameOver();
                paused = true;
                currentTurnState = TurnState::GameOver;
                break;
            case TurnState::GameOver:
                break;
            default:
                break;
        }
    }

    void Game::decideSpawning() {

        if((timeSinceLastSpawn < spawnInterval) && queue.size() > 1) return;
        //if not beginning of turn
        if(queue.front()->remainingActions != queue.front()->maxActions) return;

        if(queue.size() > 20) return;

        int spawnBudget = std::min((currentTurn + 10) / 3, 50);
        spawnBudget -= (queue.size() - 1);

        auto rings = HexCoords::floodFill({0, 0}, 7, [](HexCoords h) {
            return h.neighborCoords();
        });

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine rng(seed);

        for (int i = rings.size() - 1; i >= 0; --i) {
            std::vector<HexCoords> shuffledRing = rings[i];

            // Randomize the order of tiles in the current ring
            std::shuffle(shuffledRing.begin(), shuffledRing.end(), rng);
            //Find empty tile as far out as possible
            for (const auto& coord : shuffledRing) {
                if (hexGrid->getTileOrNull(coord)->isWalkable() && spawnBudget > 0) {
                    //Try spawning strongest possible enemies
                    if(spawnBudget > game::Tier3Enemy::scoreValue * 2 && currentTurn > 30) {
                        spawnBudget -= game::Tier3Enemy::scoreValue * 2;
                        spawnCharacter<game::Tier3Enemy>(coord);
                    }else if(spawnBudget > game::Tier2Enemy::scoreValue * 5 && currentTurn > 10) {
                        spawnBudget -= game::Tier2Enemy::scoreValue * 2;
                        spawnCharacter<game::Tier2Enemy>(coord);
                    }else if(spawnBudget > game::ExplodingEnemy::scoreValue) {
                        spawnBudget -= game::ExplodingEnemy::scoreValue;
                        spawnCharacter<game::ExplodingEnemy>(coord);
                    }
                }else{
                    timeSinceLastSpawn = 0;
                    return;
                }
            }
        }
        //printf("No tiles available to spawn enemies!");
    }

    void Game::onUnhandledInputEvent(std::unique_ptr<engine::InputEvent> event) {
        if (paused) return;
        if(auto keyEvent = dynamic_cast<engine::KeyEvent *>(event.get())) {
            if(keyEvent->key == GLFW_KEY_BACKSPACE && keyEvent->action == GLFW_PRESS) {
                //This was used to debug
                //getPlayerCharacter()->takeDamage(20);
            }
        }
        if (currentTurnState == TurnState::WaitingForInput) {
            if (auto buttonEvent = dynamic_cast<engine::MouseButtonEvent *>(event.get())) {
                if (buttonEvent->button == GLFW_MOUSE_BUTTON_LEFT && buttonEvent->action == GLFW_PRESS) {
                    currentTurnState = TurnState::ProcessingLeftClick;
                }
            }
            if (auto moveEvent = dynamic_cast<engine::MouseMoveEvent *>(event.get())) {
                glm::vec2 worldPosition = engine->screenToWorld(glm::vec2(moveEvent->xpos, moveEvent->ypos));
                mouseGridPosition = hexGrid->worldToGrid(worldPosition);
                mouseHighlight->position = hexGrid->gridToWorld(mouseGridPosition);
                currentTurnState = TurnState::ProcessingMouseMove;
            }
        }
    }

    void Game::progressQueue() {
        onActionEnd();
        queue.front()->remainingActions--;
        if(queue.front()->remainingActions <= 0) {
            queue.front()->remainingActions = queue.front()->maxActions;
            onTurnEnd();
            queue.splice(queue.end(), queue, queue.begin());
        }
    }

    void Game::onActionEnd() {
        for (game::Character* character: queue) {
            character->checkForSuffocationDamage();
        }
    }

    void Game::onTurnEnd() {
        for (auto tile: hexGrid->getAllTiles()) {
            tile->onTurnEnd();
        }
        timeSinceLastSpawn++;
        currentTurn++;
    }

    void Game::removeDefeatedCharacters() {
        std::vector<game::Character *> deadCharacters;

        std::copy_if(queue.begin(), queue.end(), std::back_inserter(deadCharacters),
                     [](game::Character *c) {
                         return c->getIsDead();
                     });

        for (auto deadCharacter: deadCharacters) {
            score += deadCharacter->scoreValue;
        }

        queue.remove_if([this](game::Character *c) {
            return c->getIsDead();
        });

        for (auto &c: deadCharacters) {
            auto tile = hexGrid->getTileOrNull(c->getHexPosition());
            if (tile)tile->character = nullptr;
        }
    }

    void Game::updateTurnState() {
        removeDefeatedCharacters();
        updateUI();
        auto currentCharacter = queue.front();
        if (dynamic_cast<game::PlayerCharacter *>(currentCharacter)) {
            currentTurnState = TurnState::PlayerTurn;
        } else {
            currentTurnState = TurnState::EnemyTurn;
        }
    }

    void Game::updateUI() {
        gameUI->updateUI();
    }

    void Game::resolvePlayerLeftClick() {
        currentTurnState = TurnState::Waiting;
        auto currentCharacter = queue.front();
        if (currentCharacter->executeAction(mouseGridPosition)) {
            clearMarkedTiles();
            currentTurnState = TurnState::WaitingForAnimationFinish;
        } else {
            currentTurnState = TurnState::WaitingForInput;
        }
    }

    void Game::resolveMouseMove() {
        auto currentCharacter = queue.front();
        currentCharacter->markAction(mouseGridPosition);
        currentTurnState = TurnState::WaitingForInput;
    }

    void Game::clearMarkedTiles() {
        for (auto position: markedTiles) {
            auto tile = hexGrid->getTileOrNull(position);
            if (tile)tile->marked = false;
        }
        markedTiles.clear();
    }


    void Game::resetBoard() {
        if (score > highScore) {
            highScore = score;
            saveHighscore();
        }
        score = 0;
        for (auto tile: hexGrid->getAllTiles()) {
            tile->marked = false;
            tile->getObstacle()->setObstacleType(game::Obstacle::Empty);
            tile->character = nullptr;
        }
        timeSinceLastSpawn = spawnInterval;
        currentTurn = 0;
        queue.clear();
        spawnCharacter<game::PlayerCharacter>({0, 0});
    }
}
