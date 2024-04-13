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
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>

// tetxure loading
#include <stb_image.h>

#include <iostream>
// callbacks

void
framebuffer_size_callback(GLFWwindow* window, int width, int height);
void
mouse_callback(GLFWwindow* window, double xpos, double ypos);
void
scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void
process_input(GLFWwindow* window);
unsigned int
load_texture(const char* path);

// settings
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)WINDOW_WIDTH / 2.0;
float lastY = (float)WINDOW_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
	// specifies whether the OpenGL context should be forward-compatible, i.e. one
	// where all functionality deprecated in the requested version of OpenGL is
	// removed. This must only be used if the requested OpenGL version is 3.0 or
	// above. If OpenGL ES is requested, this hint is ignored.
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, true);
#endif

	// create GLFW window
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "learn_opengl>>depth_test", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "GLFW: Failed to create window!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD: Failed to init glad!\n";
		return -1;
	}

	// global opengl state
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS); //  always pass the depth test (same effect as
							//  glDisable(GL_DEPTH_TEST))

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float cubeVertices[] = {
		// positions          // texture Coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,	-0.5f, -0.5f, 1.0f, 0.0f, 0.5f,	 0.5f,	-0.5f, 1.0f, 1.0f,
		0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f,	 0.0f, 0.0f, 0.5f,	-0.5f, 0.5f,  1.0f, 0.0f, 0.5f,	 0.5f,	0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,	 1.0f, 1.0f, -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

		-0.5f, 0.5f,  0.5f,	 1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,	0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,	 1.0f, 0.0f, 0.5f,	0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,	 -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,	-0.5f, 0.5f,  0.0f, 0.0f, 0.5f,	 0.5f,	0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,	-0.5f, -0.5f, 1.0f, 1.0f, 0.5f,	 -0.5f, 0.5f,  1.0f, 0.0f,
		0.5f,  -0.5f, 0.5f,	 1.0f, 0.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,	0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,	 0.5f,	0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,	 1.0f, 0.0f, -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,	-0.5f, 0.0f, 1.0f};

	float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1
		// (together with GL_REPEAT as texture
		// wrapping mode). this will cause the floor texture to repeat)
		5.0f, -0.5f, 5.0f, 2.0f, 0.0f, -5.0f, -0.5f, 5.0f,	0.0f, 0.0f, -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

		5.0f, -0.5f, 5.0f, 2.0f, 0.0f, -5.0f, -0.5f, -5.0f, 0.0f, 2.0f, 5.0f,  -0.5f, -5.0f, 2.0f, 2.0f};

	// upload geometry data to gpu -- cube
	GLuint cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	// upload geomtry data to gpu -- plane
	GLuint planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glBindVertexArray(planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	// Shaders
	Shader shader("1.3.depth_testing.vs", "1.3.depth_testing.fs");
	shader.use();
	shader.setInt("texture1", 0);

	// Textures
	unsigned int cube_texture = load_texture("../../resources/textures/marble.jpg");

	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		process_input(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection =
			glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		// cubes
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cube_texture);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// floor

		// window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// release GPU resources
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);

	// terminate glfw resource
	glfwTerminate();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void
process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void
framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void
mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void
scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int
load_texture(const char* path)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	int width, height;
	int nrComp;
	unsigned char* data = stbi_load(path, &width, &height, &nrComp, 0);
	if (data)
	{
		GLenum format;
		if (nrComp == 1)
			format = GL_RED;
		if (nrComp == 3)
			format = GL_RGB;
		if (nrComp == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load image from path!\n";
		stbi_image_free(data);
	}
	return texture;
}