cmake_minimum_required(VERSION 3.15)

project("DigDug")

# NOTE: This must be the exact order, because Game depends on Engine

set(ENGINE_NAME "Engine")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Engine")

if(EMSCRIPTEN)
    message("Using Emscripten")
endif()

if(EMSCRIPTEN)
    set(GAME_NAME ${PROJECT_NAME}_web)
else()
    set(GAME_NAME ${PROJECT_NAME})
endif()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Game")# Uses PROJECT_NAME
