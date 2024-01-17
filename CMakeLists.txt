cmake_minimum_required(VERSION 3.16)

project(learn_opengl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

if(WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(WIN32)

configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

# external dependencies
include(CPM)
set(BUILD_TESTING OFF)
CPMAddPackage(
	NAME glfw
	GIT_REPOSITORY git@github.com:glfw/glfw.git
	GIT_TAG 3.3.8
)

# We manually download GLM into our source directory so the user can access
# glm through 'glm/...'. There is no build step since this is a header only
# library.
CPMAddPackage(
	NAME glm
	GIT_REPOSITORY git@github.com:g-truc/glm.git
	GIT_TAG 0.9.9.8
	DOWNLOAD_ONLY True
)
include_directories(${glm_SOURCE_DIR}/)



include_directories(${CMAKE_SOURCE_DIR}/include)

# first create relevant static libraries required for other projects
add_library(GLAD src/glad.c)
set(LIBS ${LIBS} glfw GLAD)

add_library(STB_IMAGE "src/stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)

macro(makeLink src dest target)
  add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${src} ${dest}  DEPENDS  ${dest} COMMENT "mklink ${src} -> ${dest}")
endmacro()

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
	4.1.textures
	4.2.textures_combined
	# 4.4.textures_exercise2
	# 4.5.textures_exercise3
	# 4.6.textures_exercise4
	5.1.transformations
	# 5.2.transformations_exercise2
	6.1.coordinate_systems
	# 6.2.coordinate_systems_depth
	# 6.3.coordinate_systems_multiple
	7.1.camera_circle
	# 7.2.camera_keyboard_dt
	# 7.3.camera_mouse_zoom
	# 7.4.camera_class
	)


	set(4.advanced_opengl
    1.1.depth_testing
    # 1.2.depth_testing_view
    # 2.stencil_testing
    # 3.1.blending_discard
    # 3.2.blending_sort
    # 5.1.framebuffers
    # 5.2.framebuffers_exercise1
    # 6.1.cubemaps_skybox
    # 6.2.cubemaps_environment_mapping
    # 8.advanced_glsl_ubo
    # 9.1.geometry_shader_houses
    # 9.2.geometry_shader_exploding
    # 9.3.geometry_shader_normals
    # 10.1.instancing_quads
    # 10.2.asteroids
    # 10.3.asteroids_instanced
    # 11.1.anti_aliasing_msaa
    # 11.2.anti_aliasing_offscreen
)

  function(create_project_from_sources chapter demo)
	  file(GLOB SOURCE
			  "src/${chapter}/${demo}/*.h"
			  "src/${chapter}/${demo}/*.cpp"
			  "src/${chapter}/${demo}/*.vs"
			  "src/${chapter}/${demo}/*.fs"
			  "src/${chapter}/${demo}/*.tcs"
			  "src/${chapter}/${demo}/*.tes"
			  "src/${chapter}/${demo}/*.gs"
			  "src/${chapter}/${demo}/*.cs"
	  )
	  if (demo STREQUAL "")
		  SET(replaced "")
		  string(REPLACE "/" "_" replaced ${chapter})
		  set(NAME ${replaced})
	  else()
		  set(NAME "${chapter}__${demo}")
	  endif()
	  add_executable(${NAME} ${SOURCE})
	  target_link_libraries(${NAME} ${LIBS})
	  if(MSVC)
		  target_compile_options(${NAME} PRIVATE /std:c++17 /MP)
		  target_link_options(${NAME} PUBLIC /ignore:4099)
	  endif(MSVC)
	  if(WIN32)
		  set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}")
		  set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}/Debug")
	  elseif(UNIX AND NOT APPLE)
		  set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}")
	  elseif(APPLE)
		  set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}")
		  set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/${chapter}")
		  set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/${chapter}")
	  endif(WIN32)
	  # copy shader files to build directory
	  file(GLOB SHADERS
			   "src/${chapter}/${demo}/*.vs"
			   # "src/${chapter}/${demo}/*.frag"
			   "src/${chapter}/${demo}/*.fs"
			   "src/${chapter}/${demo}/*.tcs"
			   "src/${chapter}/${demo}/*.tes"
			   "src/${chapter}/${demo}/*.gs"
			   "src/${chapter}/${demo}/*.cs"
	  )
	  # copy dlls
	  file(GLOB DLLS "dlls/*.dll")
	  foreach(SHADER ${SHADERS})
		  if(WIN32)
			  # configure_file(${SHADER} "test")
			  add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${NAME}>)
			  add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${DLLS} $<TARGET_FILE_DIR:${NAME}>)
		  elseif(UNIX AND NOT APPLE)
			  file(COPY ${SHADER} DESTINATION ${CMAKE_SOURCE_DIR}/bin/${chapter})
		  elseif(APPLE)
			  # create symbolic link for *.vs *.fs *.gs
			  get_filename_component(SHADERNAME ${SHADER} NAME)
			  makeLink(${SHADER} ${CMAKE_SOURCE_DIR}/bin/${chapter}/${SHADERNAME} ${NAME})
		  endif(WIN32)
	  endforeach(SHADER)
	  # if compiling for visual studio, also use configure file for each project (specifically to set up working directory)
	  if(MSVC)
		  configure_file(${CMAKE_SOURCE_DIR}/configuration/visualstudio.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.vcxproj.user @ONLY)
	  endif(MSVC)
  	endfunction()
  
	# then create a project file per tutorial
	foreach(CHAPTER ${CHAPTERS})
		foreach(DEMO ${${CHAPTER}})
			create_project_from_sources(${CHAPTER} ${DEMO})
		endforeach(DEMO)
	endforeach(CHAPTER)
