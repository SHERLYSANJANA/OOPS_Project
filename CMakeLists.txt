cmake_minimum_required(VERSION 3.12)
project(My2DGame CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#  header files
include_directories(sokol)
include_directories(engine/core)
include_directories(engine/graphics)
include_directories(engine/physics)

# Automatically find and include all your source files
# Use GLOB_RECURSE with CONFIGURE_DEPENDS so CMake regenerates when files change.
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/main.cpp"
    "${CMAKE_SOURCE_DIR}/engine/*.cpp"
    "${CMAKE_SOURCE_DIR}/engine/*/*.cpp"
)

message(STATUS "SOURCES = ${SOURCES}")

add_executable(My2DGame ${SOURCES})

# Link required platform graphics libraries
if(WIN32)
    # Windows links automatically via Sokol pragmas
elseif(APPLE)
    target_link_libraries(My2DGame PRIVATE "-framework Metal" "-framework Cocoa" "-framework QuartzCore")
elseif(UNIX)
    target_link_libraries(My2DGame PRIVATE GL X11 m dl)
endif()
