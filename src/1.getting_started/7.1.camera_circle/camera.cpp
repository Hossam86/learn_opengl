// In this chapter we'll discuss how we can set up a camera in OpenGL. We will discuss
// a fly style camera that allows you to freely move around in a 3D scene.
// We'll also discuss keyboard and mouse input and finish with a custom camera class.
#include <common.h>
#include <stb_image.h>
#include <shader_m.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const char *TITLE = "learn_opengl -- camera";

// generate cube vertices
void generate_cube_vertices(float **vertices, int &nverts);

// copy vertices attributes to gpu
void copy_vertices_to_gpu(float **vertices, int nverts, uint &VAO, uint &VBO);

// generate texture maps
void generate_texture(uint &texture, const char *img, bool flip);

// render loop
void render_loop(GLFWwindow *window, Shader &shader, uint VAO, uint texture1, uint texture2);

int main() {
	GLFWwindow *window = create_glfw_window(TITLE, SCR_WIDTH, SCR_HEIGHT);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		return -1;
	}

	if (!initialize_opengl_context(window)) {
		std::cout << "Failed to init OpenGL funcs" << std::endl;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	float *vertices;
	int *indices;
	int nverts, nids;

	generate_cube_vertices(&vertices, nverts);

	uint VAO, VBO;
	copy_vertices_to_gpu(&vertices, nverts, VAO, VBO);

	// generate textures
	uint texture1, texture2;
	// texture 1
	// ---------
	// tell stb_image.h to flip loaded texture's on the y-axis.
	generate_texture(texture1, "../../resources/textures/container.jpg", true);

	// texture 2
	//  note that the awesomeface.png has transparency and thus an alpha channel,
	// so make sure to tell OpenGL the data type is of GL_RGBA
	generate_texture(texture2, "../../resources/textures/awesomeface.png", false);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	Shader shader("7.1.camera.vs", "7.1.camera.fs");

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
	// -----------------------------------------------------------------------------------------------------------
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
	shader.setMat4("projection", projection);
}

void generate_cube_vertices(float **vertices, int &nverts) {
	nverts = 6 * 6 * 5; // 6 faces 6 vertices each  5 attributes
	*vertices = new float[nverts]{
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f};
}

void copy_vertices_to_gpu(float **vertices, int nverts, uint &VAO, uint &VBO) {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, nverts * sizeof(float), *vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

void generate_texture(uint &texture, const char *img, bool flip) {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps

	stbi_set_flip_vertically_on_load(flip);
	int width, height, nrChannels;

	unsigned char *data = stbi_load(img, &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, (nrChannels == 4 ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE,
					 data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void render_loop(GLFWwindow *window, Shader &shader, uint VAO, uint texture1, uint texture2) {
	while (!glfwWindowShouldClose(window)) {
		/* code */

		// process user inputs
		processInput(window);
		// clear background
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// activate textures-- bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// activate shader
		//----------------------------------------------
		shader.use();

		// create transformation
		//----------------------------------------------
		glm::mat4 model = glm::mat4(1.0);
		glm::mat4 view = glm::mat4(1.0);
		glm::mat4 projection = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(-55.f), glm::vec3(1.0, 0, 0));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);

		// retrieve the matrix uniform locations
		unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
		unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");

		// pass them to the shaders (3 different ways)
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		// note: currently we set the projection matrix each frame, but since the projection
		// matrix rarely changes it's often best practice to set it outside the main loop only once.
		shader.setMat4("projection", projection);

		// render container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}