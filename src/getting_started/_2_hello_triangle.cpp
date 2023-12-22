#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// shader source
const char *vertexShaderSource = "#version 330 core\n"
								 "layout (location = 0) in vec3 aPos;\n"
								 "void main()\n"
								 "{\n"
								 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
								 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
								   "out vec4 FragColor;\n"
								   "\n"
								   "void main()\n"
								   "{\n"
								   "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
								   "} ";

// Everything is in 3D space, but the screen or window is a 2D array of pixels so a large part of OpenGL's
//  work is about transforming all 3D coordinates to 2D pixels that fit on your screen
// The process of transforming 3D coordinates to 2D pixels is managed by the graphics pipeline of OpenGL.
// the first transforms your 3D coordinates into 2D coordinates and the second part transforms the 2D coordinates
// into actual colored pixels.
// graphics pipeline - given vertex_data[]
//-------------------
// vertex_shader -> geometry_shader-> shape_assembly-> rasterization->fragment_shader->testing_and_blending
// -> vertex shader transform 3D coordinates into different 3D coordinates
// -> geometry shader generate other shapes by emitting new vertices to form new (or other) primitive(s).
// -> primitive assembly takes all the vertices from the vertex (or geometry) shader that form one or more
//		primitives and assembles all the point(s) in the primitive shape given
// -> rasterization stage where it maps the resulting primitive(s) to the corresponding pixels on the final
//		screen, resulting in fragments for the fragment shader to use.
// -> Before the fragment shaders run, clipping is performed. Clipping discards all fragments that are outside
//		your view, increasing performance.
// -> fragment shader calculate the final color of a pixel and Usually the fragment shader contains data
//		about the 3D scene that it can use to calculate the final pixel color
//		(like lights, shadows, color of the light and so on).
// -> the graphics pipeline is quite a complex whole and contains many configurable parts. However,
//		for almost all the cases we only have to work with the vertex and fragment shader.
//		The geometry shader is optional and usually left to its default shader

int main()
{
	// glfw:initialization and configuration
	//-------------------------------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw: window creation
	//-------------------------------------------------------
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "learn_Opengl", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GlFW Window! " << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// glad: load all opengl function pointers (We should initialize GLAD before we call any OpenGL function)
	//-------------------------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// register call back function to change view port when the window size changed
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// build and compile our shader program
	//-------------------------------------------------------
	// vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}

	// fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}

	// link shader
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
				  << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left
		0.5f, -0.5f, 0.0f,	// right
		0.0f, 0.5f, 0.0f	// top
	};

	// with the vertex data defined, we want to send it to pipeline entry gate (vertex_shader) this done by creating
	// memory on the GPU to store the vertex data, configure how should OpenGL interpret the memory  and specify how
	// to send the data to graphics processor. this can be done through vertex buffer object (VBO)

	// Just like any object in OpenGL, so we can generate one with a buffer ID using the glGenBuffers function:

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// OpenGL has many types of buffer objects and the buffer type of a vertex buffer object is GL_ARRAY_BUFFER.
	//  OpenGL allows us to bind to several buffers at once as long as they have a different buffer type.
	//  We can bind the newly created buffer to the GL_ARRAY_BUFFER target with the glBindBuffer function:
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		//  ------------------------------------------------------------------
		processInput(window);

		// render
		//  ------------------------------------------------------------------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangles
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// ------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}