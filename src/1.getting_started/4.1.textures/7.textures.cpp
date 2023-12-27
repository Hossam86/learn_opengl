#include <common.h>
#include <shader_s.h>
#include <stb_image.h>

const char *TITLE = "learn_opengl -- textures";

// Textures
// We learned that to add more detail to our objects we can use colors for each vertex to create some
// interesting images. However, to get a fair bit of realism we'd have to have many vertices so we could
// specify a lot of colors. This takes up a considerable amount of extra overhead, since each model needs
// a lot more vertices and for each vertex a color attribute as well. What artists and programmers generally
// prefer is to use a texture. A texture is a 2D image (even 1D and 3D textures exist) used to add detail to
// an object; think of a texture as a piece of paper with a nice brick image (for example) on it neatly folded
// over your 3D house so it looks like your house has a stone exterior. Because we can insert a lot of detail
// in a single image, we can give the illusion the object is extremely detailed without having to specify extra
// vertices.

// generate colored indexed triangle with texture
void generate_colored_triangle_with_texture(float **vertices, unsigned int *nverts, unsigned int **indices, unsigned int *n_ind);
// copy vertices and texture to GPU
void copy_vertices_to_gpu(float *vertices, unsigned int nverts, unsigned int *indices, unsigned int nids,
						  unsigned int VAO, unsigned int VBO, unsigned int EBO);

void generate_texture(unsigned int &texture, const char *img);

// render loop
void render_loop(GLFWwindow *window, Shader &ourShader, unsigned int VAO, unsigned int texture);

int main()
{

	// glfw:initialization, configuration and window creation
	//-------------------------------------------------------
	GLFWwindow *window = create_glfw_window(TITLE, SCR_WIDTH, SCR_HEIGHT);
	if (window == NULL)
	{
		std::cout << "Failed to create GlFW Window! " << std::endl;
		glfwTerminate();
		return -1;
	}
	// glad: load all opengl function pointers (We should initialize GLAD before we call any OpenGL function)
	//-------------------------------------------------------
	if (!initialize_opengl_context(window))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// register call back function to change view port when the window size changed
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// build and compile our shader program
	// ------------------------------------
	Shader ourShader("4.1.texture.vs", "4.1.texture.fs");

	// setup vertices data,
	// ------------------------------------------------------------------
	float *vertices;
	unsigned int *indices;
	unsigned int nverts, nids;
	generate_colored_triangle_with_texture(&vertices, &nverts, &indices, &nids);

	//  gpu buffers and configure vertex attributes
	unsigned int VAO, VBO, EBO, texture;
	copy_vertices_to_gpu(vertices, nverts, indices, nids, VAO, VBO, EBO);

	// create texture
	generate_texture(texture, "resources/textures/container.jpg");

	// render
	render_loop(window, ourShader, VAO, texture);

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// free vertices data
	delete[] vertices;
	delete[] indices;

	// free window
	glfwTerminate();
}

void generate_colored_triangle_with_texture(float **vertices, unsigned int *nverts, unsigned int **indices,
											unsigned int *nind)
{
	*nverts = (4 * 3) + (4 * 3) + (4 * 2); // coordinates + colors+ textures
	*nind = 6;							   // two triangles
	*vertices = new float[*nverts]{
		// positions          // colors           // texture coords
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,	  // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f	  // top left
	};

	*indices = new unsigned int[*nind]{
		0, 1, 3, // first triangle
		1, 2, 3	 // second triangle
	};
}

void copy_vertices_to_gpu(float *vertices, unsigned int nverts, unsigned int *indices, unsigned int nids,
						  unsigned int VAO, unsigned int VBO, unsigned int EBO)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, nverts * sizeof(*vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ARRAY_BUFFER, EBO);
	glBufferData(GL_ARRAY_BUFFER, nids * sizeof(*indices), indices, GL_STATIC_DRAW);

	// configure vertex coordinates attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// configure vertex color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// configure vertex texture attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void generate_texture(unsigned int &texture, const char *img)
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform;
	// replace it with your own image path.
	unsigned char *data = stbi_load(img, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	// free image data
	stbi_image_free(data);
}

void render_loop(GLFWwindow *window, Shader &ourShader, unsigned int VAO, unsigned int texture)
{
	// render loop
	//---------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// bind texture
		glBindTexture(GL_TEXTURE_2D, texture);

		// render container
		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// swap buffers and poll io events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}