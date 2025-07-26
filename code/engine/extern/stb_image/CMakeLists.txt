add_library(stb_image STATIC
        src/stb_image.cpp
)

# Set include path so you can do #include <stb_image/stb_image.h>
target_include_directories(stb_image PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)