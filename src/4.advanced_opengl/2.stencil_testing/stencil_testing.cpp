#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>
#include <learnopengl/camera.h>
#include <learnopengl/shader.h>

// window size
unsigned int WINDOW_WIDTH = 800;
unsigned int WINDOW_HEIGHT = 600;
// camera
Camera camera;

// callback functions

void
process_input(GLFWwindow* window);

unsigned int
load_texture(const char* path);

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

	// step2- Load opengl functions using GLAD
	//------------------------------------------------------
	// Init GLAD
	//------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD: Failed to load opengl funcs!\n";
		return -1;
	}

	// step3 -set up vertex data (and buffer(s)) and configure vertex attributes
	// -------------------------------------------------------------------------
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
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture
		// wrapping mode). this will cause the floor texture to repeat)
		5.0f, -0.5f, 5.0f, 2.0f, 0.0f, -5.0f, -0.5f, 5.0f,	0.0f, 0.0f, -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

		5.0f, -0.5f, 5.0f, 2.0f, 0.0f, -5.0f, -0.5f, -5.0f, 0.0f, 2.0f, 5.0f,  -0.5f, -5.0f, 2.0f, 2.0f};

	// upload data to GPU
	//------------------
	// CUBE -- [upload --> vertices + texture coordinates]
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	// CUBE -- attributes -- vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// CUBE -- attributes -- texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// FLOOR -- [upload --> vertices + texture coordinates]
	unsigned int floorVAO, floorVBO;
	glGenVertexArrays(1, &floorVAO);
	glBindVertexArray(floorVAO);
	glGenBuffers(1, &floorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	// FLOOR -- attributes -- vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// FLOOR -- attributes -- texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// step3 -- Load and compile shaders
	//----------------------------------
	Shader shader("2.stencil_testing.vs", "2.stencil_testing.fs");
	shader.use();

	// step4 -- Load and bind textures
	//--------------------------------codec
	// load texttures
	unsigned int cubeTexture = load_texture("../../resources/textures/marble.jpg");
	unsigned int floorTexture = load_texture("../../resources/textures/metal.png");
	// unsigned int cube_texture
	while (!glfwWindowShouldClose(window))
	{
		// process inputs
		//---------------
		process_input(window);

		// prepare frame
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// set model, view, projection
		shader.use();
		glm::mat4 model = glm::mat4(1.0);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection =
			glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.f);

		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		// Bind VAOs
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		// DRAW Triangles
		glDrawArrays(GL_TRIANGLES, 0, 36);

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

unsigned int
load_texture(const char* path)
{
	unsigned int texture;
	glGenTextures(1, &texture);

	int width, height, ncrComp;
	unsigned char* data = stbi_load(path, &width, &height, &ncrComp, 0);
	GLenum format;
	if (data)
	{
		if (ncrComp == 1)
			format = GL_RED;
		else if (ncrComp == 3)
			format = GL_RGB;
		else if (ncrComp == 4)
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
		std::cout << "stb: Failed to load image file!\n";
		stbi_image_free(data);
	}
	return texture;
}
