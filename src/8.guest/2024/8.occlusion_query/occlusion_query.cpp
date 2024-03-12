// #include <GL/gl.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/camera.h>
#include <learnopengl/shader.h>

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void
processKeyboardInputs(GLFWwindow* window);

void
mouseCursorCallback(GLFWwindow* window, double xpos, double ypos);

void
framebuffer_size_callback(GLFWwindow* window, int width, int height);

int
main()
{

	// glfw: initialize and configure
	//--------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "occlusion query", nullptr, nullptr);

	// register window callbacks
	//--------------------------
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouseCursorCallback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all opengl functions pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// setup vertex data (and buffers(s)) and configure vertex attributes
	//-------------------------------------------------------------------
	float vertices[] = {-0.5f, -0.5f, -0.5f, 0.0f,	0.0f,  -1.0f, 0.5f,	 -0.5f, -0.5f, 0.0f,  0.0f,	 -1.0f,
						0.5f,  0.5f,  -0.5f, 0.0f,	0.0f,  -1.0f, 0.5f,	 0.5f,	-0.5f, 0.0f,  0.0f,	 -1.0f,
						-0.5f, 0.5f,  -0.5f, 0.0f,	0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,	 -1.0f,

						-0.5f, -0.5f, 0.5f,	 0.0f,	0.0f,  1.0f,  0.5f,	 -0.5f, 0.5f,  0.0f,  0.0f,	 1.0f,
						0.5f,  0.5f,  0.5f,	 0.0f,	0.0f,  1.0f,  0.5f,	 0.5f,	0.5f,  0.0f,  0.0f,	 1.0f,
						-0.5f, 0.5f,  0.5f,	 0.0f,	0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,	 1.0f,

						-0.5f, 0.5f,  0.5f,	 -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,	-0.5f, -1.0f, 0.0f,	 0.0f,
						-0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,	 0.0f,
						-0.5f, -0.5f, 0.5f,	 -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,	0.5f,  -1.0f, 0.0f,	 0.0f,

						0.5f,  0.5f,  0.5f,	 1.0f,	0.0f,  0.0f,  0.5f,	 0.5f,	-0.5f, 1.0f,  0.0f,	 0.0f,
						0.5f,  -0.5f, -0.5f, 1.0f,	0.0f,  0.0f,  0.5f,	 -0.5f, -0.5f, 1.0f,  0.0f,	 0.0f,
						0.5f,  -0.5f, 0.5f,	 1.0f,	0.0f,  0.0f,  0.5f,	 0.5f,	0.5f,  1.0f,  0.0f,	 0.0f,

						-0.5f, -0.5f, -0.5f, 0.0f,	-1.0f, 0.0f,  0.5f,	 -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,
						0.5f,  -0.5f, 0.5f,	 0.0f,	-1.0f, 0.0f,  0.5f,	 -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
						-0.5f, -0.5f, 0.5f,	 0.0f,	-1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,

						-0.5f, 0.5f,  -0.5f, 0.0f,	1.0f,  0.0f,  0.5f,	 0.5f,	-0.5f, 0.0f,  1.0f,	 0.0f,
						0.5f,  0.5f,  0.5f,	 0.0f,	1.0f,  0.0f,  0.5f,	 0.5f,	0.5f,  0.0f,  1.0f,	 0.0f,
						-0.5f, 0.5f,  0.5f,	 0.0f,	1.0f,  0.0f,  -0.5f, 0.5f,	-0.5f, 0.0f,  1.0f,	 0.0f};

	// upload to gpu
	unsigned int VBO, cube1VAO, cube2VAO;
	glGenVertexArrays(1, &cube1VAO);
	glBindVertexArray(cube1VAO);
	// cuber buffer
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// vertices Attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is
	// also a 3D cube)
	glGenVertexArrays(1, &cube2VAO);
	glBindVertexArray(cube2VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// process shaders
	Shader cubeShader("8.occlusion_tesh.vs", "8.occlusion_tesh.fs");

	while (!glfwWindowShouldClose(window))
	{
		// per frame time logic
		// ------------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime= currentFrame- lastFrame;
		lastFrame= currentFrame;
	}
}

void
processKeyboardInputs(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void
mouseCursorCallback(GLFWwindow* window, double xposIn, double yposIn)
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

	std::cout << "xoffset: " << xoffset << "\n";
	camera.ProcessMouseMovement(xoffset, yoffset);
}

void
framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}