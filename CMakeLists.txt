message(STATUS "VCPKG_ROOT: $ENV{VCPKG_ROOT}")
cmake_minimum_required(VERSION 4.0.0)
project(engine_app LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(ProjDirName "proj")

add_subdirectory(${ProjDirName}/sorbengine)
add_subdirectory(${ProjDirName}/sorbeditor)
add_subdirectory(${ProjDirName}/engine_editor)
add_subdirectory(${ProjDirName}/game)
add_subdirectory(${ProjDirName}/game_editor)
add_executable(engine_app ${ProjDirName}/engine_app/src/engine_app_entry.cpp)

# engine_app needs SDL3 for app entry
find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(engine_app PRIVATE
    SDL3::SDL3
    sorbengine)

add_custom_target(copy_sorbengine_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/${ProjDirName}/sorbengine/Assets
            $<TARGET_FILE_DIR:engine_app>/Assets
    COMMENT "Copying sorbengine assets to output directory."
)

add_custom_target(copy_game_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/${ProjDirName}/game/Assets
            $<TARGET_FILE_DIR:engine_app>/Assets
    COMMENT "Copying Game assets to output directory."
)

add_custom_target(copy_config ALL
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/${ProjDirName}/sorbengine/engine_default.toml
            $<TARGET_FILE_DIR:engine_app>/engine_default.toml
    COMMENT "Copying engine_default.toml to output directory."
)

add_dependencies(engine_app copy_sorbengine_assets copy_game_assets copy_config)