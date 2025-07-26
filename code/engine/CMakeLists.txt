cmake_minimum_required(VERSION 3.28.1)
project(Engine)

file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS "include/**.h")
file(GLOB_RECURSE PRIVATE_HEADER_LIST CONFIGURE_DEPENDS "src/**.h")
file(GLOB_RECURSE SOURCE_LIST CONFIGURE_DEPENDS "src/**.cpp")

set(ENGINE_NAME Engine)

add_subdirectory(extern/glm)
add_subdirectory(extern/glad)
add_subdirectory(extern/glfw)
add_subdirectory(extern/nlohmann_json)
add_subdirectory(extern/soloud)
add_subdirectory(extern/stb_image)
add_subdirectory(extern/miniaudio)

add_library(${ENGINE_NAME} ${HEADER_LIST} ${PRIVATE_HEADER_LIST} ${SOURCE_LIST}
        extern/miniaudio/include/miniaudio/miniaudio.h
        extern/miniaudio/src/miniaudio.cpp
)

target_compile_features(${ENGINE_NAME} PRIVATE cxx_std_20)
set_target_properties(${ENGINE_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${ENGINE_NAME} PUBLIC glad glfw glm nlohmann_json soloud stb_image miniaudio)

target_include_directories(${ENGINE_NAME} PUBLIC
        include/
)