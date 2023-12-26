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
include_directories(${CMAKE_SOURCE_DIR}/resources)

# first create relevant static libraries required for other projects
add_library(GLAD src/glad.c)
set(LIBS ${LIBS} GLAD)

set (CHAPTERS
	1.getting_started
	2.lighting
	3.model_loading
	4.advanced_opengl
	5.advannced_lighting
	6.pbr
	7.game_project)
   
set(1.getting_started
	1.1.hello_window
	# 1.2.hello_window_clear
	2.1.hello_triangle
	2.2.hello_triangle_indexed
	# 2.3.hello_triangle_exercise1
	# 2.4.hello_triangle_exercise2
	# 2.5.hello_triangle_exercise3
	3.1.shaders_uniform
	3.2.shaders_interpolation
	3.3.shaders_class
	# 4.1.textures
	# 4.2.textures_combined
	# 4.4.textures_exercise2
	# 4.5.textures_exercise3
	# 4.6.textures_exercise4
	# 5.1.transformations
	# 5.2.transformations_exercise2
	# 6.1.coordinate_systems
	# 6.2.coordinate_systems_depth
	# 6.3.coordinate_systems_multiple
	# 7.1.camera_circle
	# 7.2.camera_keyboard_dt
	# 7.3.camera_mouse_zoom
	# 7.4.camera_class
	)

function(create_project_from_sources chapter demo)
	file (GLOB SOURCE 
		src/${chapter}/${demo}/*.h
		src/${chapter}/${demo}/*.cpp
		src/${chapter}/${demo}/*.vs
		src/${chapter}/${demo}/*.fs)
	if (demo STREQUAL "")
		set(replaced "")
		string(REPLACE "/" "_" replaced ${chapter})
		set(NAME ${replaced})
	else()
		set(NAME "${chapter}__${demo}")
	endif()
	add_executable(${NAME} ${SOURCE})
	target_link_libraries(${NAME} ${LIBS})
endfunction()

foreach(CHAPTER ${CHAPTERS})
	foreach(DEMO  ${${CHAPTER}})
		create_project_from_sources(${CHAPTER} ${DEMO})
	endforeach(DEMO )
endforeach(CHAPTER)
