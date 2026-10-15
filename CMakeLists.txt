cmake_minimum_required(VERSION 3.12)
project(My2DGame)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(My2DGame
    main.cpp
    engine/core/sokol_implement.cpp
    engine/core/Application.cpp
    engine/core/Input.cpp
    engine/graphics/Renderer.cpp
)

target_include_directories(My2DGame PRIVATE 
    engine/core
    engine/graphics
    engine/physics
    sokol
)
if(WIN32)
    target_compile_definitions(My2DGame PRIVATE SOKOL_GLCORE SOKOL_WIN32_FORCE_MAIN)
    target_link_libraries(My2DGame PRIVATE opengl32 gdi32 ole32)
endif()
