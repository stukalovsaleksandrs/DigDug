###########################
# Executable creation
###########################
set(SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Source")
set(HEADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Include")
file(GLOB_RECURSE GAME_SOURCES "${SOURCE_PATH}/*.cpp")
file(GLOB_RECURSE GAME_HEADERS "${HEADER_PATH}/*.h")

# Parameters
if(EMSCRIPTEN)
    set(GAME_PARAM)
else()
    set(GAME_PARAM WIN32
            Include/Application.h)
endif()

add_executable(${GAME_NAME} ${GAME_PARAM} ${GAME_SOURCES} ${GAME_HEADERS})

set(RESOURCE_DIR_NAME "Resources")

target_include_directories(${GAME_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Include")

##########################
# Emscripten
##########################
if(EMSCRIPTEN)
    set(EMSCRIPTEN_COMPILE_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -g")
    set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1 -g -sALLOW_MEMORY_GROWTH --preload-file \"${CMAKE_CURRENT_SOURCE_DIR}/${RESOURCE_DIR_NAME}@/\"")

    # Use the Emscripten toolchain
    set(CMAKE_TOOLCHAIN_FILE ../cmake/EmscriptenToolchain.cmake)

    # Additional settings for Emscripten build
    set_target_properties(${GAME_NAME} PROPERTIES
            COMPILE_FLAGS "${EMSCRIPTEN_COMPILE_FLAGS}"
            LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS}"
    )

    add_custom_command(TARGET ${GAME_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_directory "../../web" "$<TARGET_FILE_DIR:${GAME_NAME}>/"
    )

    # Have emscripten generate a html page too.
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

elseif(WIN32)
    # Set minigin as the default startup project
    set_property(DIRECTORY .. PROPERTY VS_STARTUP_PROJECT ${GAME_NAME})
    # Set working directory to output folder
    set_target_properties(${GAME_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${GAME_NAME}>")

      if(Steamworks_FOUND)
        message("Steamworks found")
        target_link_libraries("${GAME_NAME}" PUBLIC Steamworks::Steamworks)
        target_compile_definitions(${GAME_NAME} PRIVATE USE_STEAMWORKS)
        add_custom_command(TARGET ${GAME_NAME} POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy "${Steamworks_RUNTIME_DLL}" "$<TARGET_FILE_DIR:${GAME_NAME}>")
        add_custom_command(TARGET ${GAME_NAME} POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E echo "480" > "$<TARGET_FILE_DIR:${GAME_NAME}>/steam_appid.txt"
                COMMENT "Generating steam_appid.txt"
        )
    else()
        message("Steamworks not found")
    endif()

    # Copying resources to the binary folder
    add_custom_command(TARGET ${GAME_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${RESOURCE_DIR_NAME}" "$<TARGET_FILE_DIR:${GAME_NAME}>/${RESOURCE_DIR_NAME}"
    )

elseif(APPLE)
    set_target_properties(${GAME_NAME} PROPERTIES
            MACOSX_BUNDLE TRUE
            INSTALL_RPATH "@executable_path/../Frameworks"
            BUILD_WITH_INSTALL_RPATH TRUE
    )
    add_custom_command(TARGET ${GAME_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E make_directory "$<TARGET_BUNDLE_DIR:${GAME_NAME}>/Contents/Frameworks"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3::SDL3-shared>" "$<TARGET_BUNDLE_DIR:${GAME_NAME}>/Contents/Frameworks/"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3_ttf::SDL3_ttf-shared>" "$<TARGET_BUNDLE_DIR:${GAME_NAME}>/Contents/Frameworks/"
            COMMAND install_name_tool -change "@rpath/libSDL3_ttf.0.dylib" "@executable_path/../Frameworks/libSDL3_ttf.0.2.2.dylib" "$<TARGET_FILE:${GAME_NAME}>"
            COMMAND install_name_tool -change "@rpath/libSDL3.0.dylib" "@executable_path/../Frameworks/libSDL3.0.dylib" "$<TARGET_FILE:${GAME_NAME}>"
    )

    if(Steamworks_FOUND)
        target_link_libraries("${GAME_NAME}" PUBLIC Steamworks::Steamworks)
        target_compile_definitions(${GAME_NAME} PRIVATE USE_STEAMWORKS)
        add_custom_command(TARGET ${GAME_NAME} POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${Steamworks_LIBRARIES}" "$<TARGET_BUNDLE_DIR:${GAME_NAME}>/Contents/Frameworks/"
        )
        add_custom_command(TARGET ${GAME_NAME} POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E echo "480" > "$<TARGET_FILE_DIR:${GAME_NAME}>/steam_appid.txt"
                COMMENT "Generating steam_appid.txt"
        )
    endif()

    # Copy Data folder to output directory after build
    add_custom_command(TARGET ${GAME_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${RESOURCE_DIR_NAME}" "$<TARGET_FILE_DIR:${GAME_NAME}>/${RESOURCE_DIR_NAME}"
    )

else()
    if(Steamworks_FOUND)
        target_link_libraries("${GAME_NAME}" PUBLIC Steamworks::Steamworks)
        target_compile_definitions(${GAME_NAME} PRIVATE USE_STEAMWORKS)
        add_custom_command(TARGET ${GAME_NAME} POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${Steamworks_LIBRARIES}" "$<TARGET_FILE_DIR:${GAME_NAME}>"
        )
        add_custom_command(TARGET ${GAME_NAME} POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E echo "480" > "$<TARGET_FILE_DIR:${GAME_NAME}>/steam_appid.txt"
                COMMENT "Generating steam_appid.txt"
        )
    endif()

    # Copy Resources folder to output directory after build
    add_custom_command(TARGET ${GAME_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${RESOURCE_DIR_NAME}" "$<TARGET_FILE_DIR:${GAME_NAME}>/${RESOURCE_DIR_NAME}"
    )

endif()

target_link_libraries(${GAME_NAME} PRIVATE ${ENGINE_NAME})
target_link_directories(${GAME_NAME} PRIVATE ${BINARY_DIR})

# Copying SDL to game dir
foreach (DLL_TARGET IN LISTS SDL_RUNTIME_LIBRARIES)
add_custom_command(
	TARGET ${GAME_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"$<TARGET_FILE:${DLL_TARGET}>"
	${CMAKE_CURRENT_BINARY_DIR}
	VERBATIM
)
endforeach ()

