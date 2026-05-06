cmake_minimum_required(VERSION 3.15)

project("DigDug")

#################################
# Dependencies
#################################

include(FetchContent)

#################################
# glm
#################################
find_package(glm CONFIG QUIET)
if(NOT glm_FOUND)
    # Forcing glm to c
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)  
  FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/releases/download/1.0.3/glm-1.0.3.zip
    DOWNLOAD_NO_PROGRESS ON
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE# Avoiding CMP0135
  )
  FetchContent_MakeAvailable(glm)
endif()

#################################
# ImGui
#################################
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.6
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${sdl3_SOURCE_DIR}/include)
target_link_libraries(imgui PUBLIC SDL3::SDL3)

# enable c++23 features
target_compile_features(imgui PUBLIC cxx_std_23)

#################################
# Implot
#################################
FetchContent_Declare(
    implot
    GIT_REPOSITORY https://github.com/epezent/implot.git
    GIT_TAG v0.17
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(implot)

add_library(implot STATIC
        ${implot_SOURCE_DIR}/implot.cpp
        ${implot_SOURCE_DIR}/implot_items.cpp
        ${implot_SOURCE_DIR}/implot_demo.cpp
)

target_link_libraries(implot PUBLIC imgui)

#################################
# SDL3
#################################
find_package(SDL3 3.4 CONFIG QUIET)
if(NOT SDL3_FOUND)
  if (WIN32)
    FetchContent_Declare(
      SDL3
      URL https://www.libsdl.org/release/SDL3-devel-3.4.0-VC.zip
      DOWNLOAD_NO_PROGRESS ON
      DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )
    FetchContent_MakeAvailable(SDL3)
    list(PREPEND CMAKE_PREFIX_PATH "${sdl3_SOURCE_DIR}")
    find_package(SDL3 CONFIG REQUIRED)
  else()
    FetchContent_Declare(
      SDL3
      GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
      GIT_TAG release-3.4.0
      GIT_SHALLOW TRUE
      GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(SDL3)
  endif()
endif()

#################################
# SDL3_ttf
#################################
find_package(SDL3_ttf 3.2.2 CONFIG QUIET)
if(NOT SDL3_ttf_FOUND)
  if (WIN32)
    FetchContent_Declare(
      SDL3_ttf
      URL https://www.libsdl.org/projects/SDL_ttf/release/SDL3_ttf-devel-3.2.2-VC.zip
      DOWNLOAD_NO_PROGRESS ON
      DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )
    FetchContent_MakeAvailable(SDL3_ttf)
    list(PREPEND CMAKE_PREFIX_PATH "${sdl3_ttf_SOURCE_DIR}")
    find_package(SDL3_ttf CONFIG REQUIRED)
  else()
    FetchContent_Declare(
      SDL3_ttf
      GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
      GIT_TAG release-3.2.2
      GIT_SHALLOW TRUE
      GIT_PROGRESS TRUE
    )
    set(SDL3TTF_INSTALL OFF)
    if(EMSCRIPTEN)
      list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/emscripten")
      set(SDL3TTF_USE_SYSTEM_FREETYPE ON)
      set(SDL3TTF_VENDORED OFF)
      set(SDLTTF_PLUTOSVG OFF)
      add_compile_options(-sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1)
      add_link_options(-sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1)
    endif()
    FetchContent_MakeAvailable(SDL3_ttf)
  endif()
endif()

#################################
# SDL3_mixer
#################################
FetchContent_Declare(
    SDL3_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer
    GIT_TAG release-3.2.0
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(SDL3_mixer)

#################################
# SDL3_image
#################################
# NOTE: Windows build cannot find a dependency required for AVIF support
set(ENABLE_AVIF OFF CACHE BOOL "Disable AVIF support" FORCE)
set(SDL3_IMAGE_AVIF OFF CACHE BOOL "Disable AVIF support" FORCE)
FetchContent_Declare(
    SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
    GIT_TAG release-3.4.4
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(SDL3_image)

#################################
# VLD
#################################
if (WIN32)
  # Add VLD (Visual Leak Detector) for memory leak detection on Windows
  find_package(VLD CONFIG)
endif()

if(POLICY CMP0144)
  cmake_policy(SET CMP0144 NEW)
endif()

#################################
# Steam
#################################
list(APPEND CMAKE_MODULE_PATH "../cmake/steam")
find_package(Steamworks QUIET)


if(EMSCRIPTEN)
    message("Using Emscripten")
endif()

if(EMSCRIPTEN)
    set(GAME_NAME ${PROJECT_NAME}_web)
else()
    set(GAME_NAME ${PROJECT_NAME})
endif()

#################################
# Targets
#################################
# Common dependencies
set(COMMON_DEPENDENCIES "CommonDependencies")
add_library(${COMMON_DEPENDENCIES} INTERFACE)

set(SDL_RUNTIME_DLLS
      SDL3::SDL3
      SDL3_ttf::SDL3_ttf
      SDL3_mixer::SDL3_mixer
      SDL3_image::SDL3_image
 )

# TODO: Do not make everything public
target_link_libraries(${COMMON_DEPENDENCIES} INTERFACE
  ${SDL_RUNTIME_DLLS}
  glm
  imgui
  implot
  ${VLD_LIBRARIES}
)

# Engine
set(ENGINE_NAME "Engine")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/${ENGINE_NAME}")

target_include_directories(${ENGINE_NAME} PUBLIC
  "${CMAKE_SOURCE_DIR}/DigDug/"
  ${implot_SOURCE_DIR}
  ${imgui_SOURCE_DIR}
  ${VLD_INCLUDE_DIR}
)

target_link_libraries(${ENGINE_NAME} PUBLIC ${COMMON_DEPENDENCIES})

# Game
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Game")

