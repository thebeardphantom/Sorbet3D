file(GLOB_RECURSE GAME_EDITOR_SOURCES CONFIGURE_DEPENDS *.cpp *.h)

add_library(game_editor SHARED ${GAME_EDITOR_SOURCES})

find_package(SDL3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(game_editor PRIVATE
    SDL3::SDL3
    glad::glad
    imgui::imgui
    EnTT::EnTT
    glm::glm
    nlohmann_json::nlohmann_json
    game
    sorbeditor
)
