#include <common.h>
#include <shader_s.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// settings
const char *TITLE = "learn_opengl  -- transformation";

void generate_indexed_triangle(float **vertices, int &nverts, int **indices, int &nids);

void copy_vertices_to_gpu(float **vertices, int nverts, int **indices, int nids,
						  uint &VBO, uint &VAO, uint &EBO);
void generate_texture(uint &texture, bool flip, const char *img);

void render_loop(GLFWwindow *window, Shader &ourShader, uint VAO, uint texture1, uint texture2);

int main()
{
	// glfw: initialize,configure and window creation
	// ------------------------------------------------------------------
	GLFWwindow *window = create_glfw_window(TITLE, SCR_WIDTH, SCR_HEIGHT);

	// glad: load all OpenGL function pointers
	// ------------------------------------------------------------------
	if (!initialize_opengl_context(window))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	// ------------------------------------------------------------------
	Shader ourShader("5.1.transform.vs", "5.1.transform.fs");

	// set up vertex data
	// ------------------------------------------------------------------
	float *vertices;
	int *indices;
	int nverts, nids;
	generate_indexed_triangle(&vertices, nverts, &indices, nids);

	// gpu buffers and configure vertex attributes
	//  ------------------------------------------------------------------
	uint VBO, VAO, EBO;
	copy_vertices_to_gpu(&vertices, nverts, &indices, nids, VBO, VAO, EBO);

	// load and create a texture
	//  ------------------------------------------------------------------
	unsigned int texture1, texture2;
	// texture 1
	// ---------
	// tell stb_image.h to flip loaded texture's on the y-axis.
	generate_texture(texture1, true, "../../resources/textures/container.jpg");
	// texture 2
	// ---------
	// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL
	// the data type is of GL_RGBA
	generate_texture(texture2, false, "../../resources/textures/awesomeface.png");

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	// render
	//  ------------------------------------------------------------------
	render_loop(window, ourShader, VAO, texture1, texture2);

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	delete[] vertices;
	delete[] indices;

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

void generate_indexed_triangle(float **vertices, int &nverts, int **indices, int &nids)
{
	nverts = (3 + 2) * 4;
	nids = 6;
	*vertices = new float[nverts]{
		// positions          // texture coords
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,	// top right
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,	// bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f	// top left
	};
	*indices = new int[nids]{
		0, 1, 3, // first triangle
		1, 2, 3	 // second triangle
	};
}

void copy_vertices_to_gpu(float **vertices, int nverts, int **indices, int nids,
						  uint &VBO, uint &VAO, uint &EBO)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, nverts * sizeof(float), *vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nids * sizeof(int), *indices, GL_STATIC_DRAW);

	// configure vertex coordinates attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// configure vertex texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

void generate_texture(uint &texture, bool flip, const char *img)
{
	// load and create texture
	glGenTextures(1, &texture);

	// all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping parameter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform;
	// replace it with your own image path.
	unsigned char *data = stbi_load(img, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	// free image data
	stbi_image_free(data);
}

void render_loop(GLFWwindow *window, Shader &ourShader, uint VAO, uint texture1, uint texture2)
{
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// create transformations
		glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		// get matrix's uniform location and set matrix
		ourShader.use();
		unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		// render container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}