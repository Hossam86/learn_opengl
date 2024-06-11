#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/model.h>

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;
const char* TITLE = "learn_opengl--cube-maps";

void
process_input(GLFWwindow* window);
void
framebuffer_size_callback(GLFWwindow* window, int width, int height);
void
mouse_callback(GLFWwindow* window, double xpos, double ypos);
void
scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

unsigned int
load_texture(const char* path);

unsigned int
load_cupemap(vector<std::string> faces);

int
main()
{
	// glfw init
	//------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	//---------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, TITLE, nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// register callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "glad: failed to load opengl functions\n";
	}

	while (!glfwWindowShouldClose(window))
	{
		// process inputs
		process_input(window);

		// render loop
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glfwTerminate();
}

void
process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void
scroll_callback(GLFWwindow* window, double xoffest, double yoffset)
{
}

void
mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
}

void
framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

unsigned int
load_texture(const char* path)
{
}

unsigned int
load_cubemap(vector<std::string> faces)
{
}
