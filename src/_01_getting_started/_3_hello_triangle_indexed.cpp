#include <iostream>
#include "common.h"

// settings
const char *TITLE = "learn_opengl >> indexed_triangle";

void generate_indexed_triangles(float **vertices, int *n_verts, int **indices, int *n_indices);

void copy_vertices_to_gpu(float **vertices, int n_verts, int **indices, int n_indices, GLuint &VBO, GLuint &VAO, GLuint &EBO);

GLuint process_shader_program();

void render_loop(GLFWwindow *window, GLuint shader_program, GLuint VAO);

int main()
{
    GLFWwindow *window = create_glfw_window(TITLE, SCR_WIDTH, SCR_HEIGHT);

    // check for success
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window " << std::endl;
        glfwTerminate();
        return -1;
    }

    // initialize opengl funcs
    if (!initialize_opengl_context(window))
        return -1;

    // compile and link shader
    GLuint shader_program = process_shader_program();

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float *vertices;
    int *indices;
    int n_indices, n_verts;

    generate_indexed_triangles(&vertices, &n_verts, &indices, &n_indices);

    GLuint VBO, VAO, EBO;
    copy_vertices_to_gpu(&vertices, n_verts, &indices, n_indices, VBO, VAO, EBO);

    // Render
    render_loop(window, shader_program, VAO);

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader_program);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    // delete vertices
    // ------------------------------------------------------------------
    delete[] vertices;
    delete[] indices;
    return 0;
}

void generate_indexed_triangles(float **vertices, int *n_verts, int **indices, int *n_indices)
{
    *n_verts = 12;
    *n_indices = 6;
    *vertices = new float[*n_verts]{
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
    };
    *indices = new int[*n_indices]{
        // note that we start from 0!
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };
}

void copy_vertices_to_gpu(float **vertices, int n_verts, int **indices, int n_indices, GLuint &VBO, GLuint &VAO, GLuint &EBO)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 1. bind the Vertex Array Object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, n_verts * sizeof(float), *vertices, GL_STATIC_DRAW);

    // 3. copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_indices * sizeof(int), *indices, GL_STATIC_DRAW);

    // 4. then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}

GLuint process_shader_program()
{
    // build and compile our shader program
    //-------------------------------------------------------
    // vertex shader -> first thing we need to do is create a shader object, referenced by an ID
    unsigned int vertexShader;
    // We provide the type of shader we want to create as an argument
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

    // Oh yeah, and don't forget to delete the shader objects once we've linked them into the program object;
    // we no longer need them anymore:
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

void render_loop(GLFWwindow *window, GLuint shader_program, GLuint VAO)
{
    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

        // draw our first triangle
        glUseProgram(shader_program);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}