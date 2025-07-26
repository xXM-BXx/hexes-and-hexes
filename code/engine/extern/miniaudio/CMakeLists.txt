add_library(miniaudio STATIC
        src/miniaudio.cpp
)

# Set include path so you can do #include <miniaudio/miniaudio.h>
target_include_directories(miniaudio PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)