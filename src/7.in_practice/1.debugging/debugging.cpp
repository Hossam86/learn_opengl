#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

// settings
const unsigned int SCR_WIDH = 800;
const unsigned int SCR_HEIGHT = 600;

void
framebuffer_size_callback(GLFWwindow* window, int width, int height);

void
process_input(GLFWwindow* window);
GLenum
glCheckError_(const char* file, int line)
{
	GLenum error_code;
	while ((error_code = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (error_code)
		{
		case GL_INVALID_ENUM:
			error = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			error = "INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			error = "INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			error = "STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			error = "STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			error = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return error_code;
}
#define glCheckError() glCheckError_(__FILE__, ___LINE_)

// Debug output is core since OpenGL version 4.3, which means you'll find this functionality on any
// machine that runs OpenGL 4.3 or higher.
// In order to start using debug output we have to request a debug output context from OpenGL at our initialization
// process. This process varies based on whatever windowing system you use; here we will discuss setting it up on GLFW,
// but you can find info on other systems in the additional resources at the end of the chapter.

// Requesting a debug context in GLFW is surprisingly easy as all we have to do is pass a hint to GLFW that we'd like to
// have a debug output context. We have to do this before we call glfwCreateWindow:
void APIENTRY
glDebugOutput(
	GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam)
{
	// ignoew non-signficant error/warming codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
		return;
	std::cout << "-------------------" << std::endl;
	std::cout << "Debug message (" << id << ")" << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		std::cout << "Source: API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		std::cout << "Source: Window System";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		std::cout << "Source: Shader Compiler";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		std::cout << "Source: Third Party";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		std::cout << "Source: Application";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		std::cout << "Source: Other";
		break;
	}
	std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		std::cout << "Type: Error";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		std::cout << "Type: Deprecated Behaviour";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		std::cout << "Type: Undefined Behaviour";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		std::cout << "Type: Portability";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		std::cout << "Type: Performance";
		break;
	case GL_DEBUG_TYPE_MARKER:
		std::cout << "Type: Marker";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		std::cout << "Type: Push Group";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		std::cout << "Type: Pop Group";
		break;
	case GL_DEBUG_TYPE_OTHER:
		std::cout << "Type: Other";
		break;
	}
	std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		std::cout << "Severity: high";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		std::cout << "Severity: medium";
		break;
	case GL_DEBUG_SEVERITY_LOW:
		std::cout << "Severity: low";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		std::cout << "Severity: notification";
		break;
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

int
main()
{
	// glfw: initlize and configure
	//------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); // comment this line in a release build!

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPACT, GL_TRUE);
#endif

	// glfw: window creation
	//------------------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDH, SCR_HEIGHT, "learn_opengl-debug ouput", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (window == NULL)
	{
		std::cout << "glfw: Failed to create window! \n";
		glfwTerminate();
		return -1;
	}

	// call back functions
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//! glad: load all opengl function pointers
	//-----------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "glad: Failed to load opengl functions\n";
		return -1;
	}

	// enable Opengl debug context if context allows for debug context
	//----------------------------------------------------------------
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	Shader shader("debugging.vs", "debugging.fs");
	// configure 3D cube
	float vertices[] = {
		// back face
		-0.5f,
		-0.5f,
		-0.5f,
		0.0f,
		0.0f, // bottom-left
		0.5f,
		0.5f,
		-0.5f,
		1.0f,
		1.0f, // top-right
		0.5f,
		-0.5f,
		-0.5f,
		1.0f,
		0.0f, // bottom-right
		0.5f,
		0.5f,
		-0.5f,
		1.0f,
		1.0f, // top-right
		-0.5f,
		-0.5f,
		-0.5f,
		0.0f,
		0.0f, // bottom-left
		-0.5f,
		0.5f,
		-0.5f,
		0.0f,
		1.0f, // top-left
			  // front face
		-0.5f,
		-0.5f,
		0.5f,
		0.0f,
		0.0f, // bottom-left
		0.5f,
		-0.5f,
		0.5f,
		1.0f,
		0.0f, // bottom-right
		0.5f,
		0.5f,
		0.5f,
		1.0f,
		1.0f, // top-right
		0.5f,
		0.5f,
		0.5f,
		1.0f,
		1.0f, // top-right
		-0.5f,
		0.5f,
		0.5f,
		0.0f,
		1.0f, // top-left
		-0.5f,
		-0.5f,
		0.5f,
		0.0f,
		0.0f, // bottom-left
			  // left face
		-0.5f,
		0.5f,
		0.5f,
		-1.0f,
		0.0f, // top-right
		-0.5f,
		0.5f,
		-0.5f,
		-1.0f,
		1.0f, // top-left
		-0.5f,
		-0.5f,
		-0.5f,
		-0.0f,
		1.0f, // bottom-left
		-0.5f,
		-0.5f,
		-0.5f,
		-0.0f,
		1.0f, // bottom-left
		-0.5f,
		-0.5f,
		0.5f,
		-0.0f,
		0.0f, // bottom-right
		-0.5f,
		0.5f,
		0.5f,
		-1.0f,
		0.0f, // top-right
		// right face
		0.5f,
		0.5f,
		0.5f,
		1.0f,
		0.0f, // top-left
		0.5f,
		-0.5f,
		-0.5f,
		0.0f,
		1.0f, // bottom-right
		0.5f,
		0.5f,
		-0.5f,
		1.0f,
		1.0f, // top-right
		0.5f,
		-0.5f,
		-0.5f,
		0.0f,
		1.0f, // bottom-right
		0.5f,
		0.5f,
		0.5f,
		1.0f,
		0.0f, // top-left
		0.5f,
		-0.5f,
		0.5f,
		0.0f,
		0.0f, // bottom-left
			  // bottom face
		-0.5f,
		-0.5f,
		-0.5f,
		0.0f,
		1.0f, // top-right
		0.5f,
		-0.5f,
		-0.5f,
		1.0f,
		1.0f, // top-left
		0.5f,
		-0.5f,
		0.5f,
		1.0f,
		0.0f, // bottom-left
		0.5f,
		-0.5f,
		0.5f,
		1.0f,
		0.0f, // bottom-left
		-0.5f,
		-0.5f,
		0.5f,
		0.0f,
		0.0f, // bottom-right
		-0.5f,
		-0.5f,
		-0.5f,
		0.0f,
		1.0f, // top-right
			  // top face
		-0.5f,
		0.5f,
		-0.5f,
		0.0f,
		1.0f, // top-left
		0.5f,
		0.5f,
		0.5f,
		1.0f,
		0.0f, // bottom-right
		0.5f,
		0.5f,
		-0.5f,
		1.0f,
		1.0f, // top-right
		0.5f,
		0.5f,
		0.5f,
		1.0f,
		0.0f, // bottom-right
		-0.5f,
		0.5f,
		-0.5f,
		0.0f,
		1.0f, // top-left
		-0.5f,
		0.5f,
		0.5f,
		0.0f,
		0.0f // bottom-left
	};

	// upload data to gpu
	//---------------------------------
	// Generate buffers
	//----------------
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	// fill buffers
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// link vertex attributes
	//----------------------
	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// load cube texture
	//------------------
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	int width, height, nrComponents;
	unsigned char* data = stbi_load("../../resources/textures/wood.png", &width, &height, &nrComponents, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // fix 
		// glTexImage2D(GL_FRAMEBUFFER, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "texture: Failed to load texture!" << std::endl;
	}
	stbi_image_free(data);

	// setup projection matrix
	//--------------------------
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDH / (float)SCR_HEIGHT, 0.1f, 10.0f);
	shader.use(); // fix 
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform1i(glGetUniformLocation(shader.ID, "tex"), 0);

	// configure global opengl state
	//-------------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		//------
		process_input(window);

		// render
		//------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		float rotationSpeed = 10.0f;
		float angle = (float)glfwGetTime() * rotationSpeed;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0, 0.0f, -2.5));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void
process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
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
