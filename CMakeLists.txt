cmake_minimum_required(VERSION 3.16)
project(learn_opengl)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

# external dependencies
include(CPM)
set(BUILD_TESTING OFF)
CPMAddPackage(
        NAME glfw
        GIT_REPOSITORY git@github.com:glfw/glfw.git
        GIT_TAG 3.3.8
)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(hello_world src/main.cpp src/glad.c)

target_link_libraries(hello_world glfw)