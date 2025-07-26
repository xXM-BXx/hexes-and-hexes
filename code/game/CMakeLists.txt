cmake_minimum_required(VERSION 3.28.1)
project(Game)

# set variables for source files
set(SOURCE_FILES
        src/main.cpp
        src/Game.cpp
        src/Game.h

        src/expanded-hex-grid/hex-tile/CustomHexTile.cpp
        src/expanded-hex-grid/hex-tile/CustomHexTile.h
        src/expanded-hex-grid/characters/Character.h
        src/expanded-hex-grid/characters/Character.cpp
        src/expanded-hex-grid/characters/actions/Action.cpp
        src/expanded-hex-grid/characters/actions/Action.h
        src/expanded-hex-grid/characters/PlayerCharacter.cpp
        src/expanded-hex-grid/characters/PlayerCharacter.h
        src/expanded-hex-grid/characters/enemies/BasicEnemy.cpp
        src/expanded-hex-grid/characters/enemies/BasicEnemy.h
        src/expanded-hex-grid/characters/actions/MoveAction.cpp
        src/expanded-hex-grid/characters/actions/MoveAction.h
        src/expanded-hex-grid/characters/actions/LineAttack.cpp
        src/expanded-hex-grid/characters/actions/LineAttack.h
        src/expanded-hex-grid/hex-tile/Obstacle.cpp
        src/expanded-hex-grid/hex-tile/Obstacle.h
        src/ui/game-ui/CompleteUI.cpp
        src/ui/game-ui/CompleteUI.h
        src/ui/game-ui/ActionList.cpp
        src/ui/game-ui/ActionList.h
        src/ui/game-ui/ActionListItem.cpp
        src/ui/game-ui/ActionListItem.h
        src/ui/game-ui/QueueVisualization.cpp
        src/ui/game-ui/QueueVisualization.h
        src/ui/game-ui/QueueVisualizationItem.cpp
        src/ui/game-ui/QueueVisualizationItem.h
        src/ui/game-ui/PlayerHealthVisualization.cpp
        src/ui/game-ui/PlayerHealthVisualization.h
        src/ui/game-ui/MainMenu.cpp
        src/ui/game-ui/MainMenu.h
        src/ui/game-ui/PauseButton.cpp
        src/ui/game-ui/PauseButton.h
        src/ui/game-ui/PauseMenu.cpp
        src/ui/game-ui/PauseMenu.h
        src/ui/game-ui/GameOverMenu.cpp
        src/ui/game-ui/GameOverMenu.h
        src/expanded-hex-grid/characters/actions/WallAction.cpp
        src/expanded-hex-grid/characters/actions/WallAction.h
        src/expanded-hex-grid/characters/actions/SingleAttack.cpp
        src/expanded-hex-grid/characters/actions/SingleAttack.h
        src/expanded-hex-grid/characters/actions/Explode.cpp
        src/expanded-hex-grid/characters/actions/Explode.h
        src/expanded-hex-grid/characters/enemies/ExplodingEnemy.cpp
        src/expanded-hex-grid/characters/enemies/ExplodingEnemy.h
        src/expanded-hex-grid/characters/actions/Lifesteal.cpp
        src/expanded-hex-grid/characters/actions/Lifesteal.h
        src/expanded-hex-grid/characters/actions/PlayerAoE.cpp
        src/expanded-hex-grid/characters/actions/PlayerAoE.h
        src/expanded-hex-grid/characters/enemies/Tier2Enemy.cpp
        src/expanded-hex-grid/characters/enemies/Tier2Enemy.h
        src/expanded-hex-grid/characters/enemies/Tier3Enemy.cpp
        src/expanded-hex-grid/characters/enemies/Tier3Enemy.h
        src/expanded-hex-grid/characters/actions/Tier2EnemyAttack.cpp
        src/expanded-hex-grid/characters/actions/Tier2EnemyAttack.h
        src/expanded-hex-grid/characters/actions/Tier3EnemySingle.cpp
        src/expanded-hex-grid/characters/actions/Tier3EnemySingle.h
        src/expanded-hex-grid/characters/actions/Tier3EnemyAoE.cpp
        src/expanded-hex-grid/characters/actions/Tier3EnemyAoE.h
        src/expanded-hex-grid/characters/actions/PlayerSmallAoE.cpp
        src/expanded-hex-grid/characters/actions/PlayerSmallAoE.h
)

# set executable name
set(EXE_FILE HexesAndHexes)

# add the executable target
add_executable(${EXE_FILE} ${SOURCE_FILES})

set(JSON_BuildTests OFF CACHE INTERNAL "")

# require C++ 20 compiler
target_compile_features(${EXE_FILE} PRIVATE cxx_std_20)

# link with dependencies
target_link_libraries(${EXE_FILE} PRIVATE Engine)

#Asset loading
target_compile_definitions(${EXE_FILE} PRIVATE ASSET_ROOT=./assets/)

# copy assets to output directory before build
add_custom_command(TARGET ${EXE_FILE} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${EXE_FILE}>/assets)