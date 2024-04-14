
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// window size
unsigned int WINDOW_WIDTH = 800;
unsigned int WINDOW_HEIGHT = 600;

// callback functions

// inputs
void
process_input(GLFWwindow* window);

int
main()
{

	// step1 - Window Creation using windowing system
	//------------------------------------------------
	// Init GLFW
	//-----------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// forward compatiblity
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// create window
	//--------------
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "learn_opengl", nullptr, nullptr);

	if (!window)
	{
		std::cout << "GLFW: Failed to create window!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// step2- Load Opengl functions using GLAD
	//------------------------------------------------------
	// Init GLAD
	//------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD: Failed to load opengl funcs!\n";
		return -1;
	}

	while (!glfwWindowShouldClose(window))
	{
		// process inputs 
		//---------------
		process_input(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}

void
process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}