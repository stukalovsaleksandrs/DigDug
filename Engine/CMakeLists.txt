#message(WARNING "Output dir: ${CMAKE_BINARY_DIR}/Engine")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Engine")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Engine")

################################
# Library creation
################################
set(SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Source")
set(HEADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Include")

# NOTE: I am aware that it is discouraged to use GLOB to list source files,
# but reconfiguring CMake after adding a new file is faster than
# manually adding a path to every new source file.
file(GLOB_RECURSE ENGINE_SOURCES "${SOURCE_PATH}/*.cpp")
# Headers for VS to display in Solution Explorer
file(GLOB_RECURSE ENGINE_HEADERS "${HEADER_PATH}/*.h" "${HEADER_PATH}/*.hpp")

add_library(${ENGINE_NAME} STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})

target_include_directories(${ENGINE_NAME}
        PUBLIC ${HEADER_PATH}# For the games to use
        PRIVATE ${HEADER_PATH}/${ENGINE_NAME}# For the internal use
)

target_compile_features(${ENGINE_NAME} PUBLIC cxx_std_23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

if(MSVC OR CLANG OR GCC)
  # compile at warning level 4 + treat warnings as errors
  target_compile_options(${ENGINE_NAME} PRIVATE /W4)
else()
  target_compile_options(${ENGINE_NAME} PRIVATE -Wall -Wextra -Werror)
endif()

