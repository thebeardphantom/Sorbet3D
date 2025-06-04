file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS *.cpp *.h)

add_library(Game SHARED ${GAME_SOURCES})

#target_precompile_headers(Engine PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:pch.h>")

find_package(SDL3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(Game PRIVATE
    Engine
    SDL3::SDL3
    glad::glad
    assimp::assimp
    imgui::imgui
    EnTT::EnTT
    glm::glm
    nlohmann_json::nlohmann_json
)
