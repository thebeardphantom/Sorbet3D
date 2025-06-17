file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS *.cpp *.h)

add_library(Editor SHARED ${EDITOR_SOURCES})

#target_precompile_headers(Editor PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:pch.h>")

find_package(SDL3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(Editor PRIVATE
    Engine
    SDL3::SDL3
    glad::glad
    assimp::assimp
    imgui::imgui
    EnTT::EnTT
    glm::glm
    nlohmann_json::nlohmann_json
)
