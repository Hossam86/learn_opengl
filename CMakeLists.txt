cmake_minimum_required(VERSION 3.16)

project(learn_opengl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# external dependencies
include(CPM)
set(BUILD_TESTING OFF)
CPMAddPackage(
        NAME glfw
        GIT_REPOSITORY git@github.com:glfw/glfw.git
        GIT_TAG 3.3.8
)

CPMAddPackage(
        NAME glm
        GIT_REPOSITORY git@github.com:g-truc/glm.git
        GIT_TAG 0.9.9.8
)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src/_01_getting_started)

