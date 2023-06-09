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

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(hello_triangle src/getting_started/hello_triangle.cpp src/glad.c)
add_executable(hello_triangle_indexed src/getting_started/hello_triangle_indexed.cpp src/glad.c)
add_executable(shader_uniform src/getting_started/shader_uniform.cpp src/glad.c)
add_executable(shaders_interpolation src/getting_started/shaders_interpolation.cpp src/glad.c)
add_executable(shaders_class src/getting_started/shaders_class.cpp src/glad.c include/shader_s.h)
add_executable(textures src/getting_started/textures.cpp src/glad.c include/stb_image.h)

target_link_libraries(hello_triangle glfw)
target_link_libraries(hello_triangle_indexed glfw)
target_link_libraries(shader_uniform glfw)
target_link_libraries(shaders_interpolation glfw)
target_link_libraries(shaders_class glfw)
target_link_libraries(textures glfw)