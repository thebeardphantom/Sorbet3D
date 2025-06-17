file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS *.cpp *.h)

add_library(sorbengine SHARED ${ENGINE_SOURCES})

target_compile_definitions(sorbengine PRIVATE ENGINE_EXPORTS)
target_include_directories(sorbengine PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/proj/sorbengine/include>)

find_package(SDL3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)


target_link_libraries(sorbengine PRIVATE
    SDL3::SDL3
    glad::glad
    assimp::assimp
    imgui::imgui
    EnTT::EnTT
    glm::glm
    nlohmann_json::nlohmann_json
)
