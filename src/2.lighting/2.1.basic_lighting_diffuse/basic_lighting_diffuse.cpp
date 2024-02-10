
#include "GLFW/glfw3.h"
int
main()
{
	glfwInit();
	glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwInitHint(GLFW_OPENGL_CORE_PROFILE, true);

#if __APPLE__
	glfwInitHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#endif

	GLFWwindow* window = glfwCreateWindow(800, 600, "learn_opengl:diffuse_light", NULL, NULL);

	while (!glfwWindowShouldClose(window))
	{

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}