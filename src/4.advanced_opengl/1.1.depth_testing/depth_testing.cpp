// Depth testing
// In the coordinate systems chapter we've rendered a 3D container and made use
// of a depth buffer to prevent triangles rendering in the front while they're
// supposed to be behind other triangles. In this chapter we're going to
// elaborate a bit more on those depth values the depth buffer (or z-buffer)
// stores and how it actually determines if a fragment is in front.

// The depth-buffer is a buffer that, just like the color buffer (that stores
// all the fragment colors: the visual output), stores information per fragment
// and has the same width and height as the color buffer. The depth buffer is
// automatically created by the windowing system and stores its depth values as
// 16, 24 or 32 bit floats. In most systems you'll see a depth buffer with a
// precision of 24 bits.

// When depth testing is enabled, OpenGL tests the depth value of a fragment
// against the content of the depth buffer. OpenGL performs a depth test and if
// this test passes, the fragment is rendered and the depth buffer is updated
// with the new depth value. If the depth test fails, the fragment is discarded.

// Depth testing is done in screen space after the fragment shader has run (and
// after the stencil test which we'll get to in the next chapter). The screen
// space coordinates relate directly to the viewport defined by OpenGL's
// glViewport function and can be accessed via GLSL's built-in gl_FragCoord
// variable in the fragment shader. The x and y components of gl_FragCoord
// represent the fragment's screen-space coordinates (with (0,0) being the
// bottom-left corner). The gl_FragCoord variable also contains a z-component
// which contains the depth value of the fragment. This z value is the value
// that is compared to the depth buffer's content.

// Today most GPUs support a hardware feature called early depth testing. Early
// depth testing allows the depth test to run before the fragment shader runs.
// Whenever it is clear a fragment isn't going to be visible (it is behind other
// objects) we can prematurely discard the fragment.

// Fragment shaders are usually quite expensive so wherever we can avoid running
// them we should. A restriction on the fragment shader for early depth testing
// is that you shouldn't write to the fragment's depth value. If a fragment
// shader would write to its depth value, early depth testing is impossible;
// OpenGL won't be able to figure out the depth value beforehand.

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// window size
unsigned int WINDOW_WIDTH = 800;
unsigned int WINDOW_HEIGHT = 600;

// process user inputs
void
process_inputs(GLFWwindow* window);

int
main()
{

	// Init glfw
	glfwInit();
	// glfw opengl context hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
	// specifies whether the OpenGL context should be forward-compatible, i.e. one where all functionality deprecated in
	// the requested version of OpenGL is removed. This must only be used if the requested OpenGL version is 3.0 or
	// above. If OpenGL ES is requested, this hint is ignored.
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, true)
#endif

		// create GLFW window
		GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "learn_opengl", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "GLFW: Failed to create window!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD: Failed to init glad!\n";
		return -1;
	}

	// global opengl state
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS); //  always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))

	while (!glfwWindowShouldClose(window))
	{
		// process user inputs
		process_inputs(window);

		// background color
		glClearColor(0.1, 0.1, 0.1, 0.5);
		// clear color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT);

		// window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// terminate glfw resource
	glfwTerminate();
}

void
process_inputs(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}