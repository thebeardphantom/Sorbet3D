message(STATUS "VCPKG_ROOT: $ENV{VCPKG_ROOT}")
cmake_minimum_required(VERSION 4.0.0)
project(EngineApp LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

#add_subdirectory(deps/SDL EXCLUDE_FROM_ALL)
add_subdirectory(src/Engine)
add_subdirectory(src/Game)
add_subdirectory(src/Editor)
add_subdirectory(src/EngineEditor)
add_subdirectory(src/GameEditor)

add_executable(EngineApp
    src/EngineApp/engine_app_entry.cpp
)
find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(EngineApp PRIVATE
    SDL3::SDL3
    Engine
)

add_custom_target(copy_engine_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/src/Engine/Assets
            $<TARGET_FILE_DIR:EngineApp>/Assets
    COMMENT "Copying Engine assets to output directory"
)

add_custom_target(copy_game_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/src/Game/Assets
            $<TARGET_FILE_DIR:EngineApp>/Assets
    COMMENT "Copying game assets to output directory"
)

add_custom_target(copy_config ALL
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/src/Engine/engine_default.toml
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_default.toml
    COMMENT "Copying config.toml on each build"
)

add_dependencies(EngineApp copy_engine_assets copy_game_assets copy_config)