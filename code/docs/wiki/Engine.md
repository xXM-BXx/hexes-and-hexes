# How to make a game

Also see [formatting .json files](Json.md)
## My Engine
My Engine has functionality to render sprites and animated sprites from atlas textures. It has a tile map with an auto-tiling feature.
It can also render and manage Ui components and their inputs. It also contains an Audio Manager with SoLoud.
It however has no physics processing. This makes it best suited for e.g. 2D Turn Based Games, that can work around those restrictions.

But without a GUI my engine is far clunkier than a "proper" engine, and should really only be used out of curiosity.
## How to make a game

### Setting up the project
You will need this structure:
```
ProjectRoot/
    | assets              This folder is accessed by all loading functions.
    |-- engine/             Copy the whole engine folder there.
    |-- game/               The games' structure is somewhat optional, but I will explain
    |   |-- src/            it like this
    |   |   |-- Game.h     
    |   |   |-- Game.cpp 
    |   |   |-- main.cpp  
    |   |-- CMakeLists.txt
    |-- CMakeLists.txt      
```
The root CMakeList will need to contain:
```CMake
cmake_minimum_required(VERSION 3.28.1)
project(code)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(engine)

add_subdirectory(game)      #Or whatever your game CMakeList.txt is called.
```
And your ```game/CMakeLists.txt``` should look something like this:
```CMake
cmake_minimum_required(VERSION 3.28.1)
project(Game)

# set variables for source files
set(SOURCE_FILES
        src/main.cpp
        src/Game.cpp
        src/Game.h
)

# set executable name
set(EXE_FILE YourExeName)   #Change this name

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
```
Your ```Game.h``` should contain this to automatically link the update event etc.:
```C++
class Game : public gl3::engine::Game{
    using gl3::engine::Game::Game;
}
```
And lastly your ```main.cpp``` should look like this:
```C++
#define GET_STRING(x) #x
#define GET_DIR(x) GET_STRING(x)

///Create a Engine and a game, the run the engine.
int main() {
    try {
        gl3::engine::Engine& engine = gl3::engine::Engine::get();
        engine.init(1920, 1080, "Your Window Name"); //Change this name
        gl3::Game game = gl3::Game(&engine);
        engine.run();
    }
    catch(const std::exception &e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
    }
}
```

### Creating a game
In your ```Game.h``` you will most likely first want to override the Engine Events:
```C++
void onStart() override;
void onUpdate(float deltaTime) override;
void onUnhandledInputEvent(std::unique_ptr<engine::InputEvent> event) override;
```
In ```onUnhandledInputEvent(std::unique_ptr<engine::InputEvent> event)``` you can compare the events' values with GLFW constants.

In ```onStart()``` you could then create an Object like:
```C++
auto newObject = engine::Engine::get()->getObjectManager()->addObject<engine::GameObject>();
```
When using ```engine::HexGrid<HexTileType>``` you can add your own ```game::CustomHexTile``` by extending ```gl3::engine::HexTile```.

You can generally extend ```gl3::engine::GameObject``` and ```gl3::engine::ui::UIObject``` but you need to remember to override
``draw()`` and ``checkForUiInput()``. And you will always need at least one top level object registered with the object manager to be in the game loop.

For more information refer to the API-Documentation.