file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS *.cpp *.h)

add_library(Engine SHARED ${ENGINE_SOURCES})

# PHYSX BEGIN
set(PHYSX_BUILD_TYPE "checked" CACHE STRING "The build type of PhysX")
set_property(CACHE PHYSX_BUILD_TYPE PROPERTY STRINGS debug checked profile release)

# Assume PhysX has been built manually and you're linking it manually
set(PHYSX_ROOT "${CMAKE_SOURCE_DIR}/ext/PhysX/physx")
set(PHYSX_LIB_DIR "${PHYSX_ROOT}/bin/win.x86_64.vc143.mt/${PHYSX_BUILD_TYPE}")
set(PHYSX_INCLUDE_DIR "${PHYSX_ROOT}/include")

# Example for an Engine library that depends on PhysX
target_include_directories(Engine PRIVATE "${PHYSX_INCLUDE_DIR}")
target_link_directories(Engine PRIVATE "${PHYSX_LIB_DIR}")
# PHYSX END

target_compile_definitions(Engine PRIVATE ENGINE_EXPORTS)

find_package(SDL3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(Engine PRIVATE
    SDL3::SDL3
    glad::glad
    assimp::assimp
    imgui::imgui
    EnTT::EnTT
    glm::glm
    nlohmann_json::nlohmann_json

    PhysXExtensions_static_64
    PhysXPvdSDK_static_64
    PhysXCommon_64
    PhysXFoundation_64
)
