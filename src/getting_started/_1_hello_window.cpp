// ref https://learnopengl.com/Getting-started/Creating-a-window
// important definitions
// OpenGL Context->

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

// callback function on the window that gets called each time the window is resized.
void frame_size_buffer_callback(GLFWwindow *window, int width, int height);

// We also want to have some form of input control in GLFW and we can achieve this with several of GLFW's input
//  functions. We'll be using GLFW's glfwGetKey function that takes the window as input together with a key.
//  The function returns whether this key is currently being pressed. We're creating a processInput function
// to keep all input code organized
void processInput(GLFWwindow *window);

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
    GLFWwindow *window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // we want to initialize GLAD before we call any OpenGL function:
    //-------------------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    // We do have to tell GLFW we want to call this function on every window resize by registering it:
    glfwSetFramebufferSizeCallback(window, frame_size_buffer_callback);

    // We don't want the application to draw a single image and then immediately quit and close the window.
    //  We want the application to keep drawing images and handling user input until the program has been
    // explicitly told to stop. For this reason we have to create a while loop, that we now call the render
    //  loop, that keeps on running until we tell GLFW to stop.
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // process input
        processInput(window);

        // render commands,
        //....

        // As you may recall from the OpenGL chapter, the glClearColor function is a state-setting function
        // and glClear is a state-using function in that it uses the current state to retrieve the clearing
        // color from.
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // check and call events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // s soon as we exit the render loop we would like to properly clean/delete all of GLFW's resources that
    //  were allocated. We can do this via the glfwTerminate function that we call at the end of the main function.
    glfwTerminate();
    return 0;
}

void frame_size_buffer_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}