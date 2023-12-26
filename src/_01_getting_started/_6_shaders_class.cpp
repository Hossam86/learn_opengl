#include <common.h>
#include <shader_s.h>

const char *TITLE = "learn_opengl -- green card ";

float *generate_colored_triangle();
void copy_vertices_to_gpu(float *vertices, GLuint &VBO, GLuint &VAO);
// build and compile our shader program
GLuint process_shader_program();
void render_loop(GLFWwindow *window, Shader &shader, GLuint VAO);

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

    // build and compile our shader program usinf shader class
    // you can name your shader files however you like
    // ------------------------------------
    Shader ourShader("shaders/3.3.shader.vs", "shaders/3.3.shader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float *vertices = generate_colored_triangle();
    unsigned int VBO, VAO;
    copy_vertices_to_gpu(vertices, VBO, VAO);

    render_loop(window, ourShader, VAO);

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    delete[] vertices;
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

float *generate_colored_triangle()
{
    float *vertices = new float[18]{
        // positions         // colors
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // top

    };

    return vertices;
}

void copy_vertices_to_gpu(float *vertices, GLuint &VBO, GLuint &VAO)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void render_loop(GLFWwindow *window, Shader &our_shader, GLuint VAO)
{ // bind the VAO (it was already bound, but just to demonstrate): seeing as we only have a single VAO we can
    // just bind it beforehand before rendering the respective triangle; this is another approach.
    glBindVertexArray(VAO);

    our_shader.use();

    while (!glfwWindowShouldClose(window))
    {
        // user input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangles
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // ------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}