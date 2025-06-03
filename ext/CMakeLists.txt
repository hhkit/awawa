set(DILIGENT_NO_OPENGL ON)
add_subdirectory(DiligentCore)

# FetchContent
include(FetchContent)
include(${CMAKE_SOURCE_DIR}/cmake/FindSlang.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/SlangUtils.cmake)

# glfw
FetchContent_Declare(
    glfw 
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG 3.4)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)