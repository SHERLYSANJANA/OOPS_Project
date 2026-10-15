cmake_minimum_required(VERSION 3.15)
project(My2DGame)

# 1. Enable C++17 (fixes make_unique as well!)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Gather your source files
file(GLOB_RECURSE SOURCES 
    "main.cpp"
    "engine/core/*.cpp"
    "engine/graphics/*.cpp"
    "engine/physics/*.cpp"
    "game/*.cpp"
)

# 3. Create the executable
add_executable(My2DGame ${SOURCES})

# 4. TELL THE COMPILER WHERE THE HEADERS ARE (This fixes your error!)
target_include_directories(My2DGame PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/sokol
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/core
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/graphics
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/physics
    ${CMAKE_CURRENT_SOURCE_DIR}/game
)

# 5. Compiler definitions for Sokol on Windows
target_compile_definitions(My2DGame PRIVATE
    SOKOL_GLCORE
    SOKOL_WIN32_FORCE_MAIN
    _WIN32_WINNT=0x0601
)

# 6. Link necessary Windows/OpenGL libraries
target_link_libraries(My2DGame PRIVATE
    opengl32
    gdi32
)