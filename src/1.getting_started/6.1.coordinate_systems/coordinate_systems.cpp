#include <learnopengl/common.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/shader_m.h>
#include <stb_image.h>

// settings
const char *TITLE = "learn_opengl -- coordinate system";

// steps
void generate_indexed_vertices(float **vertices, int &nverts, int **indices,
                               int &nids);
void copy_vertices_to_gpu(float **vertices, int nverts, int **indices, int nids,
                          uint &VAO, uint &VBO, uint &EBO);

void generate_texture(uint &texture, const char *img, bool flip);
void render_loop(GLFWwindow *window, Shader &shader, uint VAO, uint texture1,
                 uint texture2);

int main() {
  // glfw:initialization, configuration and window creation
  //--------------------------------------------------------
  GLFWwindow *window = create_glfw_window(TITLE, SCR_WIDTH, SCR_HEIGHT);

  // GLAD initialization
  if (!initialize_opengl_context(window)) {
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
  uint VAO, VBO, EBO;
  copy_vertices_to_gpu(&vertices, nverts, &indices, nids, VAO, VBO, EBO);

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

  // tell opengl for each sampler to which texture unit it belongs to (only has
  // to be done once)
  // -------------------------------------------------------------------------------------------
  ourShader.use();
  ourShader.setInt("texture1", 0);
  ourShader.setInt("texture2", 1);

  // Render Loop
  render_loop(window, ourShader, VAO, texture1, texture2);

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------
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
void generate_indexed_vertices(float **vertices, int &nverts, int **indices,
                               int &nids) {
  // no of vertices attribute 3 for coordinates and 2 for texture
  nverts = 4 * (3 + 2);
  nids = 6;

  *vertices = new float[nverts]{
      0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 0.0f, 1.0f  // top left
  };

  *indices = new int[nids]{
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle

  };
}

void copy_vertices_to_gpu(float **vertices, int nverts, int **indices, int nids,
                          uint &VAO, uint &VBO, uint &EBO) {
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, nverts * sizeof(float), *vertices,
               GL_STATIC_DRAW);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, nids * sizeof(int), *indices,
               GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                 (nrChannels == 4 ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
}

void render_loop(GLFWwindow *window, Shader &shader, uint VAO, uint texture1,
                 uint texture2) {
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
    projection =
        glm::perspective(glm::radians(45.0f),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    // retrieve the matrix uniform locations
    unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
    unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");

    // pass them to the shaders (3 different ways)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    // note: currently we set the projection matrix each frame, but since the
    // projection matrix rarely changes it's often best practice to set it
    // outside the main loop only once.
    shader.setMat4("projection", projection);

    // render container
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}