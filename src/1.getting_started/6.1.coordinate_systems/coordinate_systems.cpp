#include <common.h>
#include <shader_m.h>

// settings
const char *TITLE = "learn_opengl -- coordinate system";
void generate_indexed_vertices(float **vertices, int nverts, int **indices, int nids);

int main()
{
	// glfw:initialization, configuration and window creation
	//--------------------------------------------------------
	GLFWwindow *window = create_glfw_window(TITLE, SCR_WIDTH, SCR_HEIGHT);

	// GLAD initialization
	if (!initialize_opengl_context(window))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// register resize call back function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// build and compile our shader program
	//--------------------------------------------------------
	Shader ourShader("6.1.coordinate_systems.vs", "6.1.coordinate_systems.fs");

	// generate indexed triangle and texture coordinates,
	//--------------------------------------------------------
	float *vertices;
	int *indices;
	int nverts, nids;
	generate_indexed_vertices(&vertices, nverts, &indices, nids);

	// copy vertices to gpu
	//--------------------------------------------------------
}

void generate_indexed_vertices(float **vertices, int &nverts, int **indices, int &nids)
{
	// no of vertices attribute 3 for coordinates and 2 for texture
	nverts = 2 * (3 + 2);
	nids = 6;

	*vertices = new float[nverts]{
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
						  uint &VAO, uint &VBO, uint &EBO)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, nverts * sizeof(float), *vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ARRAY_BUFFER, nids * sizeof(int), *indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}