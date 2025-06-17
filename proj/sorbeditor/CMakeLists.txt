file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS *.cpp *.h)

add_library(sorbeditor SHARED ${EDITOR_SOURCES})

find_package(glad CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

target_include_directories(sorbeditor PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/proj/sorbeditor/include>)

target_link_libraries(sorbeditor PRIVATE
    SDL3::SDL3
    glad::glad
    assimp::assimp
    imgui::imgui
    EnTT::EnTT
    glm::glm
    nlohmann_json::nlohmann_json
    sorbengine
)
