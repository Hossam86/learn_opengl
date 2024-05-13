#include <cmath>
#include <learnopengl/common.h>
// settings
const char *TITLE = "learn opengl -- shader_uniforms";

// shader source
const char *vertexShaderSource2 =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource2 = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "uniform vec4 ourColor;\n"
                                    "\n"
                                    "void main()\n"
                                    "{\n"
                                    "    FragColor = ourColor;\n"
                                    "} ";

float *generate_single_triangle();
GLuint process_shader_program();
void copy_vertices_to_gpu(float *vertices, GLuint &VBO, GLuint &VAO);
void render_loop(GLFWwindow *window, GLuint shader_program, GLuint VAO);

int main() {
  // glfw:window creation, initialization and configuration
  //-------------------------------------------------------
  GLFWwindow *window = create_glfw_window(TITLE, SCR_WIDTH, SCR_HEIGHT);
  if (window == NULL) {
    std::cout << "Failed to create GlFW Window! " << std::endl;
    glfwTerminate();
    return -1;
  }
  // glad: load all opengl function pointers (We should initialize GLAD before
  // we call any OpenGL function)
  if (!initialize_opengl_context(window))
    return -1;

  // register call back function to change view port when the window size
  // changed
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float *vertices = generate_single_triangle();

  // build and compile our shader program
  //-------------------------------------------------------------------
  GLuint shader_program = process_shader_program();

  unsigned int VBO, VAO;
  copy_vertices_to_gpu(vertices, VBO, VAO);

  render_loop(window, shader_program, VAO);

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shader_program);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

float *generate_single_triangle() {
  // set up vertex data
  // ------------------------------------------------------------------
  float *vertices = new float[9]{
      -0.5f, -0.5f, 0.0f, // left
      0.5f,  -0.5f, 0.0f, // right
      0.0f,  0.5f,  0.0f  // top
  };
  return vertices;
}

GLuint process_shader_program() {
  // build and compile our shader program
  //-------------------------------------------------------
  // vertex shader -> first thing we need to do is create a shader object,
  // referenced by an ID
  unsigned int vertexShader;
  // We provide the type of shader we want to create as an argument
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource2, NULL);
  glCompileShader(vertexShader);
  // check for shader compile errors
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // fragment shader
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource2, NULL);
  glCompileShader(fragmentShader);
  // check for shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
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
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  // Oh yeah, and don't forget to delete the shader objects once we've linked
  // them into the program object; we no longer need them anymore:
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  return shaderProgram;
}

void copy_vertices_to_gpu(float *vertices, GLuint &VBO, GLuint &VAO) {
  // with the vertex data defined, we want to send it to pipeline entry gate
  // (vertex_shader) this done by creating memory on the GPU to store the vertex
  // data, configure how should OpenGL interpret the memory  and specify how to
  // send the data to graphics processor. this can be done through vertex buffer
  // object (VBO) Just like any object in OpenGL, so we can generate one with a
  // buffer ID using the glGenBuffers function:
  glGenBuffers(1, &VBO);

  // To use a VAO all you have to do is bind the VAO using glBindVertexArray.
  // From that point on we should bind/configure the corresponding VBO(s) and
  // attribute pointer(s) and then unbind the VAO for later use. As soon as we
  // want to draw an object, we simply bind the VAO with the preferred settings
  // before drawing the object and that is it. In code this would look a bit
  // like this:
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // OpenGL has many types of buffer objects and the buffer type of a vertex
  // buffer object is GL_ARRAY_BUFFER.
  //  OpenGL allows us to bind to several buffers at once as long as they have a
  //  different buffer type. We can bind the newly created buffer to the
  //  GL_ARRAY_BUFFER target with the glBindBuffer function:
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // Then we can make a call to the glBufferData function that copies the
  // previously defined vertex data into the buffer's memory:
  glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);

  // tell OpenGL how it should interpret the vertex data (per vertex attribute)
  // using glVertexAttribPointer:
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally
  // modify this VAO, but this rarely happens. Modifying other VAOs requires a
  // call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
  // VBOs) when it's not directly necessary.
  glBindVertexArray(0);
}

void render_loop(GLFWwindow *window, GLuint shader_program, GLuint VAO) {
  // render loop
  while (!glfwWindowShouldClose(window)) {
    // input
    // ------------------------------------------------------------------
    processInput(window);

    // render
    // ------------------------------------------------------------------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw our first triangles
    glUseProgram(shader_program);

    // bind vertex array
    glBindVertexArray(VAO);

    // update shader uniform
    double timeValue = glfwGetTime();
    float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
    int vertexColorLocation = glGetUniformLocation(shader_program, "ourColor");
    // updating a uniform does require you to first use the program (by calling
    // glUseProgram),
    //  because it sets the uniform on the currently active shader program.
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // ------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}