cmake_minimum_required(VERSION 3.31.6)
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
