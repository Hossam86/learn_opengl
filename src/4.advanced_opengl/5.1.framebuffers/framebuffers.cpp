#include <learnopengl/camera.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <learnopengl/shader.h>
#include <stb_image.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// callback funcs
// void
// framebuffer_size_callback(GLFWwindow* window, int width, int height);

// void
// mouse_callback(GLFWwindow* window, double xpos, double ypos);

// void
// scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// void
// process_input(GLFWwindow* window);

// load texture
// unsigned int
// loadTexture(const char* path);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int
main()
{
	// glfw: initialize and configure
	//-------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#endif

	// create window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "learn_opengl>>frame_buffers", nullptr, NULL);
	if (window == NULL)
	{
		std::cout << "glfw: failed to create window!\n";
	}

	while (!glfwWindowShouldClose(window))
	{

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}