cmake_minimum_required(VERSION 3.16)
project(learn_opengl)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# external dependencies
include(CPM)
set(BUILD_TESTING OFF)
CPMAddPackage(
        NAME glfw
        GIT_REPOSITORY git@github.com:glfw/glfw.git
        GIT_TAG 3.3.8
)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(hello_triangle src/getting_started/hello_triangle.cpp src/glad.c)
add_executable(hello_triangle_indexed src/getting_started/hello_triangle_indexed.cpp src/glad.c)
add_executable(shader_uniform src/getting_started/shader_uniform.cpp src/glad.c)

target_link_libraries(hello_triangle glfw)
target_link_libraries(hello_triangle_indexed glfw)
target_link_libraries(shader_uniform glfw)