###########################################################################
# Library creation
###########################################################################
set(SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Source")
set(HEADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Include")

# NOTE: I am aware that it is discouraged to use GLOB to list source files,
# but reconfiguring CMake after adding a new file is faster than
# manually adding a path to every new source file.
file(GLOB_RECURSE ENGINE_SOURCES "${SOURCE_PATH}/*.cpp")
# Headers for VS to display in Solution Explorer
file(GLOB_RECURSE ENGINE_HEADERS "${HEADER_PATH}/*.h")

add_library(${ENGINE_NAME} ${ENGINE_SOURCES} ${ENGINE_HEADERS})

target_include_directories(${ENGINE_NAME}
        PUBLIC ${HEADER_PATH}# For the games to use
        PRIVATE ${HEADER_PATH}/${ENGINE_NAME}# For the internal use
)

target_compile_features(${ENGINE_NAME} PUBLIC cxx_std_23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

if(MSVC OR CLANG OR GCC)
  # compile at warning level 4 + treat warnings as errors
  target_compile_options(${ENGINE_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${ENGINE_NAME} PRIVATE -Wall -Wextra -Werror)
endif()

###########################################################################
# Dependencies
###########################################################################

include(FetchContent)

#################################
# glm
#################################
find_package(glm CONFIG QUIET)
if(NOT glm_FOUND)
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
      list(APPEND CMAKE_MODULE_PATH "../cmake/emscripten")
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

#################################
# Dependency setup
#################################
target_include_directories("${ENGINE_NAME}" PUBLIC
  "${CMAKE_SOURCE_DIR}/DigDug/"
  ${implot_SOURCE_DIR}
  ${imgui_SOURCE_DIR}
  ${VLD_INCLUDE_DIR}
)

target_link_libraries("${ENGINE_NAME}"
  PRIVATE
  SDL3::SDL3
  SDL3_ttf::SDL3_ttf
  PUBLIC
  glm::glm
  imgui
  implot
  ${VLD_LIBRARIES}
)
